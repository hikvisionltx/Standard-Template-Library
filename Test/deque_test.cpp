#include "../deque.h"
#include <iostream>
using namespace std;

int main()
{
    ltx::deque<int> d;
    for(int i=0; i<10; ++i)
    {
        d.push_back(i*i);
    }

    d.insert(d.begin(), 10);

    for(auto ite = d.begin(); ite!=d.end(); ++ite)
    {
        cout << *ite<< endl;
    }
    cout<< d.size() << endl;
    cout << d[3] << endl;
    d.pop_back();
    d.pop_front();
    d.push_back(123);
    d.push_front(456);
    d.erase(d.begin());
    for(auto ite=d.begin(); ite!=d.end(); ++ite)
    {
        if(*ite % 2 == 0) 
        {
            cout << "fuck" << endl;
            d.insert(ite, 233);
            ++ite;
        }

    }
    cout << "pop:" << endl;
    for(auto ite = d.begin(); ite!=d.end(); ++ite)
    {
        cout << *ite<< endl;
    }

    return 0;
}