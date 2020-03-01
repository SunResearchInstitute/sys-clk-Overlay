#pragma once
#include "ValueListItem.h"
#include "../Utils/clk.h"

class ClkConfigListItem : public ValueListItem
{
public:
    ClkConfigListItem(std::string text, const std::vector<std::string> values, int defaultPos, const std::string data) : ValueListItem(text, values, defaultPos, data) {}
    ~ClkConfigListItem() { Utils::clk::ChangeConfiguration(this); }
};