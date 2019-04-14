#include <type_traits>
#include <iostream>
#include "stl_uninitialized.h"
using namespace std;
class A
{
    public:
    int m;
virtual void f() {}
};
int main()
{
    char s[] = "12345678";
    char s1[1000];
    ltx::uninitialized_copy(s, s+9, s1);
    cout << s1 << endl;
    ltx::uninitialized_fill(s1, s1+100, 'x');
    s1[99] = '\0';
    cout << s1 << endl;
    A a;
    
    cout << is_pod<int>::value << endl;
    cout << is_pod<A>::value << endl;
    cout << is_pod<decltype(*&a)>::value << endl;
    cout << is_pod<A>::value << endl;
    int *p = (int*)::operator new(sizeof(int)*10);
    ltx::uninitialized_fill_n(p, 10, 1234);
    for(int i=0; i<10; ++i) cout << p[i] << endl;
    

    return 0;
}