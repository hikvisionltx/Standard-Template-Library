#include "../list.h"
#include <random>
#include <iostream>
using namespace std;


int main()
{
    ltx::list<int> l;
    l.push_back(10);
    l.push_back(20);
    l.push_front(0);
    l.erase(l.begin());
    l.erase(l.begin());
    l.push_back(1111);
    for(auto ite = l.begin(); ite!=l.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;
    ltx::list<int> l2;
    l2.push_back(10);
    l2.push_back(20);
    l.splice(++l.begin(), l2);
    
    
    for(auto ite = l.begin(); ite!=l.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;
    l.reverse();
    for(auto ite = l.begin(); ite!=l.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;
    l.remove(20);
    for(auto ite = l.begin(); ite!=l.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    for(int i=0; i<100; ++i)
    {
        l.push_back(rand()%1000);
    }
    for(auto ite = l.begin(); ite!=l.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;
    l.sort();
        for(auto ite = l.begin(); ite!=l.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;


    return 0;
}