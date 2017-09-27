/*!
\file
\brief Lexical analyzer scanner

The file contains the realization of scanner function of Lexical analyzer class.
This function simulates the operation of the deterministic final state machine.
*/
#include "lex_analyzer.h"
#include <string>
#include <cstdlib>

void Lex_analyzer::scanner()
{
    enum state {H, TERM, ITER, PLUS, REP, LPAR, RPAR, NUM}; /// The deterministic final state machine states
    state CS = H; /// Current state
    int num = 0;
    string terms = ""; /// Current buffer of terminals
    string all_terms = ""; /// Buffer of all possible terminals
    string s2;
    char ch = '!';
    all_terms += ch;
    do {
        all_terms += ++ch;
    } while(ch != '~');
    do {
        switch(CS) {
            case H:
                gc();
                if(c == 0) {
                    if(terms != "") {
                        v.push_back(Lex(LEX_TERM, terms));
                    }
                    v.push_back(Lex(LEX_FIN));
                    return;
                }
                if(all_terms.find(c) ==  string::npos) {
                    throw invalid_argument("Invalid character was used");
                } else {
                    if(c == '|') {
                        if(terms != "") {
                            v.push_back(Lex(LEX_TERM, terms));
                            terms = "";
                        }
                        v.push_back(Lex(LEX_OR));
                    } else if(c == '*') {
                        CS = ITER;
                    } else if(c == '+') {
                        CS = PLUS;
                    } else if(c == '(') {
                        CS = LPAR;
                    } else if(c == ')') {
                        CS = RPAR;
                    } else if(c == '{') {
                        CS = REP;
                    } else if(c == '\\') {
                        gc();
                        terms += c;
                    } else {
                        if(c == '?' || c == '[' || c == ']' || c =='-') {
                            throw invalid_argument("No escaping special characters");
                        } else {
                            terms += c;
                        }
                    }
                }
                break;
            case LPAR:
                if(terms != "") {
                    v.push_back(Lex(LEX_TERM, terms));
                    v.push_back(Lex(LEX_CONCAT));
                    terms = "";
                }
                v.push_back(Lex(LEX_LPAREN));
                CS = H;
                break;
            case RPAR:
                if(terms != "") {
                    v.push_back(Lex(LEX_TERM, terms));
                    terms = "";
                }
                v.push_back(Lex(LEX_RPAREN));
                gc();
                s2 = "){+*|";
                if(c != 0 && s2.find(c) == string::npos) {
                    v.push_back(Lex(LEX_CONCAT));
                }
                i--;
                CS = H;
                break;
            case ITER:
                if(terms == "") {
                    gc();
                    s2 = "){+*|";
                    v.push_back(Lex(LEX_ITER));
                    if(c != 0 && s2.find(c) == string::npos) {
                        v.push_back(Lex(LEX_CONCAT));
                    }
                    i--;
                    CS = H;
                } else {
                    i--;
                    CS = TERM;
                }
                break;
            case PLUS:
                gc();
                if(c != '?') {
                    throw invalid_argument("No escaping special characters");
                } else {
                    if(terms == "") {
                        gc();
                        v.push_back(Lex(LEX_LAZY_COPY));
                        s2 = "(){+*|";
                        if(c != 0 && s2.find(c) == string::npos) {
                            v.push_back(Lex(LEX_CONCAT));
                        }
                        i--;
                        CS = H;
                    } else {
                        i -= 2;
                        CS = TERM;
                    }
                }
                break;
            case TERM:
                if(terms.size() > 1) {
                    string last_ch = terms.substr(terms.size() - 1);
                    terms.pop_back();
                    v.push_back(Lex(LEX_TERM, terms));
                    v.push_back(Lex(LEX_CONCAT));
                    v.push_back(Lex(LEX_TERM, last_ch));
                } else {
                    gc();
                    s2 = "){+*|";
                    v.push_back(Lex(LEX_TERM, terms));
                    if(c != 0 && s2.find(c) == string::npos) {
                        v.push_back(Lex(LEX_CONCAT));
                    }
                    i--;
                }
                terms = "";
                CS = H;
                break;
            case REP:
                if(terms != "") {
                    i--;
                    CS = TERM;
                    break;
                }
                gc();
                if(c != ',') {
                    throw invalid_argument("No escaping special characters");
                } else {
                    CS = NUM;
                }
                break;
            case NUM:
                gc();
                if(isdigit(c)) {
                    num *= 10;
                    num += c - '0';
                } else {
                    if(c == '}') {
                        gc();
                        s2 = "(){+*|";
                        v.push_back(Lex(LEX_REPEAT, "", num));
                        num = 0;
                        if(c != 0 && s2.find(c) == string::npos) {
                            v.push_back(Lex(LEX_CONCAT));
                        }
                        i--;
                        CS = H;
                    } else {
                        throw invalid_argument("No escaping special characters");
                    }
                }
                break;
        }
    } while(true);
}
