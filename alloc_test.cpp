#include <iostream>
#include <vector>
#include "stl_alloc.h"
using std::cin;
using std::cout;
using std::endl;
int main()
{
    
    // int * ip = (int*)ltx::base_alloc::allocate(sizeof(int));

    int * ip = (int*)::operator new(100);
    *ip = 123456;
    std::cout << *ip << std::endl;
    // ::operator delete(ip);
    ltx::base_alloc::deallocate(ip, sizeof(int));
    // std::vector<int> v(-10);
    int a[100];
    for(int i=0; i<100; ++i) cout << a[i] << " ";
    int x=10;
    int *pp = (int*)ltx::default_alloc::chunk_alloc(sizeof(int), x);
    for(int i=0; i<10; ++i)
    {
        pp[i] = i;
    }
    for(int i=0; i<10; ++i) cout << pp[i] << endl;

    return 0;
}