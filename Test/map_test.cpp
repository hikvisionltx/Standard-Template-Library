#include "../map.h"
#include <iostream>
using namespace std;

int main()
{
    ltx::map<string, int> mp;
    mp[string("sdf")] = 10;
    cout << mp[string("sdf")];
    ltx::map<int, long long>  x2;
    for(int i=0; i<1e4; ++i)
    {
        x2[i] = 1ll*i*i;
    }
    for(int i=0; i<1e4; i+=rand()%100)
    {
        cout << i << " " << x2[i] << endl;
    }

    
    return 0;
}