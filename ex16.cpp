// Chapter 19, exercise 16: define a vector so that sizeof(vector<int>) is
// ==sizeof(int*), i.e., so that the vector itself consists only of a pointer to
// a representation consisting of the elements, the number of elements and the
// space pointer

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
public:
    A alloc;
    int sz;
    T *elem;
    int space;

    my_vector() : sz{0}, elem{nullptr}, space{0} {}
    explicit my_vector(int n);
    my_vector(initializer_list<T> lst);

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
my_vector<T, A>::my_vector(initializer_list<T> lst)
    : sz{(int)lst.size()}, elem{alloc.allocate(sz)}, space{sz}
{
    T *ptr = elem;
    for (const T &p : lst)
        alloc.construct(ptr++, p);
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
const T &my_vector<T, A>::at(int n) const
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

template <typename T>
class my_mini_vector
{
    my_vector<T> *ptr;

public:
    my_mini_vector() : ptr{new my_vector<T>()} {}
    explicit my_mini_vector(int n) : ptr{new my_vector<T>(n)} {}
    my_mini_vector(initializer_list<T> lst) : ptr{new my_vector<T>(lst)} {}

    my_mini_vector(const my_mini_vector &);            // copy constructor
    my_mini_vector &operator=(const my_mini_vector &); // copy assignment

    my_mini_vector(my_mini_vector &&);            // move constructor
    my_mini_vector &operator=(my_mini_vector &&); // move assignment

    ~my_mini_vector() { delete ptr; }

    T &operator[](int n) { return (*ptr).operator[](n); }
    const T &operator[](int n) const { return (*ptr).operator[](n); }

    T &at(int n) { return (*ptr).at(n); }
    const T &at(int n) const { return (*ptr).at(n); } // checked access

    int size() const { return (*ptr).size(); }
    int capacity() const { return (*ptr).capacity(); }

    void reserve(int newalloc) { (*ptr).reserve(newalloc); }
    void push_back(const T &val) { (*ptr).push_back(val); }
    void resize(int newsize, T val = T()) { (*ptr).resize(newsize, val); }
};

template <typename T>
my_mini_vector<T>::my_mini_vector(const my_mini_vector &origin)
    : ptr{new my_vector<T>(origin.ptr->sz)}
{
    for (int i = 0; i < origin.ptr->sz; i++)
        ptr->elem[i] = origin.ptr->elem[i];
    ptr->space = origin.ptr->space;
}

template <typename T>
my_mini_vector<T> &my_mini_vector<T>::operator=(const my_mini_vector &origin)
{
    if (this == &origin) // self-assignment, no work needed
        return *this;

    if (origin.ptr->sz <= ptr->space) // enough space, no need for new allocation
    {
        for (int i = 0; i < origin.ptr->sz; i++)
            ptr->elem[i] = origin.ptr->elem[i];
        ptr->sz = origin.ptr->sz;
        return *this;
    }

    my_vector<T> *p = new my_vector<T>(origin.ptr->sz);
    for (int i = 0; i < origin.ptr->sz; i++)
        p->elem[i] = origin.ptr->elem[i];
    delete ptr;
    ptr = p;
    return *this;
}

template <typename T>
my_mini_vector<T>::my_mini_vector(my_mini_vector &&origin)
    : ptr{origin.ptr}
{
    origin.ptr = nullptr;
}

template <typename T>
my_mini_vector<T> &my_mini_vector<T>::operator=(my_mini_vector &&origin)
{
    ptr = origin.ptr;
    origin.ptr = nullptr;
    return *this;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------

// vector diagnostics
template <typename T>
void diag_vec(const my_mini_vector<T> &v, const string &description = "")
{
    cout << description << "\n";
    cout << "\tsize: " << v.size() << "\n";
    cout << "\tcapacity: " << v.capacity() << "\n\t";
    for (int i = 0; i < v.size(); ++i)
        cout << v[i] << ' ';
    cout << "\n";
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------

int main()
try
{
    int *ptr;
    my_mini_vector<int> vi{1, 2, 3, 4, 5};
    my_mini_vector<int> vi2{vi};
    my_mini_vector<int> vi3;
    my_mini_vector<int> vi4(30);
    my_mini_vector<my_mini_vector<my_mini_vector<int>>> vv;

    vi3 = vi;
    vi[0] = 0;
    vi.push_back(6);
    vi2.at(1) = 0;
    vi2.reserve(20);
    vi3[2] = 0;
    vi3.resize(2);
    diag_vec(vi);
    diag_vec(vi2);
    diag_vec(vi3);
    diag_vec(vi4);
    cout << "size of mini vector: " << sizeof(vi) << endl;
    cout << "size of mini vector2: " << sizeof(vi2) << endl;
    cout << "size of mini vector3: " << sizeof(vi3) << endl;
    cout << "size of mini vector4: " << sizeof(vi4) << endl;
    cout << "size of mini vector vv: " << sizeof(vv) << endl;
    cout << "size of int*: " << sizeof(ptr) << endl;
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
