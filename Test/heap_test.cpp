#include "../heap.h"
#include <iostream>
#include <ctime>
using namespace std;

int main()
{
    // srand(time(0));
    int n = 20;
    int a[n+1];
    for(int i=0; i<n; ++i)
    {
        a[i] = rand()%100;
    }
    a[n] = 1234;
    ltx::make_heap(a, a+n, greater<int>());
    ltx::push_heap(a, a+n+1, greater<int>());
    ltx::sort_heap(a, a+n+1, greater<int>());
    for(auto ite : a) cout << ite << " ";
    return 0;
}