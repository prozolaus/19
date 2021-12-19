#include "std_lib_facilities.h"

template <typename T>
struct S
{
    S() : val(T()) {}
    S(T t) : val{t} {}
    const T &get() const;
    T &get();
    void set(const T &);
    S<T> &operator=(const T &);

private:
    T val;
};
//------------------------------------------------------

template <typename T>
T &S<T>::get()
{
    return val;
}

template <typename T>
const T &S<T>::get() const
{
    return val;
}

template <typename T>
void S<T>::set(const T &value)
{
    val = value;
}

template <typename T>
S<T> &S<T>::operator=(const T &value)
{
    val = value;
    return *this;
}
//---------------------------------------------------

template <typename T>
istream &operator>>(istream &is, S<T> &s)
{
    T v;
    is >> v;
    if (!is)
        return is;
    s = v;
    return is;
}

template <typename T>
istream &operator>>(istream &is, S<vector<T>> &s)
{
    char ch1, ch2;
    T temp;
    vector<T> v;
    is >> ch1;
    if (!is || ch1 != '{')
    {
        is.clear(ios_base::failbit);
        return is;
    }
    while (is >> temp >> ch2 && ch2 == ',')
        v.push_back(temp);
    if (!is || ch2 != '}')
    {
        v.clear();
        is.clear(ios::failbit);
        return is;
    }
    v.push_back(temp); // ch2=='}', read last value
    s = v;
    return is;
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

template <typename T>
void read_val(S<T> &s)
{
    cin >> s;
    if (!cin)
        error("Wrong input!");
}

template <typename T>
void print_values(const S<T> &v)
{
    cout << v.get() << endl;
}

//--------------------------------------------------

int main()
try
{
    /*
    S<int> s1(30);
    s1.set(40);
    s1.operator=(50);
    s1 = 60;
    s1.get() = 70;
    S<char> s2('a');
    S<double> s3(3.14);
    S<string> s4("hello");
    vector<int> vi{1, 2, 3};
    S<vector<int>> s5(vi);
    for (int i = 0; i < vi.size(); i++)
        cout << s5.get()[i] << endl;
        */
    //S<int> v;
    //S<char> v;
    //S<double> v;
    //S<string> v;
    S<vector<int>> v;
    read_val(v);
    print_values(v);
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
