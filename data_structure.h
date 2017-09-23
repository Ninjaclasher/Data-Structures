#ifndef DATA_STRUCTURE_INTERFACE_H
#define DATA_STRUCTURE_INTERFACE_H

#include <type_traits>

template <typename T>
class DataStructure
{
    static_assert(std::is_integral<T>::value, "Integer type required.");
    static_assert(std::is_arithmetic<T>::value, "Arithmetic is not supported with supplied type.");
public:
    virtual void reset() = 0;
    DataStructure(const T);
    virtual T size() const;

private:

protected:
    const T numElements;
};

template <typename T>
DataStructure<T>::DataStructure(const T n) : numElements(n) {}

template <typename T>
T DataStructure<T>::size() const
{
    return numElements;
}

#endif