/*!
\file
\brief The header with description of lexical analyzer class

The file contains the description of lexical analyzer class, which
tokenizes regular expression and controls using bad symbols or same
diffrent basic errors
*/
#ifndef LEX_ANALYZER_H_INCLUDED
#define LEX_ANALYZER_H_INCLUDED

#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

/// Enumeration of diffrent types of lexems
enum type_of_lex
{
    LEX_NULL, ///<NULL lexem
    LEX_TERM, ///<Terminals
    LEX_OR, ///<Lexeme '|'
    LEX_ITER, ///<Lexeme '*'
    LEX_LPAREN, ///<Lexeme '('
    LEX_RPAREN, ///<Lexeme ')'
    LEX_REPEAT, ///<Lexeme '{,m}'
    LEX_LAZY_COPY, ///<Lexeme '+?'
    LEX_CONCAT, ///<Lexeme of concatenation
    LEX_FIN ///<Final lexeme
};

/// \brief Lexeme with all information
class Lex
{
    type_of_lex t_lex;///<Type of lexeme
    string info_lex;///<Lexeme's data (only for terminals)
    int m;///<Count of repeatition (only for lexeme '{,m}')
public:
/*!
Constructor of class of lexemes
\param[in] t Type of lexeme
\param[in] info Lexeme's data (for no terminals = "")
\param[in] k Count of repeatition (for no repeatition = 0)
*/
    Lex(type_of_lex t = LEX_NULL, string info = "", int k = 0): t_lex(t), info_lex(info), m(k) {}
/*!
Getting type of lexeme
\return Type of lexeme
*/
    type_of_lex get_type() const
    {
        return t_lex;
    }
/*!
Getting string with terminals
\return String with terminals
*/
    string get_info() const
    {
        return info_lex;
    }
/*!
Getting count of repeatition
\return Count of repeatition
*/
    int get_m() const
    {
        return m;
    }
/*!
Print lexeme to stream
\return Link to stream
*/
    friend ostream& operator<<(ostream& os, const Lex& lx)
    {
        switch(lx.get_type()) {
            case 0:
                os << "LEX_NULL";
                break;
            case 1:
                os << "LEX_TERM" << ' ' << lx.get_info();
                break;
            case 2:
                os << "LEX_OR";
                break;
            case 3:
                os << "LEX_ITER";
                break;
            case 4:
                os << "LEX_LPAREN";
                break;
            case 5:
                os << "LEX_RPAREN";
                break;
            case 6:
                os << "LEX_REPEAT";
                break;
            case 7:
                os << "LEX_LAZY_COPY";
                break;
            case 8:
                os << "LEX_CONCAT";
                break;
            case 9:
                os << "LEX_FIN";
                break;
        }
        return os;
    }
};

/// \brief Lexical analyzer
class Lex_analyzer
{
    vector<Lex> v;///<Vector with all lexemes
    vector<Lex>::iterator it;///<Iterator to vector with all lexemes
    char c;///<Current symbol in regular expression
    string buf;///<Regular expression
    unsigned i = 0;///<Position to current symbol in regular expression
/*!
Getting next symbol
\return Next symbol
*/
    void gc()
    {
        if(i >= buf.size()) {
            c = 0;
            i++;
        } else {
            c = buf[i++];
        }
    }
/*!
Fillng vector with all lexemes
\throw invalid_argument In case of using bad symbols or same other errors
*/
    void scanner();
public:
/*!
Constructor of lexical analyzer class
\param[in] s Regular expression (default = "")
*/
    Lex_analyzer(const string &s = ""): buf(s)
    {
        try{
            scanner();
        } catch(invalid_argument &ia) {
            cerr << "Bad regular expression: " << ia.what() << endl;
            exit(EXIT_FAILURE);
        }
        it = v.begin();
    }
/*!
Getting next lexeme
\return Next lexeme
*/
    Lex get_lex()
    {
        return *(it++);
    }
};

#endif //LEX_ANALYZER_H_INCLUDED
