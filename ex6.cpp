// Chapter 19, exercise 06: repeat previous exercise with a class Number<T> instead Int
// and try adding operator%, use for Number<double> and Number<int>

#include "std_lib_facilities.h"

template <typename T>
class Number
{
    T value;

public:
    Number() : value{T()} {};    //default constructor
    explicit Number(T i) : value{i} {}; //constructor
    Number(const Number &i) : value{i.value} {};  //copy constructor

    T get() const { return value; }
    void operator=(T i) { value = i; }
    Number &operator=(const Number &i) { value = i.value; return *this; }   //copy assignment

    istream &operator>>(istream &is) { is >> value; return is; }
};

//----------------------------------------------------------

template <typename T>
ostream &operator<<(ostream &os, Number<T> i)
{
    os << i.get();
    return os;
}

template <typename T>
istream &operator>>(istream &is, Number<T> &i)
{
    i.operator>>(is);
    return is;
}

template <typename T>
Number<T> operator+(const Number<T> &v1, const Number<T> &v2)
{
    return Number{v1.get() + v2.get()};
}

template <typename T>
Number<T> operator-(const Number<T> &v1, const Number<T> &v2)
{
    return Number{v1.get() - v2.get()};
}

template <typename T>
Number<T> operator*(const Number<T> &v1, const Number<T> &v2)
{
    return Number{v1.get() * v2.get()};
}

template <typename T>
Number<T> operator/(const Number<T> &v1, const Number<T> &v2)
{
    if (v2.get() == 0)
        error("Division by zero!");
    return Number{v1.get() / v2.get()};
}

template <typename T>
Number<T> operator%(const Number<T> &v1, const Number<T> &v2)
{
    return Number{v1.get() % v2.get()};
}

//-----------------------------------------------------------

int main()
try
{
    Number<double> val{5}, val2{3}, val3;
    val3 = val % val2;  //operator % doesn't compile with double
    //cin >> val;
    cout << val3 << endl;
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
