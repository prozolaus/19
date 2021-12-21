// Chapter 19, exercise 02: 
// write a template function that takes a vector<T> vt and a vector<U> vu as arguments
// and returns the sum of all vt[i]*vu[i]s

// Requires the C++11 std::common_type facilities
// to determine the return type

#include "std_lib_facilities.h"

template <typename T, typename U>
typename std::common_type<T, U>::type f(const vector<T> &vt, const vector<U> &vu)
{
    typename std::common_type<T, U>::type sum = 0;
    int min_size = vt.size() < vu.size() ? vt.size() : vu.size();
    for (int i = 0; i < min_size; i++)
        sum += vt[i] * vu[i];
    return sum;
}

char f(const vector<char> &vt, const vector<char> &vu)
{
    error("Cannot multiply characters!");
    return 0;   //for compiler
}

string f(const vector<string> &vt, const vector<string> &vu)
{
    error("Cannot multiply strings!");
    return "";  //for compiler
}

template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v)
{
    os << "{";
    for (int i = 0; i < v.size(); i++)
    {
        os << v[i];
        if (i < v.size() - 1)
            os << ", ";
    }
    os << "}";
    return os;
}

int main()
try
{
    vector<int> v1{1, 2, 3}, v2{4, 5, 6, 7};
    //vector<char> v1{'a', 'b', 'c'}, v2{'d', 'e', 'f'};
    //vector<double> v1{1.1, 2.2, 3.3}, v2{4.4, 5.5, 6.6};
    //vector<string> v1{"Aaa", "Bbb", "Ccc"}, v2{"Ddd", "Eee", "Fff", "Ggg"};
    cout << v1 << endl
         << v2 << endl;
    cout << "After:\n"
         << f(v1, v2) << endl;
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
