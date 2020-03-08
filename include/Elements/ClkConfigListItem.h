#pragma once
#include "ValueListItem.h"
#include "helper.h"

class ClkConfigListItem : public ValueListItem
{
public:
    ClkConfigListItem(std::string text, const std::vector<std::string> &values, const std::string &configName);
    ~ClkConfigListItem();

    const std::string &getConfigName() { return ConfigName; };
    void setConfigCurValue();

private:
    const std::string &ConfigName;
};