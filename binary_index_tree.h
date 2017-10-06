#ifndef BINARY_INDEX_TREE_DATA_STRUCTURE_H
#define BINARY_INDEX_TREE_DATA_STRUCTURE_H

#include "data_structure.h"
#include <vector>
#include <type_traits>

template <typename T>
class BinaryIndexTree : public DataStructure<unsigned int>
{
    static_assert(std::is_integral<T>::value, "Integer type required.");
    static_assert(std::is_arithmetic<T>::value, "Arithmetic is not supported with supplied type.");
public:
    BinaryIndexTree(const unsigned int);
    BinaryIndexTree(const unsigned int, T);
    T query (unsigned int);
    void update (unsigned int, const T);
    void change (unsigned int, const T);
    void reset();

private:
    std::vector<T> tree;
};

template <typename T>
BinaryIndexTree<T>::BinaryIndexTree (const unsigned int n) : DataStructure<unsigned int> (n)
{
    tree.resize(this->numElements, 0);
}

template <typename T>
BinaryIndexTree<T>::BinaryIndexTree (const unsigned int n, T val) : DataStructure<unsigned int> (n)
{
    tree.resize(this->numElements, val);
}

template <typename T>
void BinaryIndexTree<T>::reset()
{
    fill (tree.begin(), tree.end(), 0);
}

template <typename T>
T BinaryIndexTree<T>::query (unsigned int index)
{
    T sum = 0;
    for (; index > 0; index -= index&-index)
        sum+=tree[index];
    return sum;
}

template <typename T>
void BinaryIndexTree<T>::update (unsigned int index, const T val)
{
    for (; index <= this->numElements; index += index&-index)
        tree[index] += val;
}

template <typename T>
void BinaryIndexTree<T>::change (unsigned int index, const T val)
{
    update (index, val-query(index)+query(index-1));
}

#endif