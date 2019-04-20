#include <iostream>
#include <algorithm>
#include "../rb_tree.h"
#include <functional>

using namespace std;

struct A
{
    int i;
    double d;
};
struct B
{
    A* a;
    A& operator* () { return *a; }
    A* operator->() { return &(operator*()); }
};



int main()
{
    ltx::__rb_tree_base_iterator ite;
    A a;
    a.i = 10;
    a.d = 1.234;
    B b;
    b.a = &a;
    cout << (*b).d << endl;
    cout << b->d << endl;
    
    

    ltx::rb_tree<int, int, _Identity<int>, less<int>> itree;
    itree.insert_unique(10);
    itree.insert_unique(234);
    itree.insert_unique(234);
    itree.insert_unique(3);
    itree.insert_unique(140);
    itree.insert_unique(101);
    itree.insert_equal(100);
    itree.insert_equal(100);
    itree.insert_equal(100);
    itree.insert_equal(100);
    
    for(auto ite = itree.begin(); ite!=itree.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;
    cout << boolalpha <<  (itree.find(1234) != itree.end()) << endl;
    auto range = itree.equal_range(100);
    for(auto ite = range.first; ite!=range.second; ++ite)
    {
        cout << *ite << " ";
    }


    return 0;
}