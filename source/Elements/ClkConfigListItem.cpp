#include "ClkConfigListItem.h"
#include "clk.h"

ClkConfigListItem::ClkConfigListItem(std::string text, const std::vector<std::string> &values, const std::string &configName) : ValueListItem(text, values), ConfigName(configName) { m_curValue = Utils::clk::getConfigValuePos(this->m_values, this->ConfigName); }
ClkConfigListItem::~ClkConfigListItem() { Utils::clk::ChangeConfiguration(this); }

void ClkConfigListItem::setConfigWithCurValue() { ClkConfigListItem::setCurValue(Utils::clk::getConfigValuePos(this->getValues(), this->getConfigName())); }
