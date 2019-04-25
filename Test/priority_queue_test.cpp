#include "../priority_queue.h"
#include <iostream>


using namespace std;

int main()
{
    ltx::priority_queue<int> que;
    for(int i=0; i<10; ++i)
    {
        que.push(rand()%100);
    }
    while (!que.empty())
    {   
        cout << que.top() << ' ';
        que.pop();
    }


    return 0;
}