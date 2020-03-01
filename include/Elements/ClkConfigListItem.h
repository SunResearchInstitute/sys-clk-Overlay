#pragma once
#include "ValueListItem.h"
#include "../Utils/clk.h"

class ClkConfigListemItem : public ValueListItem
{
public:
    ClkConfigListemItem(std::string text, const std::vector<std::string> values, int defaultPos, const std::string data) : ValueListItem(text, values, defaultPos, data) {}
    ~ClkConfigListemItem() { Utils::clk::ChangeConfiguration(this); }
};