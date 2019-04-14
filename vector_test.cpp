#include "vector.h"
#include <iostream>

using namespace std;
int main()
{

    ltx::vector<int> v;
    v.push_back(2412);
    cout << v[0] << endl;
    // v.resize(1000);
    for(int i=0; i<10; ++i)
    {
        cout << "-----" << i << endl;
        v.push_back(i);
        
    for(int i=0; i<v.size(); ++i) 
    {
        cout << v[i] << " ";
    }
    cout << endl;
    cout << v.capacity() << endl;
    cout << endl;
    }
    for(int i=0; i<v.size(); ++i) 
    {
        cout << v[i] << " ";
    }
    return 0;
}