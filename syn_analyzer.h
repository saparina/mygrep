/*!
\file
\brief The header with description of syntactic analyzer class

The file contains the description of syntactic analyzer class, which
checks belonging to the set of all valid expressions regular expressions
and translates into Polish Notation
*/
#ifndef SYN_ANALYZER_H_INCLUDED
#define SYN_ANALYZER_H_INCLUDED

#include "lex_analyzer.h"
#include <stack>

using namespace std;

/// \brief Syntactic analyzer
class Syn_analyzer
{
    Lex cur_lex;///<Current lexeme
    type_of_lex t_lex;///<Type of current lexeme
    Lex_analyzer lexical;///<Lexical analyzer
    vector<Lex> polish;///<Vector, where lexemes stores in Polish Notation
    stack<Lex> temp;///<Subsidiary stack

///Recursive descent functions
    void S();
    void A();
    void S1();
    void K();
    void O();
    void F();
    void T();
/*!
Getting next lexeme
\return Next lexeme
*/
    void gl()
    {
        cur_lex = lexical.get_lex();
        t_lex = cur_lex.get_type();
    }
public:
/*!
Constructor of syntactic analyzer class
\param[in] s Regular expression
*/
    Syn_analyzer(const string &s): lexical(s) {}
/*!
Analyzing regular expression.
In case using uncorrect regular expression - exit.
\return Lexemes in Polish Notation
*/
    vector<Lex> &analyze()
    {
        try {
            gl();
            S();
            if(t_lex != LEX_FIN) {
                throw t_lex;
            }
        } catch (type_of_lex t) {
            cerr << "ERROR on lexeme" << endl;
            cout << cur_lex << endl;
            exit(EXIT_FAILURE);
        }
        return polish;
    }
/*!
Print Polish Notation
*/
    void print_polish()
    {
        for(auto it = polish.begin(); it != polish.end(); it++) {
            cout << *it << endl;
        }
    }
};

#endif //SYN_ANALYZER_H_INCLUDED
