/*!
\file
\brief Checking functions

The file contains the realization of checking functions
*/
#include "fit_to_reg.h"

bool Fit_to_reg::fit()
{
    fl_iter = 0;
    fl_rep = 0;
    fl_copy = 0;
    fl_or = 0;
    it_or = s_or.begin();
    it = v.begin();
    i = 0;
    int fl_tr = 0;
    while(it < v.end()) {
        if(S() != true) {
            if(i != s.size() && (fl_iter || fl_rep || fl_copy || fl_or)) {
                break;
            }
            return false;
        } else if(fl_srch) {
            fl_tr = i;
        }
    }
    if(i != s.size() && fl_srch == 0) {
        if(fl_iter) {
            fl1 = 1;
            return fit();
        } else if(fl_rep){
            fl2 = 1;
            return fit();
        } else if(fl_copy){
            fl3 = 1;
            return fit();
        } else if(fl_or) {
            fl4 = 1;
            return fit();
        } else {
            return false;
        }
    }
    if(fl_tr) {
        check(true);
    }
    if(i != s.size() && fl_srch) {
        if(fl_copy) {
            if(i != 0) {
                return check(true);
            }
            fl3 = 1;
            return check(fit());
        } else if(fl_rep){
            if(i != 0) {
                return check(true);
            }
            fl2 = 1;
            return check(fit());
        } else if(fl_iter){
            if(i != 0) {
                return check(true);
            }
            fl1 = 1;
            return check(fit());
        } else if(fl_or) {
            if(i != 0) {
                return check(true);
            }
            fl4 = 1;
            return check(fit());
        } else {
            if(i != 0) {
                return check(true);
            }
            return false;
        }
    }
    if(fl_srch) {
        return check(true);
    }
    return true;
}

bool Fit_to_reg::S()
{
    if(it->get_type() == LEX_TERM) {
        return T();
    } else if(it->get_type() == LEX_OR) {
        return O();
    } else if(it->get_type() == LEX_CONCAT) {
        return C();
    } else if(it->get_type() == LEX_ITER) {
        return I();
    } else if(it->get_type() == LEX_REPEAT) {
        return R();
    } else if (it->get_type() == LEX_LAZY_COPY) {
        return L();
    }
    return false;
}

bool Fit_to_reg::T()
{
    string term = it->get_info();
    int f = 0;
    unsigned cur_i = i;
    for(unsigned j = 0; j < term.size(); j++) {
        if(cur_i == s.size()) {
            f = 1;
            break;
        }
        if(s[cur_i++] != term[j]) {
            f = 1;
            break;
        }
    }
    it++;
    if(f == 1) {
        return false;
    }
    i = cur_i;
    return true;
}

bool Fit_to_reg::O()
{
    auto k = it - v.begin();
    it++;
    unsigned cur_i = i;
    unsigned j;
    if(fl4 != 0) {
        if(k == *it_or) {
            S();
            i = cur_i;
            it_or++;
            S();
            if(it_or == s_or.end()) {
                if(--it_or != s_or.begin()) {
                    fl_or = 1;
                }
                s_or.erase(it_or);
            }
            return true;
        }
    }
    if(S()) {
        j = i;
        i = cur_i;
        if(S()){
            fl_or = 1;
            s_or.insert(k);
            //cout << k;
        }
        i = j;
        return true;
    } else {
        i = cur_i;
        if(S()){
            return true;
        }
    }
    i = cur_i;
    return false;
}

bool Fit_to_reg::C()
{
    it++;
    unsigned cur_i = i;
    if(S()) {
        if(S()) {
            return true;
        }
        i = cur_i;
        return false;
    }
    S();
    i = cur_i;
    return false;
}

bool Fit_to_reg::I()
{
    it++;
    auto cur_it = it;
    if(fl1 == 0) {
        fl_iter = 1;
        int count  = 0;
        while(S()) {
            count++;
            it = cur_it;
        }
        v_iter.push_back(count);
    } else {
        if(fl_iter == 0) {
            it_iter = v_iter.begin();
        } else {
            it_iter++;
        }
        fl_iter = 1;
        int count = *it_iter;
        unsigned cur_i = i;
        if(count == 0) {
            S();
            i = cur_i;
            fl_iter = 0;
            return false;
        }
        while(S() && --count > 0) {
            it = cur_it;
            cur_i = i;
        }
        i = cur_i;
        *it_iter--;
    }
    return true;
}

bool Fit_to_reg::R()
{
    if(fl2 == 0) {
        int k = it->get_m();
        it++;
        auto cur_it = it;
        fl_rep = 1;
        int count = 1;
        unsigned cur_i = i;
        if(k == 0) {
            S();
            i = cur_i;
            v_rep.push_back(k);
            return true;
        }
        while(S() && --k > 0) {
            count++;
            it = cur_it;
            cur_i = i;
        }
        v_rep.push_back(count);
    } else {
        it++;
        auto cur_it = it;
        if(fl_rep == 0) {
            it_rep = v_rep.begin();
        } else {
            it_rep++;
        }
        fl_rep = 1;
        int count = *it_rep;
        unsigned cur_i = i;
        if(count == 0) {
            S();
            i = cur_i;
            fl_rep = 0;
            return false;
        }
        while(S() && --count > 0) {
            it = cur_it;
            cur_i = i;
        }
        i = cur_i;
        *it_rep--;
    }
    return true;
}

bool Fit_to_reg::L()
{
    it++;
    auto cur_it = it;
    if(fl3 == 0) {
        unsigned cur_i = i;
        if(S() != true) {
            i = cur_i;
            return false;
        } else {
            fl_copy = 1;
            v_copy.push_back(1);
        }
    } else {
        if(fl_copy == 0) {
            it_copy = v_copy.begin();
        } else {
            it_copy++;
        }
        fl_copy = 1;
        int count = *it_copy;
        count++;
        unsigned cur_i = i;
        while(S() && --count > 0) {
            it = cur_it;
            cur_i = i;
        }
        if(count != 0) {
            i = cur_i;
            fl_copy = 0;
            return false;
        } else {
            *it_copy++;
        }
    }
    return true;
}
