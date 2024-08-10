#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<math.h>

using namespace std;

string space_rem(string s)                  //remove unnecessary spaces for further processing
{
    int i;
    string res;
    for(i=0;i<s.size();i++)
    {
        if(s[i] != ' ')
        res+=s[i];
    }
    return res;
}

string all_in_powers(string s)        //writes every variable in string in its power form x^(i)
{                                     //e.g. abca is written as a^(1)b^(1)c^(1)a^(1)
    string temp = s;

    if(temp.size()==0)
    return temp;

    int i;

    if(temp[temp.size()-1]>='a'&&temp[temp.size()-1]<='z')
    temp+="^(1)";

    string res;

    char x;

    for(i=0;i<temp.size()-3;i++)
    {
        x=temp[i];
        
        if(x>='a' && x<='z' && temp[i+1]!='^')     //if variable not in power form
        {
            res+=x;
            res+="^(1)";                           //writes ab as a^(1)b^(1)
        }

        else
        {
            res+=x;                          //variable in power form so copy as it is
        }
    }

    for(i=temp.size()-3;i<temp.size();i++)
    {
        res+=temp[i];
    }
    return res;
}

class polynomial_term                   //stores everything regarding a polynomial term
{
    public:
        int powers[26]={0};              //power of each variable from a to z
        string pol;                      //string representation of term without coefficient
        int coeff;                       //coefficient of term
        char sign;                      //positive or negative
        int pow_sum;                      //sum of powers 
    
    public:
        void full_term(string s);            //inputs string form and puts all the data of the term in this class's object
        void string_construct();             //makes the string representation pol from the powers array
        void pol_pow(string s);              //gathers data of total power of each variable from input string
        void display();                       //displays the complete polynomial term
        polynomial_term operator * (polynomial_term);    //to multiply two terms   
        bool operator < (polynomial_term);                //lexical order
        bool operator > (polynomial_term);               //lexical order
        bool operator == (polynomial_term);             //like terms
        void display_pow();                             //powers of each variable a to z displayed seperately
        polynomial_term operator + (polynomial_term);   //add two polynomial terms
        void copy_frm(polynomial_term);      //copy all data from a polynomial
        polynomial_term operator - (polynomial_term);

};

void polynomial_term::copy_frm(polynomial_term B)               //copy all data from a polynomial
{
    for(int i=0;i<26;i++)
    {
        powers[i]=B.powers[i];
    }
    coeff=B.coeff;
    pol=B.pol;
    sign=B.sign;
    pow_sum=B.pow_sum;
}

void polynomial_term:: display_pow()              //powers of each variable a to z displayed seperately
{
    for(int i=0;i<26;i++)
    {
        cout<<powers[i]<<" ";
    }
    cout<<endl;
}

void polynomial_term::display()                                    
{
    if(coeff==0)
    {
        cout<<"0"<<endl;
        return;
    }

    if(coeff==1)
    {   
        if(pol.size()!=0)
        cout<<pol<<endl;

        else
        {
            cout<<1<<endl;               //only coefficient printed as pol is empty
        }   
    }        

    else
    {
        if(coeff!=-1)
        cout<<coeff<<pol<<endl;

        else
        cout<<"-"<<pol<<endl;
        
    }
}

void polynomial_term::string_construct()              //generates pol member of polynomial term class using powers array
{
    pol.clear();

    for(int i=0;i<26;i++)
    {
        if(powers[i]!=0)
        {
            pol+=(char)(i+'a');

            if(powers[i]!=1)
            {
                pol+="^(";
                pol+=to_string((powers[i]));
                pol+=')';
            }
        }
    }

}

void polynomial_term::pol_pow(string s)               //gererates the powers array from the string
{
    char x;

    if(s.size()==0)
    return;

    for(int i=0;i<s.length();i++)
    {
        x=s[i];

        if(x>='a'&&x<='z')
        {
            string tmp;
            int pow;

            for(int j=i+3;s[j]!=')';j++)           //gathers power of variable in tmp string              
            {
                tmp+=s[j];
            }
            pow=stoi(tmp);
            powers[x-'a']+=pow;
        }
    }

    pow_sum=0;

    for(int i=0;i<26;i++)
    {
        pow_sum+=powers[i];
    }
}

