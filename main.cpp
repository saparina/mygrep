/*!
\file
\brief Main function

This file contains the main function, which can be operated in 2 modes
*/

#include "syn_analyzer.h"
#include "fit_to_reg.h"

using namespace std;

/*!
Main function
\param[in] argc Count of arguments
\param[in] argv Pointer to arguments
\return if it exit without errors
*/
int main(int argc, char **argv)
{
    if(argc < 2) {
        cerr << "Too few arguments" << endl;
        exit(EXIT_FAILURE);
    }
    string reg(argv[1]), s;
    int f_search = 0;
    if(argc == 3) {
        f_search = 1; //mode search flag
    }
    Syn_analyzer syn(reg);
    vector<Lex> v = syn.analyze(); //analyze regular expression
    while(cin >> s) {
        Fit_to_reg f(v, s, f_search);
        if(f_search == 0 ) {
            if(f.fit()) {
                cout << s << endl;
            } else {
                cout << endl;
            }
        } else {
            cout << f.search() << endl;
        }
    }
    return 0;
}
