#pragma once
#include "Utils/clk.h"
#include <tesla.hpp>
#include <Elements/ValueListItem.h>
#include <SimpleIniParser.hpp>
#include <cstring>

void ChangeConfiguration(const std::vector<std::string> configValues, int valueSelection, std::string configName);