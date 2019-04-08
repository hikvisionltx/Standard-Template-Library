#include "stl_construct.h"
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
class A
{
    
public:
static int c;

    std::string s;
    void f()
    {
        cout << s << endl;
        cout << "f" << endl;
    }
    int id;
    A()
    {
        s = std::string("abc");
        id = c++;
        cout << id << " construct" << endl;
    }
    ~A()
    {
        
        cout << id << " destroy" << endl;
    }
};
int A::c = 0;
int main()
{
    char p[100];
    ltx::construct((int*)(p), 123);
    cout << *((int*)(p)) << endl;
    ltx::destroy((int*)p);
    A a[10];
    ltx::destroy(a, a+10);

    return 0;
}