// Chapter 19, exercise 13: Tracer class where constructor and destructor print
// strings (given as argument to constructor). Use it to see where RAII objects
// will do their job (local objects, member objects, global objects, objects
// allocated with new...), then add copy constructor and copy assignment to see
// when copying is done

#include "std_lib_facilities.h"

class Tracer
{
private:
    string s;

public:
    Tracer(const string &str);
    Tracer(const Tracer &src);
    Tracer &operator=(const Tracer &src);
    ~Tracer();
};

Tracer::Tracer(const string &str)
    : s{str}
{
    cout << this << " constructor: " << s << endl;
}

Tracer::~Tracer()
{
    cout << this << " destructor: " << s << endl;
}

Tracer::Tracer(const Tracer &src)
    : s{src.s}
{
    cout << this << " copy constructor: " << s << endl;
}

Tracer &Tracer::operator=(const Tracer &src)
{
    s = src.s;
    cout << this << " copy assignment: " << s << endl;
    return *this;
}

//----------------------------------------------------------------------

struct Test
{
    Test(const string &str) : tr{str} {}
    Tracer tr;
};

//----------------------------------------------------------------------

Tracer global{"global scope"};

//----------------------------------------------------------------------

int main()
try
{
    {
        Tracer local1{"local scope"};
    }
    Test test("object-member");
    Tracer *ptr = new Tracer{"dynamic memory"};
    //global = test.tr; //copy assignment
    //Tracer tracer{*ptr};  //copy constructor
    delete ptr;
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
