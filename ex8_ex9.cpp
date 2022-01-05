// Chapter 19, exercise 08: implement an allocator (19.3.6) using malloc() and
// free() (B.10.4); get vector as defined by the end of 19.3 to work for a few
// simple test cases. Hint: look up "placement new" and "explicit call of destructor"

// Exercise 09: re-implement vector::operator=() using an allocator

#include "std_lib_facilities.h"

template <typename T>
struct my_allocator
{
    T *allocate(int n);
    void deallocate(T *p) { free(p); }
    void construct(T *p, const T &v) { new (p) T(v); }
    void destroy(T *p) { p->~T(); }
};

template <typename T>
T *my_allocator<T>::allocate(int n)
{
    T *p = static_cast<T *>(malloc(n * sizeof(T)));
    if (!p)
        error("No memory for allocating");
    return p;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------

template <typename T, typename A = my_allocator<T>>
class my_vector
{
    A alloc;
    int sz;
    T *elem;
    int space;

public:
    my_vector() : sz{0}, elem{nullptr}, space{0} {}
    explicit my_vector(int n);

    my_vector(const my_vector &);            // copy constructor
    my_vector &operator=(const my_vector &); // copy assignment

    my_vector(my_vector &&);            // move constructor
    my_vector &operator=(my_vector &&); // move assignment

    ~my_vector();

    T &operator[](int n) { return elem[n]; }
    const T &operator[](int n) const { return elem[n]; }

    T &at(int n);
    const T &at(int n) const; // checked access

    int size() const { return sz; }
    int capacity() const { return space; }

    void reserve(int newalloc);
    void push_back(const T &val);
    void resize(int newsize, T val = T());
};
//--------------------------------------------------------------------

template <typename T, typename A>
my_vector<T, A>::my_vector(int n)
    : sz{n}, elem{alloc.allocate(n)}, space{n}
{
    for (int i = 0; i < sz; i++)
        alloc.construct(&elem[i], T());
}
//--------------------------------------------------------------------

template <typename T, typename A>
my_vector<T, A>::my_vector(const my_vector &origin)
    : sz{origin.sz}, elem{alloc.allocate(sz)}, space{origin.space}
{
    for (int i = 0; i < sz; i++)
        alloc.construct(&elem[i], origin.elem[i]);
}
//--------------------------------------------------------------------

template <typename T, typename A>
my_vector<T, A> &my_vector<T, A>::operator=(const my_vector &origin)
{
    if (this == &origin) // self-assignment, no work needed
        return *this;

    if (origin.sz <= space) // enough space, no need for new allocation
    {
        for (int i = 0; i < origin.sz; i++)
            alloc.construct(&elem[i], origin.elem[i]);
        sz = origin.sz;
        return *this;
    }

    T *p = alloc.allocate(origin.sz);   // allocate new space
    for (int i = 0; i < origin.sz; i++) // copy new elements
        alloc.construct(&p[i], origin.elem[i]);

    for (int i = 0; i < sz; i++) // destroy old objects
        alloc.destroy(&elem[i]);
    alloc.deallocate(elem); // deallocate old space

    space = sz = origin.sz;
    elem = p;
    return *this;
}
//--------------------------------------------------------------------

template <typename T, typename A>
my_vector<T, A>::~my_vector()
{
    for (int i = 0; i < sz; i++) // destroy objects
        alloc.destroy(&elem[i]);
    alloc.deallocate(elem); // deallocate space
}
//--------------------------------------------------------------------

template <typename T, typename A>
T &my_vector<T, A>::at(int n)
{
    if (n < 0 || n >= sz)
        throw Range_error(n);
    return elem[n];
}
//--------------------------------------------------------------------

template <typename T, typename A>
void my_vector<T, A>::reserve(int newalloc)
{
    if (newalloc <= space)
        return;
    T *p = alloc.allocate(newalloc);
    for (int i = 0; i < sz; i++)
        alloc.construct(&p[i], elem[i]);
    for (int i = 0; i < sz; i++)
        alloc.destroy(&elem[i]);
    alloc.deallocate(elem);
    elem = p;
    space = newalloc;
}
//--------------------------------------------------------------------

template <typename T, typename A>
void my_vector<T, A>::push_back(const T &val)
{
    if (space == 0)
        reserve(8);
    else if (sz == space)
        reserve(2 * sz);
    alloc.construct(&elem[sz], val);
    sz++;
}
//--------------------------------------------------------------------

template <typename T, typename A>
void my_vector<T, A>::resize(int newsize, T val)
{
    if (newsize < 0)
        throw Range_error(newsize);
    reserve(newsize);
    // create
    for (int i = sz; i < newsize; i++)
        alloc.construct(&elem[i], val);
    // destroy
    for (int i = newsize; i < sz; i++)
        alloc.destroy(&elem[i]);
    sz = newsize;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------

// vector diagnostics
template <typename T>
void diag_vec(const my_vector<T> &v, const string &description)
{
    cout << description << "\n";
    cout << "\tsize: " << v.size() << "\n";
    cout << "\tcapacity: " << v.capacity() << "\n\t";
    for (int i = 0; i < v.size(); ++i)
        cout << v[i] << ' ';
    cout << "\n";
}
//--------------------------------------------------------------------

int main()
try
{
    // test integer
    // default constructor
    my_vector<int> v;
    diag_vec(v, "constructed with v");

    // constructor with int argument
    my_vector<int> v1(5);
    diag_vec(v1, "constructed with v1(5)");

    // copy constructor
    for (int i = 0; i < v1.size(); ++i)
        v1[i] = i;
    my_vector<int> v2 = v1;
    diag_vec(v2, "copy constructed from modified v1");

    // copy assignment
    my_vector<int> v3(1); // shorter than v2
    diag_vec(v3, "v3 after initialization");
    v2 = v2; // self-assignment
    v1 = v2; // copy assignment without need to allocate new memory
    v3 = v2; // copy assignment with new allocation required
    diag_vec(v3, "v3 after copy assignment from v2");

    // range checked access, runtime errors
    //cout << v1.at(5) << "\n";
    //v3.resize(-10);

    // growth
    v3.reserve(5); // nothing should happen
    diag_vec(v3, "v3 after reserve(5)");
    v3.reserve(10); // adding space
    diag_vec(v3, "v3 after reserve(10)");
    v3.resize(3);
    diag_vec(v3, "v3 after resize(3)");
    v3.resize(10);
    diag_vec(v3, "v3 after resize(10)");
    v3.push_back(99);
    v3.push_back(100);
    diag_vec(v3, "v3 after two push_back()");

    // test string
    // default constructor
    my_vector<string> vs;
    diag_vec(vs, "constructed with vs");

    // constructor with int argument
    my_vector<string> vs1(5);
    diag_vec(vs1, "constructed with vs1(5)");

    // copy constructor
    vs1[0] = "zero";
    vs1[1] = "one";
    vs1[2] = "two";
    vs1[3] = "three";
    vs1[4] = "four";
    my_vector<string> vs2 = vs1;
    diag_vec(vs2, "copy constructed from modified vs1");

    // copy assignment
    my_vector<string> vs3(1); // shorter than vs2
    diag_vec(vs3, "vs3 after initialization");
    vs2 = vs2; // self-assignment
    vs1 = vs2; // copy assignment without need to allocate new memory
    vs3 = vs2; // copy assignment with new allocation required
    diag_vec(vs3, "vs3 after copy assignment from vs2");

    // growth
    vs3.reserve(5); // nothing should happen
    diag_vec(vs3, "vs3 after reserve(5)");
    vs3.reserve(10); // adding space
    diag_vec(vs3, "vs3 after reserve(10)");
    vs3.resize(3);
    diag_vec(vs3, "vs3 after resize(3)");
    vs3.resize(10);
    diag_vec(vs3, "vs3 after resize(10)");
    vs3.push_back("ninety-nine");
    vs3.push_back("one hundred");
    diag_vec(vs3, "vs3 after two push_back()");
    
    // all destructors are called here
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
