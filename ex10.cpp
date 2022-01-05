// Chapter 19, exercise 10: implement a simple my_unique_ptr supporting only a
// constructor, destructor, ->, * and release() - don't try assignment or copy
// constructor

#include "std_lib_facilities.h"

template <class T>
class my_unique_ptr
{
private:
    T *p;

public:
    my_unique_ptr(T *ptr) : p{ptr} {}

    my_unique_ptr(const my_unique_ptr &obj) = delete;
    my_unique_ptr &operator=(const my_unique_ptr &obj) = delete;

    T *operator->() { return p; }
    T& operator*() { return *p; }

    ~my_unique_ptr() { delete p; }
    T *release();
};

template <class T>
T *my_unique_ptr<T>::release()
{
    T *temp = p;
    p = nullptr;
    return temp;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// for testing
vector<int> *make_vec()
{
    const int n = 10;
    my_unique_ptr<vector<int>> p{new vector<int>};
    for (int i = 0; i < n; i++)
        (*p).push_back(i);
    p->push_back(n);
    return p.release();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// for testing
struct Tracer
{
    Tracer() { cout << "Tracer()" << endl; }
    Tracer(int n) : val(n) { cout << "Tracer(int n)" << endl; }
    ~Tracer() { cout << "~Tracer()" << endl; }
    int val;
};

//------------------------------------------------------------------------------

// demonstrates how only destructor of object held by my_unique_ptr is called
Tracer *f()
{
    Tracer *p1 = new Tracer();               // destructor never called
    my_unique_ptr<Tracer> p2(new Tracer());  // destructor called when out of scope
    my_unique_ptr<Tracer> p3(new Tracer(5)); // released from my_unique_ptr
    return p3.release();
}

//------------------------------------------------------------------------------

// shows how my_unique_ptr destroys its object when an exception is thrown
void f2()
{
    Tracer *p1 = new Tracer();
    my_unique_ptr<Tracer> p2(new Tracer());
    error("Exception!"); // calls destructor of p2
    delete p1;           // never gets called
}

//------------------------------------------------------------------------------


int main()
try
{
    Tracer *p = f();
    cout << "Dereference: " << (*p).val << endl;
    cout << "Pointer access: " << p->val << endl;
    delete p;
    
    f2();
    

    /*
    auto p = make_vec();
    for (int i = 0; i < p->size(); i++)
        cout << p->at(i) << " ";
    cout << endl;
    delete p;
    */
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
