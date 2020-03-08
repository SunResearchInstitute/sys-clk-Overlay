#pragma once
#include <vector>
#include <algorithm>

#define R_ASSERT(res_expr) \
    ({ \
        const auto rc = (res_expr); \
        if (R_FAILED(rc)) {  \
            fatalThrow(rc); \
        } \
    })

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