void polynomial_term::full_term(string s)        //inputs raw string and organises its data in the polynomial_term class object
{
    sign='+';          //default sign

    coeff=1;                    //default coefficient     

    for(int i=0;i<26;i++)
    {
        powers[i]=0;
    }       

    pol.clear();

    pow_sum=0;

    int i=0;

    if(s[0]=='-')
    {
        sign='-';
        i++;
    }

   string tmp;

    while(i<s.size() && s[i]>='0' && s[i]<='9')          //tmp collects the coefficient part
    {
        tmp+=s[i];
        i++;
    }

    if(tmp.size()>0)coeff=stoi(tmp);

    if(sign=='-')                     
    coeff*=-1;

    tmp="";

    if(1)
    {
        for(int j=i;j<s.length();j++)
        {
            tmp+=s[j];
        }

    }

    s=tmp;

    s=all_in_powers(s);
    pol_pow(s);
    string_construct();
}

polynomial_term  polynomial_term::operator * (polynomial_term B)   //returns multiplication of terms
{
    polynomial_term res;

    res.coeff=coeff*B.coeff;

    if(res.coeff<0)
    res.sign='-';

    else
    res.sign='+';
    

    string temp=pol+B.pol;

    temp=all_in_powers(temp);
    res.pol_pow(temp);
    res.string_construct();

    return res;
}

bool polynomial_term::operator < (polynomial_term B)                                      //lexicographic order comparison
{

    for(int i=0; i<26; i++)
    {
        int x,y;

        for(x=i;x<26;x++)
        {
            if(powers[x]!=0)
            break;
        }

        for(y=i;y<26;y++)
        {
            if(B.powers[y]!=0)
            break;
        }

        if(x>y)
        return false;

        if(x<y)
        return true;

        if(powers[i]>B.powers[i])
        {
            int rest_empty=1;

            for(int j=i+1;j<26;j++)
            {
                if(B.powers[j]!=0)
                rest_empty=0;
            }

            if(rest_empty)
            return false;
            

            return true;
        }

        else if(powers[i]<B.powers[i])
        {    
            int rest_empty=1;

            for(int j=i+1;j<26;j++)
            {
                if(powers[j]!=0)
                rest_empty=0;
            }

            if(rest_empty)
            return true;


            return false;              
        }

    }
    return false;
}

bool polynomial_term::operator > (polynomial_term B)                                      //lexicographic order comparison
{
    for(int i=0; i<26; i++)
    {
        if(powers[i]>B.powers[i])
        return true;

        else
        {
            if(powers[i]<B.powers[i])
            return false;

            else
            continue; 
        }
    }
    return false;
}

bool polynomial_term::operator == (polynomial_term B)
{
    for(int i=0;i<26;i++)
    {
        if(powers[i] != B.powers[i])
        return false;
    }
    return true;
}

polynomial_term  polynomial_term::operator + (polynomial_term B)            //return addition of polynomial_terms
{
    polynomial_term C;

    if(pol!=B.pol)
    {
        cout<<"Error: Adding two non_compatible polynomial terms!!\n";
    }

    C.coeff = coeff+B.coeff;

    if(C.coeff<0)
    C.sign='-';

    else 
    C.sign='+';

    C.pol=pol;
    C.pol_pow(all_in_powers(pol));
    C.string_construct();
    return C;
}

polynomial_term  polynomial_term::operator - (polynomial_term B)            //return addition of polynomial_terms
{
    polynomial_term C;

    if(pol!=B.pol)
    {
        cout<<"Error: Subtracting two non_compatible polynomial terms!!\n";
    }

    C.coeff = coeff-B.coeff;

    if(C.coeff<0)
    C.sign='-';

    else 
    C.sign='+';

    C.pol=pol;
    C.pol_pow(all_in_powers(pol));
    C.string_construct();
    return C;
}

