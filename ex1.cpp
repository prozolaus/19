// Chapter 19, exercise 01: 
// write a template function that adds elements of one vector<T> 
// to the elements of another: f(v1,v2) should do v1[i]+=v2[i] for each element of v1

#include "std_lib_facilities.h"

template <typename T>
void f(vector<T> &v1, const vector<T> &v2)
{
    for (int i = 0; i < v1.size(); i++)
    {
        if (v2.size() == i)
            break;
        v1[i] += v2[i];
    }
}

void f(vector<char> &v1, const vector<char> &v2)
{
    error("Cannot add characters!");
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
    cout << v1 << endl << v2 << endl;
    f(v1, v2);
    cout << "After:\n" << v1 << endl;
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
