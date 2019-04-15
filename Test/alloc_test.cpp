#include <iostream>
#include <vector>
#include "../stl_alloc.h"
#include "../memory.h"
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
int main()
{
    
    // vector<int, test::Allocator<int> > v;
    // v.push_back(1);
    std::vector<int, ltx::Allocator<int>> v;
    for(int i=0; i<100; ++i) v.push_back(rand());
    for(int i=0; i<100; ++i) cout << v[i] << endl;
    for(auto ite=v.begin(); ite!=v.end(); )
    {
        if((*ite)%2 == 0) 
        {
            ite = v.erase(ite);
        }
        else ++ite;
    }
    
    cout <<"end" << endl;
    std::sort(v.begin(), v.end());
    for(auto i : v) cout << i << " " ;
    
    // ltx::Allocator<int> alloc;
    // int *p = alloc.allocate(1);
    // cout << "fuK" << endl;
    // int x = 1234;
    // alloc.construct(p, 10);
    // cout << *p << endl;

    // int * ip = (int*)ltx::base_alloc::allocate(siz   eof(int));

    // int * ip = (int*)::operator new(100);
    // *ip = 123456;
    // std::cout << *ip << std::endl;
    // ::operator delete(ip);
    // ltx::base_alloc::deallocate(ip, sizeof(int));
    // std::vector<int> v(-10);
    // int a[100];
    // for(int i=0; i<100; ++i) cout << a[i] << " ";
    // int x=10;
    // cout << "sizeofint" << sizeof(int) << endl;
    // int * ppi = (int*)ltx::default_alloc::allocate(sizeof(int));
    // *ppi = 10;
    // cout << *ppi << endl;
    // int *pp = (int*)ltx::default_alloc::chunk_alloc(sizeof(int), x);
    // cout << (void*)ltx::default_alloc::start_free << " " << (void*)ltx::default_alloc::end_free << endl;
    // cout << "get" << x << endl;
    // int nn = 20;
    // int *ppp = (int*)ltx::default_alloc::refill(sizeof(int));
    
    // cout << (void*)ltx::default_alloc::start_free << " " << (void*)ltx::default_alloc::end_free << endl;
    // for(int i=0; i<10; ++i)
    // {
    //     pp[i] = i;
    // }
    // for(int i=0; i<10; ++i) cout << pp[i] << endl;

    return 0;
}