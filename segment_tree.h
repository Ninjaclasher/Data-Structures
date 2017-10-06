#ifndef SEGMENT_TREE_DATA_STRUCTURE_H
#define SEGMENT_TREE_DATA_STRUCTURE_H

#include "data_structure.h"

#include <cmath>
#include <functional>
#include <queue>
#include <type_traits>
#include <vector>

template <typename T>
class SegmentTree : public DataStructure<unsigned int>
{
    static_assert(std::is_integral<T>::value, "Integer type required.");
    static_assert(std::is_arithmetic<T>::value, "Arithmetic is not supported with supplied type.");
public:
    SegmentTree(const unsigned int);
    SegmentTree(const unsigned int, std::function<T(T, T)>);
    SegmentTree(const std::vector<T>&, std::function<T(T, T)>);
    SegmentTree(const std::vector<T>&);
    void update(unsigned int, T);
    void update(unsigned int, unsigned int, T);
    T query (unsigned int, unsigned int);
    void reset();

private:
    T construct(const std::vector<T>&, unsigned int, unsigned int, unsigned int);
    void updateRangeUtil(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, T);
    std::vector<T> tree, lazy;
    std::function<T(T, T)> run;
};

template <typename T>
SegmentTree<T>::SegmentTree(const unsigned int n)
    : DataStructure<unsigned int>(n)
{
    run = [](T n1, T n2) {return n1+n2;};
    unsigned int nElements = 2*pow(2, ceil(log2(this->numElements)))-1;
    tree.resize(nElements);
    lazy.resize(nElements);    
}

template <typename T>
SegmentTree<T>::SegmentTree(const unsigned int n, std::function<T(T, T)> func)
    : DataStructure<unsigned int>(n), run(func)
{
    unsigned int nElements = 2*pow(2, ceil(log2(this->numElements)))-1;
    tree.resize(nElements);
    lazy.resize(nElements);
}

template <typename T>
SegmentTree<T>::SegmentTree(const std::vector<T>& originalArray) 
    : DataStructure<unsigned int>(originalArray.size())
{
    run = [](T n1, T n2) {return n1+n2;};    
    unsigned int nElements = 2*pow(2, ceil(log2(this->numElements)))-1;
    tree.resize(nElements);
    lazy.resize(nElements);
    construct(originalArray, 0, this->numElements-1, 0);
}

template <typename T>
SegmentTree<T>::SegmentTree(const std::vector<T>& originalArray, std::function<T(T, T)> func) 
    : DataStructure<unsigned int>(originalArray.size()), run(func)
{
    unsigned int nElements = 2*pow(2, ceil(log2(this->numElements)))-1;
    tree.resize(nElements);
    lazy.resize(nElements);
    construct(originalArray, 0, this->numElements-1, 0);
}

template <typename T>
void SegmentTree<T>::reset()
{
    fill(tree.begin(), tree.end(), 0);
    fill(lazy.begin(), lazy.end(), 0);
}

template <typename T>
T SegmentTree<T>::query(unsigned int qs, unsigned int qe)
{
    T result = 0;
    std::queue<std::pair<unsigned int,std::pair<unsigned int,unsigned int>>> buf;
    buf.push(std::make_pair(0, std::make_pair(0, this->numElements-1)));

    while (!buf.empty())
    {
        unsigned int v = buf.front().first, s = buf.front().second.first, e = buf.front().second.second;
        buf.pop();
        if (lazy[v])
        {
            tree[v] = run((e-s+1)*lazy[v], tree[v]);
            if (s != e)
            {
                lazy[2*v+1] += lazy[v];
                lazy[2*v+2] += lazy[v];
            }
            lazy[v] = 0;
        }
        if (qs <= s && qe >= e)
            result = run(result, tree[v]);
        else if (s <= qe && e >= qs)
        {
            unsigned int mid = s + (e -s)/2;
            buf.push(std::make_pair(2*v+1, std::make_pair(s, mid)));
            buf.push(std::make_pair(2*v+2, std::make_pair(mid+1, e)));
        }
    }
    return result;
}
 
template <typename T>
void SegmentTree<T>::update(unsigned int index, T val)
{
    std::queue<std::pair<unsigned int,std::pair<unsigned int,unsigned int>>> buf;
    buf.push(std::make_pair(0, std::make_pair(0, this->numElements-1)));

    while (!buf.empty())
    {
        unsigned int v = buf.front().first, s = buf.front().second.first, e = buf.front().second.second;
        buf.pop();
        if (lazy[v])
        {
            tree[v] = run((e-s+1)*lazy[v], tree[v]);
            if (s != e)
            {
                lazy[2*v+1] += lazy[v];
                lazy[2*v+2] += lazy[v];
            }
            lazy[v] = 0;
        }
        if (index >= s && index <= e)
        {
            tree[v] = run(val, tree[v]);
            if (s != e)
            {
                unsigned int mid = s + (e -s)/2;
                buf.push(std::make_pair(2*v+1, std::make_pair(s, mid)));
                buf.push(std::make_pair(2*v+2, std::make_pair(mid+1, e)));
            }
        }
    } 
}

template <typename T>
void SegmentTree<T>::update(unsigned int us, unsigned int ue, T val)
{
    updateRangeUtil(0, 0, this->numElements-1, us, ue, val);
}

template <typename T>
void SegmentTree<T>::updateRangeUtil(unsigned int v, unsigned int s, unsigned int e, unsigned int l, unsigned int r, T val)
{
    if (lazy[v])
    { 
        tree[v] = run((e-s+1)*lazy[v], tree[v]);  
        if(s != e)
        {
            lazy[2*v+1] += lazy[v];  
            lazy[2*v+2] += lazy[v];       
        }
        lazy[v] = 0;                                 
    }
    if (s > e || s > r || e < l)          
        return;
    if (s >= l && e <= r)
    {
        tree[v] = run((e-s+1)*val, tree[v]);
        if(s != e)
        {
            lazy[2*v+1] += val;
            lazy[2*v+2] += val;
        }
        return;
    }
    int mid = (s + e) / 2;
    updateRangeUtil(2*v+1, s, mid, l, r, val); 
    updateRangeUtil(2*v+2, mid + 1, e, l, r, val); 
    tree[v] = run(tree[2*v+1], tree[2*v+2]);  
}

template <typename T>
T SegmentTree<T>::construct(const std::vector<T>& arr, unsigned int s, unsigned int e, unsigned int v)
{
    if (s == e)
    {
        tree[v] = arr[s];
        return arr[s];
    }
 
    unsigned int mid = s + (e -s)/2;
    tree[v] = run(construct(arr, s, mid, v*2+1), construct(arr, mid+1, e, v*2+2));
    return tree[v];
}

#endif