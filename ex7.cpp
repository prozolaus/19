// Chapter 19, exercise 07: 
// Apply the solution of ex.2 to several objects of type Number

#include "std_lib_facilities.h"

template <typename T>
class Number
{
    T value;

public:
    Number() : value{T()} {};                    //default constructor
    explicit Number(T i) : value{i} {};          //constructor
    Number(const Number &i) : value{i.value} {}; //copy constructor

    T get() const { return value; }
    void operator=(T i) { value = i; }
    Number &operator=(const Number &i)
    {
        value = i.value;
        return *this;
    } //copy assignment

    istream &operator>>(istream &is)
    {
        is >> value;
        return is;
    }
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
//-----------------------------------------------------------

template <typename T, typename U>
typename std::common_type<T, U>::type f(const vector<T> &vt, const vector<U> &vu)
{
    typename std::common_type<T, U>::type sum = 0;
    int min_size = vt.size() < vu.size() ? vt.size() : vu.size();
    for (int i = 0; i < min_size; i++)
        sum += vt[i] * vu[i];
    return sum;
}

template <typename T, typename U>
typename std::common_type<T, U>::type f(const vector<Number<T>> &vt, const vector<Number<U>> &vu)
{
    typename std::common_type<T, U>::type sum = 0;
    int min_size = vt.size() < vu.size() ? vt.size() : vu.size();
    for (int i = 0; i < min_size; i++)
        sum += vt[i].get() * vu[i].get();
    return sum; 
}

//----------------------------------------------------------

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

//----------------------------------------------------------

int main()
try
{
    Number<int> val1{5}, val2{3}, val3{1};
    Number<double> vald1{5.5}, vald2{3.3}, vald3{1.1};
    vector<Number<int>> v1{val1, val2, val3};
    vector<Number<double>> v2{vald1, vald2, vald3};
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
