#include "../set.h"
#include <iostream>
using namespace std;

int main()
{
    ltx::set<int> s;
    s.insert(12);
    s.insert(234);
    s.insert(2);
    cout << s.count(12) << endl;
    for(int i=0; i<20; ++i) s.insert(rand()%100);
    for(auto ite : s) cout << ite << " "; cout << endl;
    int cnt =0;
    for(auto ite=s.begin(); ite!=s.end(); )
    {
        if(*ite % 2 == 0)
        {
            s.erase(ite++);
        }
        else ++ite;
    }

    if(s.find(7) != s.end()) cout << "yes" <<endl;

    
    for(auto ite : s) cout << ite << " "; cout << endl;

    return 0;
}