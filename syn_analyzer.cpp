/*!
\file
\brief Recursive descent

The file contains the description of recursive descent functions of syntactic
analyzer class, which analyzes regular expression and transfers it to Polish Notation
*/
#include "syn_analyzer.h"

void Syn_analyzer::S()
{
    S1();
    A();
}

void Syn_analyzer::S1()
{
    F();
    O();
    K();
}

void Syn_analyzer::F()
{
    if(t_lex == LEX_LPAREN) {
        gl();
        while(!temp.empty()) {
            polish.push_back(temp.top());
            temp.pop();
        }
        vector<Lex> v = polish;
        polish.erase(polish.begin(), polish.end());
        S();
        for(auto it = polish.rbegin(); it != polish.rend(); it++){
            temp.push(*it);
        }
        polish = v;
        if(t_lex != LEX_RPAREN) {
            throw t_lex;
        }
        gl();
    } else if (t_lex == LEX_TERM) {
        temp.push(cur_lex);
        gl();
    } else {
        throw t_lex;
    }
}

void Syn_analyzer::O()
{
    if(t_lex == LEX_ITER || t_lex == LEX_LAZY_COPY || t_lex == LEX_REPEAT) {
        temp.push(cur_lex);
        gl();
        O();
    }
    while(!temp.empty()) {
        polish.push_back(temp.top());
        temp.pop();
    }
}

void Syn_analyzer::K()
{
    if(t_lex == LEX_CONCAT) {
        vector<Lex> v;
        v.push_back(cur_lex);
        v.insert(v.end(), polish.begin(), polish.end());
        polish = v;
        gl();
        S1();
    }
}

void Syn_analyzer::A()
{
    if(t_lex == LEX_OR) {
        vector<Lex> v;
        v.push_back(cur_lex);
        v.insert(v.end(), polish.begin(), polish.end());
        polish = v;
        gl();
        polish.erase(polish.begin(), polish.end());
        S();
        v.insert(v.end(), polish.begin(), polish.end());
        polish = v;
    }
}
