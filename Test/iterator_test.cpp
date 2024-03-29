#include "../stl_iterator.h"
#include <typeinfo>
#include <iostream>
using namespace std;


template <typename T>
class ListItem
{
public:
    ListItem() {_next = nullptr;}
    ListItem(T value, ListItem<T> * next)
    {
        _value = value;
        _next = next;
    }
    T _value;
    ListItem<T> * _next;
};


template <typename T>
class List
{
public:
    void insert_front(T value)
    {
        if(_front == nullptr)
        { 
            ListItem<T> * node = new ListItem<T>(value, nullptr);
            _front = _end = node;
        }
        else 
        {
            ListItem<T> * node = new ListItem<T>(value, _front);
            _front = node;
        }
    }
    void inerst_end(T value)
    {
        if(_front == nullptr)
        {
            ListItem<T> *node = new ListItem<T>(value, nullptr);
            _front = _end = node;
        }
        else 
        {
            ListItem<T> *node = new ListItem<T>(value, nullptr);
            _end->_next = node;
            _end = node;
        }
    }
    void display(std::ostream &os = std::cout) const
    {
        ListItem<T> * t = _front;
        while(t != nullptr)
        {
            os << t->_value << " ";
            t = t->_next;
        }
    }

    List()
    {
        _front = _end = nullptr;
    }
    ListItem<T> * _end;
    ListItem<T> * _front;
    long _size;
};

template <typename Item>
struct ListIter :public ltx::iterator<Item, long>
{
    Item * ptr;
    ListIter(Item * p = nullptr) :ptr(p){}
    Item& operator *() const {return *ptr;}
    Item* operator ->() const {return ptr;}

    ListIter& operator ++()
    {
        ptr = ptr->_next;
        return *this;
    }
    ListIter& operator ++(int)
    {
        ListIter tmp = *this;
        ++*this;
        return tmp;
    }
    bool operator ==(const ListIter& i) const 
    {
        return ptr==i.ptr;
    }
    bool operator !=(const ListIter& i) const
    {
        return ptr!=i.ptr;
    }

};


#include "../vector.h"
#include "../list.h"
int main()
{
    using namespace ltx;
    vector<int> v;
    auto back_insert_ite = ltx::back_inserter(v);
    back_insert_ite = 10;
    back_insert_ite = 20;
    back_insert_ite = 30;
    for(auto ite : v) cout << ite << " "; cout << endl;
    list<int> l;
    auto front_insert_ite = ltx::front_inserter(l);
    front_insert_ite = 1;
    front_insert_ite = 2;
    front_insert_ite = 3;
    for(auto ite : l) cout << ite << " "; cout << endl;
    auto ite = l.begin();
    ++ite;
    auto insertor = ltx::inserter(l, ite);
    insertor = 10;
    insertor = 20;
    for(auto ite : l) cout << ite << " "; cout << endl;

    for(auto rite=v.rbegin(); rite!=v.rend(); ++rite)
    {
        cout << *rite << " " ;
    }
    // ltx::istream_iterator<int> in_iter(std::cin);
    // int t = *in_iter;
    // cout << t << endl;
    // ++in_iter;
    // t = *in_iter;
    // cout << t << endl;

    ltx::ostream_iterator<int> out_iter(std::cout, "-");
    for(int i=0; i<10; ++i) out_iter = i*i;

    // List<int> list;
    // list.inerst_end(10);
    // list.inerst_end(20);
    // list.insert_front(20);
    // list.inerst_end(30);
    // list.display();
    // ListIter<ListItem<int>> begin(list._front);
    // ListIter<ListItem<int>> end;
    // ListIter<ListItem<int>> iter;
    // for(iter = begin; iter!=end; ++iter)
    // {
    //     cout << iter->_value << endl;
    // }


    


    return 0;
}