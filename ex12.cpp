// Chapter 19, exercise 12: File_handle class with a constructor that takes
// a string argument (the file name), opens the file in the constructor and
// closes it in the destructor

#include "std_lib_facilities.h"

class File_handle
{
private:
    string fname;
    fstream fs;
public:
    File_handle(const string &filename);
    ~File_handle();
};

File_handle::File_handle(const string &filename)
: fname{filename}
{
    fs.open(fname);
    if (!fs)
        error("Cannot open a file ", fname);
    cout << "File is open" << endl;
}

File_handle::~File_handle()
{
    fs.close();
    cout << "File is closed" << endl;
}


int main()
try
{
    File_handle fh{"file.txt"};
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