class polynomial
{
    public:
        polynomial_term terms[100];
        int term_count;
        string expression;
    
    public:
        void expression_generate();

        polynomial(): term_count(0),expression("0")
        {
            polynomial_term X;
            X.full_term("0");

            for(int i=0;i<10;i++)
            {
                terms[i].copy_frm(X);
            }
        }

        polynomial(string s)                                                      //constructor segregates terms and stores them in terms array
        {
            polynomial_term X;
            X.full_term("0");

            for(int i=0;i<10;i++)
            {
                terms[i].copy_frm(X);
            }

			s=all_in_powers(s);

            polynomial_term tmp;
            string temp="";
            int i;

            for(i=0;!(i!=0 && (s[i]=='+'|| s[i]=='-')) && i<s.size();i++)         //put first term in the array
            {
                temp+=s[i];
            }

            tmp.full_term(temp);
            terms[0]=tmp;
            temp="";
            term_count=1;

            while(i<s.size())
            {
                if(s[i]=='+' )
                {
                    temp="";
                    i++;

                    while(s[i]!='+' && s[i]!='-' && i<s.size())
                    {
                        temp+=s[i];
                        i++;
                    }
                    tmp.full_term(temp);
                    terms[term_count]=tmp;
                    term_count++;
                }

                else if(s[i]=='-')
                {
                    temp="-";
                    i++;

                    while(s[i]!='+' && s[i]!='-' && i<s.size())
                    {
                        temp+=s[i];
                        i++;
                    }
                    tmp.full_term(temp);
                    terms[term_count]=tmp;
                    term_count++;
                }
            }
			expression_generate();
            lexicosort();
            powersort();
        }

        void copy_frm(polynomial);
        void lexicosort();                         //sort in lexical order
        void powersort();                          //sort in increasing power of terms
        void display_terms();                      //dispplay terms seperately
        void display();                            //display polynomial
        bool found(polynomial_term);
        void compress();        
        void negate();                   //add  like terms to simplify polynomial
        void add_term(polynomial_term);            //add a term to existing polynomial
        polynomial operator + (polynomial);        //polynomial addition
        polynomial operator * (polynomial);        //polynomial multiplication
        polynomial operator - (polynomial);
        polynomial power_expand(int);              //polynomial raised to a power
};

void polynomial::copy_frm(polynomial B)
{
    for(int i=0;i<100;i++)
    {
        terms[i].copy_frm(B.terms[i]);
        term_count=B.term_count;
        expression=B.expression;
    }
}

void polynomial::add_term(polynomial_term B)        //add a term to the polynomial
{
    terms[term_count].copy_frm(B);
    term_count++;

    lexicosort();
    powersort();
    expression_generate();


}

void polynomial::display_terms()          //display terms seperately
{
    for(int i =0;i<term_count;i++)
    {
        terms[i].display();
    }
}

void polynomial::display()          //display the polynomial
{
    cout<<expression<<endl;
}

void polynomial::powersort()                          //sort polynomial in increasing power order
{
    polynomial_term least;
    int least_index,i,j;

    for(i=0;i<term_count;i++)
    {
        least = terms[i];
        least_index = i;

        for(j=i;j<term_count;j++)
        {
            if(terms[j].pow_sum<least.pow_sum)
            {
           
                least=terms[j];
                least_index=j;
            }
        }
        polynomial_term temp = terms[i];
        terms[i] = terms[least_index];
        terms[least_index] = temp;
    }

    expression_generate();
}

void polynomial::lexicosort()                                //lexical sorting of polynomial
{
    polynomial_term least;
    int least_index,i,j;

    for(i=0;i<term_count;i++)
    {
        least = terms[i];
        least_index = i;

        for(j=i;j<term_count;j++)
        {
            if(terms[j]<least)
            {
  
                least=terms[j];
                least_index=j;
            }
        }
        polynomial_term temp = terms[i];
        terms[i] = terms[least_index];
        terms[least_index] = temp;
    }

    expression_generate();
}

