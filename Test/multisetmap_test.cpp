#include "../multiset.h"
#include "../multimap.h"

#include <iostream>
using namespace std;

int main()
{
    ltx::multimap<int, int> mp;

    for(int i=0; i<20; ++i) 
    {
        mp.insert(pair<int, int>(i, i*i));
        mp.insert(pair<int, int>(i, i*i*i));
    }
    for(auto ite : mp)
    {
        cout << ite.first << " " << ite.second << " ";
    }




    return 0;
}
