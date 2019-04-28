#include "../memory.h"
#include "../deque.h"
#include "../heap.h"
#include "../list.h"
#include "../map.h"
#include "../multimap.h"
#include "../multiset.h"
#include "../priority_queue.h"
#include "../queue.h"
#include "../rb_tree.h"
#include "../set.h"
#include "../stack.h"
#include "../stl_hash_fun.h"
#include "../stl_iterator.h"
#include "../vector.h"
using namespace ltx;

int main()
{
    deque<int> d;
    list<int> l;
    map<int, int> m;
    multimap<int, int> mm;
    multiset<int> ms;
    priority_queue<int> pq;
    set<int> s;
    stack<int> st;
    vector<int> v;
    d.push_back(10);
    l.push_back(10);
    m[1] = 1;
    mm.insert(pair<int, int>(1, 1));
    ms.insert(1);
    pq.push(1);
    s.insert(1);
    st.push(1);
    v.push_back(1);

    return 0;
}