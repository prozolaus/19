// Chapter 19, exercise 11: design and implement a counted_ptr<T>: a type that
// holds a pointer to an object of type T and a pointer to a "user count" (an
// int) shared by all pointers to the same object of type T, holding the number
// of counted pointers pointing to a given T. The constructor should allocate a
// T and a use count on the heap and take an argument as initial value for the T
// elements. When the last counted_ptr for a T is destroyed, the destructor
// should delete the T.
// counted_ptr should have operations that allow us to use it as a pointer.
// Write a set of test cases.

#include "std_lib_facilities.h"

template <class T>
class counted_ptr
{
private:
    T *p;
    int *counter;

public:
    counted_ptr(const T &val) : p{new T{val}}, counter{new int{1}} { cout << "constructor: *counter = " << *counter << endl; }
    counted_ptr(const counted_ptr &src);            // copy constructor
    counted_ptr &operator=(const counted_ptr &src); // copy assignment
    ~counted_ptr();

    T *operator->() { return p; }
    T &operator*() { return *p; }
    int count() { return *counter; }
};

template <class T>
counted_ptr<T>::counted_ptr(const counted_ptr &src)
{
    p = src.p;
    counter = src.counter;
    (*counter)++;
    cout << "copy constructor: *counter = " << *counter << endl;
}

template <class T>
counted_ptr<T> &counted_ptr<T>::operator=(const counted_ptr &src)
{
    if (&src == this)   //self-assignment
        return *this;
    (*counter)--;
    p = src.p;
    counter = src.counter;
    (*counter)++;
    cout << "copy assignment: *counter = " << *counter << endl;
    return *this;
}

template <class T>
counted_ptr<T>::~counted_ptr()
{
    (*counter)--;
    if (*counter == 0)
    {
        delete p;
        delete counter;
        cout << "destructor: object data was deleted" << endl;
        return;
    }
    cout << "destructor: *counter = " << *counter << endl;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// test functions

void f1()
{
    const double pi = M_PI;
    counted_ptr<double> ptr{pi};   //calls constructor
    counted_ptr<double> ptr2{ptr}; //calls copy constructor
    counted_ptr<double> ptr3{ptr2};
    cout << *ptr3 << endl;
}

counted_ptr<int> f2(int i)
{
    counted_ptr<int> ptr1{i};
    counted_ptr<int> ptr2{ptr1};
    counted_ptr<int> ptr3{ptr2};
    return ptr3;
}

counted_ptr<vector<string>> f3()
{
    vector<string> vs{"When", " the", " smoke", " is", " going", " down\n"};
    counted_ptr<vector<string>> ptr1{vs};
    counted_ptr<vector<string>> ptr2{ptr1};
    return ptr2;
}

void f4()
{
    counted_ptr<string> ptr1{"Hello"};
    counted_ptr<string> ptr11{ptr1};
    counted_ptr<string> ptr2{"World"};
    cout << *ptr1 << ", " << *ptr2 << endl;
    ptr1 = ptr2;
    cout << *ptr1 << ", " << *ptr2 << endl;
    cout << "ptr1.count: " << ptr1.count() << endl;
    cout << "ptr11.count: " << ptr11.count() << endl;
    cout << "ptr2.count: " << ptr2.count() << endl;
}

//------------------------------------------------------------------------------

template <class T>
void fc3(counted_ptr<T> &cp2)
{
    counted_ptr<T> cp3{0};
    cp3 = cp2;
    cout << "*cp3: " << *cp3 << endl;
}

template <class T>
void fc2(counted_ptr<T> &cp1)
{
    counted_ptr<T> cp2{cp1};
    cout << "*cp2: " << *cp2 << endl;
    fc3(cp2);
}

void fc1()
{
    double d = 100;
    counted_ptr<double> cp1{d};
    cout << "*cp1: " << *cp1 << endl;
    fc2(cp1);
    cout << "cp1.counter: " << cp1.count() << endl;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

int main()
try
{
    //f1();

    /*
    counted_ptr<int> ptri = f2(100);
    cout << *ptri << endl;
    *ptri += 100;
    cout << *ptri << endl;
    */

    /*
    counted_ptr<vector<string>> ptr = f3();
    ptr->push_back("Scorpions\n");
    for (const string &s : *ptr)
        cout << s;
    */

    //f4();

    //fc1(); // test counted_ptr with destruction after scope ends

    // test counted_ptr as container element
    vector<counted_ptr<string>> vcp;
    counted_ptr<string> ptr{"word"};
    for (int i = 0; i < 3; i++)
        vcp.push_back(ptr);
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
