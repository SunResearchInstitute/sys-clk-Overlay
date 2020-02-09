#pragma once

#include <utility>
#include <vector>
#include <algorithm>

template <typename T>
int findInVector(const std::vector<T> &vecOfElements, const T &element)
{
    int result;
    // Find given element in vector
    auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

    if (it != vecOfElements.end())
    {
        result = std::distance(vecOfElements.begin(), it);
    }
    else
    {
        result = 0;
    }

    return result;
}