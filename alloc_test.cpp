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


template <int inst>
class B
{

};

int main()
{
    char p[100];
    ltx::construct((int*)(p), 123);
    cout << *((int*)(p)) << endl;
    ltx::destroy((int*)p);
    A a[10];
    ltx::destroy(a, a+10);
    int b[10];
    std::vector<int>::value_type z;
    // cout << typeid(delz)==typeid(int) << endl;
    z = 123443245;
    cout << z << endl;
    B<4> f();

    return 0;
}