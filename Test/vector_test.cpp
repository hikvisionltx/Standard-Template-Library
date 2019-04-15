#include "../vector.h"
#include <iostream>
#include <algorithm>
using namespace std;
int main()
{

    ltx::vector<int> v;
    v.push_back(2412);
    cout << v[0] << endl;
    // v.resize(1000);
    for(int i=0; i<10; ++i)
    {
        v.push_back(i);

    }
    for(int i=0; i<v.size(); ++i) 
    {
        cout << v[i] << " ";
    }
    cout << endl;
    v.insert(v.begin()+3, 3, 1234);
    
    for(int i=0; i<v.size(); ++i) 
    {
        cout << v[i] << " ";
    }
    cout << endl;
    
    sort(v.begin(), v.end());
for(int i=0; i<v.size(); ++i) 
    {
        cout << v[i] << " ";
    }
    cout << endl;
    for(int i=0; i<v.size(); ++i) 
    {
        cout << v[i] << " ";
    }
    cout << endl;
    
    return 0;
}