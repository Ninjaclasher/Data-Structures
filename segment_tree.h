#ifndef SEGMENT_TREE_DATA_STRUCTURE_H
#define SEGMENT_TREE_DATA_STRUCTURE_H

#include "data_structure.h"

#include <cmath>
#include <functional>
#include <queue>
#include <type_traits>
#include <vector>

template <typename Tstored, typename Tsize>
class SegmentTree : public DataStructure<Tsize>
{
    static_assert(std::is_integral<Tstored>::value, "Integer type required.");
    static_assert(std::is_arithmetic<Tstored>::value, "Arithmetic is not supported with supplied type.");
public:
    SegmentTree(const std::vector<Tstored>&, std::function<Tstored(Tstored, Tstored)>);
    void update(Tsize, Tstored);
    void update(Tsize, Tsize, Tstored);
    Tstored query (Tsize, Tsize);
    void reset();

private:
    Tstored construct(const std::vector<Tstored>&, Tsize, Tsize, Tsize);
    void updateRangeUtil(Tsize, Tsize, Tsize, Tsize, Tsize, Tstored);
    std::vector<Tstored> tree;
    std::vector<Tstored> lazy;
    std::function<Tstored(Tstored, Tstored)> run;
};

template <typename Tstored, typename Tsize>
SegmentTree<Tstored,Tsize>::SegmentTree(const std::vector<Tstored>& originalArray, std::function<Tstored(Tstored, Tstored)> aa) 
    : DataStructure<Tsize>(originalArray.size()), run(aa)
{
    Tsize nElements = 2*pow(2, ceil(log2(this->numElements)))-1;
    tree.resize(nElements);
    lazy.resize(nElements);
    construct(originalArray, 0, this->numElements-1, 0);
} 

template <typename Tstored, typename Tsize>
void SegmentTree<Tstored,Tsize>::reset()
{
    fill(tree.begin(), tree.end(), 0);
    fill(lazy.begin(), lazy.end(), 0);
}

template <typename Tstored, typename Tsize>
Tstored SegmentTree<Tstored,Tsize>::query(Tsize qs, Tsize qe)
{
    Tstored result = 0;
    std::queue<std::pair<Tsize,std::pair<Tsize,Tsize>>> buf;
    buf.push(std::make_pair(0, std::make_pair(0, this->numElements-1)));

    while (!buf.empty())
    {
        Tsize v = buf.front().first, s = buf.front().second.first, e = buf.front().second.second;
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
            Tsize mid = s + (e -s)/2;
            buf.push(std::make_pair(2*v+1, std::make_pair(s, mid)));
            buf.push(std::make_pair(2*v+2, std::make_pair(mid+1, e)));
        }
    }
    return result;
}
 
template <typename Tstored, typename Tsize>
void SegmentTree<Tstored,Tsize>::update(Tsize index, Tstored val)
{
    std::queue<std::pair<Tsize,std::pair<Tsize,Tsize>>> buf;
    buf.push(std::make_pair(0, std::make_pair(0, this->numElements-1)));

    while (!buf.empty())
    {
        Tsize v = buf.front().first, s = buf.front().second.first, e = buf.front().second.second;
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
                Tsize mid = s + (e -s)/2;
                buf.push(std::make_pair(2*v+1, std::make_pair(s, mid)));
                buf.push(std::make_pair(2*v+2, std::make_pair(mid+1, e)));
            }
        }
    } 
}

template <typename Tstored, typename Tsize>
void SegmentTree<Tstored, Tsize>::update(Tsize us, Tsize ue, Tstored val)
{
    updateRangeUtil(0, 0, this->numElements-1, us, ue, val);
}

template <typename Tstored, typename Tsize>
void SegmentTree<Tstored,Tsize>::updateRangeUtil(Tsize v, Tsize s, Tsize e, Tsize l, Tsize r, Tstored val)
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

template <typename Tstored, typename Tsize>
Tstored SegmentTree<Tstored,Tsize>::construct(const std::vector<Tstored>& arr, Tsize s, Tsize e, Tsize v)
{
    if (s == e)
    {
        tree[v] = arr[s];
        return arr[s];
    }
 
    Tsize mid = s + (e -s)/2;
    tree[v] = run(construct(arr, s, mid, v*2+1), construct(arr, mid+1, e, v*2+2));
    return tree[v];
}

#endif