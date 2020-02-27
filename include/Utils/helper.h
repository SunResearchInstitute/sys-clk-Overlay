#pragma once
#include <vector>
#include <algorithm>

template <typename T>
int findInVector(const std::vector<T> &vecOfElements, const T &element)
{
    // Find given element in vector
    auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

    if (it != vecOfElements.end())
        return std::distance(vecOfElements.begin(), it);
    else
        return 0;
}

template <typename T>
auto findIT(const std::vector<T> &vecOfElements, const T &element)
{
    return std::find(vecOfElements.begin(), vecOfElements.end(), element);
}