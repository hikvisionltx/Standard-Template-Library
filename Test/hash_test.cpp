#include "../stl_hash_fun.h"
#include <iostream>
using namespace std;

int main()
{
    char s[] = "fuck";
    char t[] = "fuck";
    cout << ltx::hash<char*>()(s) << endl;
    cout << ltx::hash<char*>()(t) << endl;


    return 0; 
}