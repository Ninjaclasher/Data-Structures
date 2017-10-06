#ifndef DISJOINT_SET_DATA_STRUCTURE_H
#define DISJOINT_SET_DATA_STRUCTURE_H

#include "data_structure.h"

#include <algorithm>
#include <vector>

template <typename T>
class DisjointSet : public DataStructure<unsigned int>
{
public:
    DisjointSet(const T);
    T find(T);
    void merge(T,T);
    void reset();

private:
    std::vector<T> rnk, parent;
};

template <typename T>
DisjointSet<T>::DisjointSet(const T n) : DataStructure<unsigned int>(n)
{
    rnk.resize(this->numElements, 0), parent.resize(this->numElements);
    reset();
}

template <typename T>
void DisjointSet<T>::reset()
{
    fill(rnk.begin(), rnk.end(), 0);
    T n = 0;
    generate(parent.begin(), parent.end(), [&n] () {return n++;}); 
}

template <typename T>
T DisjointSet<T>::find(T node)
{
    if (node != parent[node]) parent[node] = find(parent[node]);
    return parent[node];
}

template <typename T>
void DisjointSet<T>::merge(T node1, T node2)
{
    node1 = find(node1), node2 = find(node2);
    if (rnk[node1] > rnk[node2]) parent[node2] = node1;
    else parent[node1] = node2;
    if (rnk[node1] == rnk[node2]) rnk[node2]++;
}

#endif