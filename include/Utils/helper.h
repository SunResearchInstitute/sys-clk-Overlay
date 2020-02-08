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
        result = distance(vecOfElements.begin(), it);
    }
    else
    {
        result = -1;
    }

    return result;
}