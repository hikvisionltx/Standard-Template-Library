#include "../stl_numeric.h"
#include "../vector.h"
#include "../stl_algo.h"
#include <iostream>
#include <ctime>
#include <algorithm>
using namespace std;

int* partition(int *l, int *r)
{
    int x = rand()%(r-l);
    swap(*r, *(l+x));
    int pivot = *(r-1);
    while(true)
    {
        while(*l < pivot) ++l;
        --r;
        while(pivot < *r) --r;
        if(!(l<r)) return l;
        swap(*l, *r);
        ++l;
    }   
}
void quick_sort(int *l, int *r)
{
    if(l < r)
    {
        int * m = partition(l, r);
        quick_sort(l, m);
        quick_sort(m+1, r);
    }

    
}

const int len = 1e6;
int a1[len], a2[len];

int main()
{
    // ltx::vector<int> v;
    // for(int i=0; i<10; ++i) v.push_back(i), v.push_back(i);
    // cout << ltx::accumulate(v.begin(), v.end(), 0) << endl;
    // cout << ltx::accumulate(v.begin(), v.end(), 0, [](const int&x, const int &y)->int{cout << x << " " << y << endl;return x+y;}) << endl;
    // cout << ltx::lower_bound(v.begin(), v.end(), 5) - v.begin() << endl;
    // cout << ltx::lower_bound(v.begin(), v.end(), 5, 
    // [](const int&x, const int&y)->bool{return x<y;}) - v.begin() << endl;
    // cout << ltx::upper_bound(v.begin(), v.end(), 5) - v.begin() << endl;
    // cout << ltx::upper_bound(v.begin(), v.end(), 5, 
    // [](const int&x, const int&y)->bool{return x<y;}) - v.begin() << endl;
    // cout << boolalpha << ltx::binary_search(v.begin(), v.end(), 8) << endl;
    
    // ltx::vector<int> a;
    // for(int i=0; i<4; ++i) a.push_back(i);
    // // ltx::reverse(a.begin(), a.end());
    
    // //     ltx::iter_swap(a.begin(), a.end()-1);
    // //     for(auto ite : a) cout << ite << " ";
    // //     cout << endl;
    // do
    // {
    //     for(auto ite : a) cout << ite << " ";
    //     cout << endl;
    // } while (ltx::next_permutation(a.begin(), a.end()));
    // cout << "-------" << endl;
    // ltx::reverse(a.begin(), a.end());
    // do
    // {
    //     for(auto ite : a) cout << ite << " ";
    //     cout << endl;
    // } while (ltx::prev_permutation(a.begin(), a.end()));
    
    // ltx::vector<int> sv;
    // for(int i=0; i<10; ++i) sv.push_back(rand()%100);
    // // ltx::__insertion_sort(sv.begin(), sv.end());

    // ltx::sort(sv.begin(), sv.end());
    //     for(auto ite : sv) cout << ite << " ";
    //     cout << endl;

    // ltx::vector<int> v1, v2;
    // int t;
    
    // srand(233);
    // for(int i=0; i<1e7; ++i)
    // {
    //     t = rand();
    //     v1.push_back(t);
    //     v2.push_back(t);
    // }
    // int begin_time = clock();
    // std::sort(v1.begin(), v1.end());
    // cout << clock() - begin_time << endl;
    // begin_time = clock();
    // ltx::sort(v2.begin(), v2.end());
    // cout << clock() - begin_time << endl;
    // for(int i=0; i<v1.size(); ++i)
    // {
    //     if(v1[i] != v2[i]) 
    //     {
    //         cout << "fuck" << endl;
    //         break;
    //     }
    // }
    //     std::sort(v1.begin(), v1.end());
    // cout << clock() - begin_time << endl;
    // begin_time = clock();
    // ltx::sort(v2.begin(), v2.end());
    // cout << clock() - begin_time << endl;
    // for(int i=0; i<v1.size(); ++i) v1[i] = v2[i] = 0;
    //     std::sort(v1.begin(), v1.end());
    // cout << clock() - begin_time << endl;
    // begin_time = clock();
    // ltx::sort(v2.begin(), v2.end());
    // cout << clock() - begin_time << endl;
    
    // int ar[10];
    // for(int i=0; i<10; ++i) ar[i] = rand();
    // quick_sort(ar, ar+10);
    // for(auto ite : ar) cout << ite << " ";

    for(int i=0; i<len; ++i) a1[i] = a2[i] = rand();
    int begin_time = clock();
    ltx::sort(a1, a1+len);
    cout << clock() - begin_time << endl;
    begin_time = clock();
    quick_sort(a2, a2+len);
    cout << clock() - begin_time << endl;

    begin_time = clock();
    ltx::sort(a1, a1+len);
    cout << clock() - begin_time << endl;
    begin_time = clock();
    quick_sort(a2, a2+len);
    cout << clock() - begin_time << endl;
    
    return 0;
}
