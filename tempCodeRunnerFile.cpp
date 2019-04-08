#include "stl_construct.h"
#include <iostream>
using std::cout;
using std::endl;
int main()
{
    char p[100];
    ltx::construct((int*)(p), 123);
    cout << *((int*)(p)) << endl;
    ltx::destroy((int*)p);
    return 0;
}