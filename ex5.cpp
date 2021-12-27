// Chapter 19, exercise 05: class Int with single member - int
// with constructors, assignment, operators +,-,*,/, << and >> for convenient input/output

#include "std_lib_facilities.h"

class Int
{
    int value;

public:
    Int() : value{0} {};    //default constructor
    explicit Int(int i) : value{i} {};
    Int(const Int &i) : value{i.value} {};  //copy constructor

    int get() const { return value; }
    void operator=(int i) { value = i; }
    Int &operator=(const Int &i) { value = i.get(); return *this; }   //copy assignment

    istream &operator>>(istream &is) { is >> value; return is; }
};

//----------------------------------------------------------

ostream &operator<<(ostream &os, Int i)
{
    os << i.get();
    return os;
}

istream &operator>>(istream &is, Int &i)
{
    i.operator>>(is);
    return is;
}

Int operator+(const Int &v1, const Int &v2)
{
    return Int{v1.get() + v2.get()};
}

Int operator-(const Int &v1, const Int &v2)
{
    return Int{v1.get() - v2.get()};
}

Int operator*(const Int &v1, const Int &v2)
{
    return Int{v1.get() * v2.get()};
}

Int operator/(const Int &v1, const Int &v2)
{
    if (v2.get() == 0)
        error("Division by zero!");
    return Int{v1.get() / v2.get()};
}

//-----------------------------------------------------------

int main()
try
{
    Int val{4}, val2{3}, val3;
    val3 = val * val2;
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