void polynomial::expression_generate()              //generate string representation using terms array
{
   // cout<<"**\n";
    expression.clear();

    expression="";

    if(term_count==0)
    {
        //cout<<"*\n";
        expression="0";
        return;
    }


    for(int i=0;i<term_count;i++)
    {
        if(terms[i].coeff==0)
        {
            if(i==1)
            {
                expression="0";
                return;
            }

            else
            {
                continue;
            }
        }

        if((terms[i].coeff)!=1 && (terms[i].coeff)!=-1)
        {
            if(i!=0 && terms[i].sign=='+')
            expression += terms[i].sign;

            expression += to_string(terms[i].coeff);
        }

        else
        {
            if(i!=0)
            expression += terms[i].sign;

            else if(terms[i].sign=='-')
            expression += terms[i].sign;
        }
           
        expression += terms[i].pol;
    }
}

bool polynomial::found(polynomial_term X)
{
    for(int i=0;i<term_count;i++)
    {
        if(X==terms[i])
        return true;
    }

    return false;
}

void polynomial:: compress()                    //compresses by grouping similar terms (terms that can be added)
{
    lexicosort();
    expression_generate();
    powersort();
    expression_generate();


    polynomial_term tmp;
    tmp.copy_frm(terms[0]);
 
    polynomial res;

    for(int i=1;i<=term_count;i++)
    {
        if(!(terms[i]==tmp))
        {
            if(tmp.coeff!=0)
            res.add_term(tmp);
            tmp.copy_frm(terms[i]);
        }

        else
        {
            tmp=tmp+terms[i];
        }
    }

    res.expression_generate();

    for(int i=0;i<100;i++)
    {
        terms[i]=res.terms[i];
    }

    expression = res.expression;
    term_count = res.term_count;
}

void polynomial:: negate()
{
    for(int i=0;i<term_count;i++)
    {
        terms[i].coeff = (-1)*terms[i].coeff;

        if(terms[i].sign=='+')
        terms[i].sign='-';

        else
        terms[i].sign='+';

        terms[i].string_construct();
    }
    expression_generate();
}

polynomial polynomial::operator + (polynomial B)           //returns addition of two polynomials  
{
    string temp = expression;
    if(B.expression[0]!='-')temp+="+";
    temp+=B.expression;
    polynomial tmp(temp);
    tmp.compress();
    tmp.expression_generate();
    tmp.lexicosort();
    tmp.powersort();

    return tmp;
}

polynomial polynomial::operator * (polynomial B)                   //polynomial multiplication
{
    polynomial res;

    for(int i=0;i<term_count;i++)
    {
        for(int j=0;j<B.term_count;j++)
        {
            res.add_term(terms[i]*B.terms[j]);
        }
    }

    res.compress();

    return res;

}

polynomial polynomial::operator - (polynomial B)
{
    polynomial temp;
    temp.copy_frm(B);
    temp.negate();

    string tmp = expression;
    if(temp.expression[0]!='-')tmp+="+";
    tmp+=temp.expression;
    polynomial res(tmp);
    res.compress();
    res.expression_generate();
    res.lexicosort();
    res.powersort();

    return res;
}

polynomial polynomial::power_expand(int x)        //returns polynomial raised to a power
{
    polynomial temp;

    for(int i=0;i<term_count;i++)
    {
        temp.terms[i].copy_frm(terms[i]);
    }

    temp.term_count=term_count;
    temp.expression=expression;

    polynomial res; res=temp;

    for(int i=1;i<x;i++)
    {
        res=res*temp;
    }

    return res;

}

int main()
{
    polynomial A("3x-3y^(2)");
    polynomial B("x-a");
    polynomial C=A*B;
    C.display();

}