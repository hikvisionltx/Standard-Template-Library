#include "../stack.h"
#include "../vector.h"
#include "../list.h"
#include <iostream>
using namespace std;

int main()
{
    // ltx::stack<int> s;
    ltx::stack<int, ltx::vector<int>> s;
    for(int i=0; i<10; ++i) s.push(i*i);
    while(!s.empty())
    {
        cout << s.top() << endl;
        s.pop();
    }
    return 0;
}