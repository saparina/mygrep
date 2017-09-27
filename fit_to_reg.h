/*!
\file
\brief The header with description of Fit_to_reg class

The file contains the description of Fit_to_reg class, which
selects the appropriate line and searches, if it is mode 2
*/
#ifndef FIT_TO_REG_H_INCLUDED
#define FIT_TO_REG_H_INCLUDED

#include <vector>
#include <iterator>
#include <set>
#include "lex_analyzer.h"

/// \brief Fitting in strings and searching first fitting substring
class Fit_to_reg
{
    vector<Lex> v;///<Regular expression
    vector<Lex>::iterator it;///<Iterator to current lexeme on regular expression
    vector<int> v_iter;///<Vector that stores positions and states of all lexemes '*'
    vector<int>::iterator it_iter;///<Iterator to vector with infornation of of all lexemes '*'
    vector<int> v_rep;///<Vector that stores positions and states of all lexemes '{,m}'
    vector<int>::iterator it_rep;///<Iterator to vector with infornation of of all lexemes '{,m}'
    vector<int> v_copy;///<Vector that stores positions and states of all lexemes '+?'
    vector<int>::iterator it_copy;///<Iterator to vector with infornation of of all lexemes '+?'
    set<int> s_or;///<Vector that stores positions and states of all lexemes '|'
    set<int>::iterator it_or;///<Iterator to vector with infornation of of all lexemes '|'
    string s;///<Current string (or substring for search)
    unsigned max_ans = 0;///<Maximum lenght of fitting substring (only for search mode)
    unsigned i = 0;///<Current position in string
    int fl1 = 0;///<'*' must be reprocessed
    int fl2 = 0;///<'{,m}' must be reprocessed
    int fl3 = 0;///<'+?' must be reprocessed
    int fl4 = 0;///<'|' must be reprocessed
    int fl_iter = 0;///<Expression have some '*', thet can be reprocessed
    int fl_rep = 0;///<Expression have some '{,m}', thet can be reprocessed
    int fl_copy = 0;///<Expression have some '+?', thet can be reprocessed
    int fl_srch = 0;///<FLag = 1, if it isearching mode
    int fl_or = 0;///<Expression have some '|', thet can be reprocessed
    
///Checking functions
    bool S();
    bool T();
    bool O();
    bool C();
    bool I();
    bool R();
    bool L();
/*!
Checking function fit() return value in mode search
\param[in] v Return value
\return Function fit() return value
*/
    bool check(bool v)
    {
        if(v == true && i > max_ans) {
            max_ans = i;
        }
        return v;
    }
public:
/*!
Constructor of Fit_to_reg class
\param[in] vv Regular expression
\param[in] ss String that must be processed
\param[in] f Is it a searching mode
*/
    Fit_to_reg(vector<Lex> &vv, string ss, int f): v(vv), s(ss), fl_srch(f)
    {
        it = v.begin();
    }
/*!
Checking string
\return It shall determine whether this string can be regular expression
*/
    bool fit();
/*!
Search first fitting substring
\return First fitting substring
*/
    string search()
    {
        string all_s = s;
        for(unsigned j = 0; j < all_s.size(); j++) {
            fl1 = 0;
            fl2 = 0;
            fl3 = 0;
            fl4 = 0;
            max_ans = 0;
            v_iter.clear();
            v_rep.clear();
            v_copy.clear();
            s_or.clear();
            s = all_s.substr(j);
            //cout << s << endl;
            if(fit()) {
                return all_s.substr(j, i);
            } else {
                if(max_ans) {
                    return all_s.substr(j, max_ans);
                }
            }
        }
        return "";
    }
};

#endif //FIT_TO_REG_H_INCLUDED
