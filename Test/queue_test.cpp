#include "../queue.h"
#include "../list.h"
#include <iostream>
using namespace std;

int main()
{
    // ltx::queue<int> q;
    
    ltx::queue<int, ltx::list<int>> q;
    q.push(10);
    cout << q.front();
    q.push(20);
    q.push(30);
    q.pop();
    cout << q.front() << endl;
    
    

    return 0;
}