#include "std_lib_facilities.h"

template <typename T1, typename T2>
struct Pair
{
    T1 t1;
    T2 t2;
    Pair(T1 member1, T2 member2) : t1{member1}, t2{member2} {}
};

class Symbol_table
{
    vector<Pair<string, double>> var_table; // it stores values of variables
    
public:
    double get(string s);
    void set(string s, double d);
    bool is_declared(string var);
    double define_name(string var, double val);
};

double Symbol_table::get(string s)
//returns the value of a variable named "s"
{
    for (const Pair<string, double> &v : var_table)
        if (v.t1 == s)
            return v.t2;
    error("get: undefined variable ", s);
    return 0;
}

void Symbol_table::set(string s, double d)
// assigns a value 'd' to an 's' object of type Pair<string, double>
{
    for (Pair<string, double> &v : var_table)
        if (v.t1 == s)
        {
            v.t2 = d;
            return;
        }
    error("set: undefined variable ", s);
}

//-------------------------------------------------------------------------------

bool Symbol_table::is_declared(string var)
//Is there a variable 'var' in the var_table vector?
{
    for (const Pair<string, double> &v : var_table)
        if (v.t1 == var)
            return true;
    return false;
}

double Symbol_table::define_name(string var, double val)
// add a couple (var, val) to the vector var_table
{
    if (is_declared(var))
        set(var, val);
    else
        var_table.push_back(Pair<string, double>(var, val));
    return val;
}

int main()
try
{
    Symbol_table st;
    st.define_name("pi", 3.1415926535);
    double d = st.get("pi");
    cout << d << endl;
}
catch (const std::exception &e)
{
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch (...)
{
    cerr << "Some kind of exception\n";
    return 2;
}
