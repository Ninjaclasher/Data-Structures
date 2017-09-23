#ifndef BINARY_INDEX_TREE_DATA_STRUCTURE_H
#define BINARY_INDEX_TREE_DATA_STRUCTURE_H

#include "data_structure.h"
#include <vector>
#include <type_traits>

template <typename Tstored, typename Tsize>
class BinaryIndexTree : public DataStructure<Tsize>
{
    static_assert(std::is_integral<Tstored>::value, "Integer type required.");
    static_assert(std::is_arithmetic<Tstored>::value, "Arithmetic is not supported with supplied type.");
public:
    BinaryIndexTree(const Tsize);
    Tstored range (Tsize);
    void update (Tsize, const Tstored);
    void change (Tsize, const Tstored);
    void reset();

private:
    std::vector<Tstored> tree;

};

template <typename Tstored, typename Tsize>
BinaryIndexTree<Tstored, Tsize>::BinaryIndexTree (const Tsize n) : DataStructure<Tsize> (n)
{
    tree.resize(this->numElements, 0);
    reset();
}

template <typename Tstored, typename Tsize>
void BinaryIndexTree<Tstored, Tsize>::reset()
{
    fill (tree.begin(), tree.end(), 0);
}

template <typename Tstored, typename Tsize>
Tstored BinaryIndexTree<Tstored, Tsize>::range (Tsize index)
{
    Tstored sum = 0;
    for (; index > 0; index -= index&-index)
        sum+=tree[index];
    return sum;
}

template <typename Tstored, typename Tsize>
void BinaryIndexTree<Tstored, Tsize>::update (Tsize index, const Tstored val)
{
    for (; index <= this->numElements; index += index&-index)
        tree[index] += val;
}

template <typename Tstored, typename Tsize>
void BinaryIndexTree<Tstored, Tsize>::change (Tsize index, const Tstored val)
{
    update (index, val-range(index)+range(index-1));
}

#endif