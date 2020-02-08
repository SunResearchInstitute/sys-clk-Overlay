#pragma once
#include <switch.h>
#include <filesystem>
#include <vector>
#include <cstring>
#include <SimpleIniParser.hpp>
#include <sys/stat.h>
#include "helper.h"

#define CONTENTSDIR "sdmc:/atmosphere/contents/00FF0000636C6BFF"
#define CONFIGDIR "sdmc:/config/sys-clk"
#define CONFIG_INI CONFIGDIR "/config.ini"

#define PROGRAMDIR CONTENTSDIR "/exefs.nsp"

const u64 sysClkTid = 0x00FF0000636C6BFF;

enum class ClkState
{
    NotFound = -2,
    Error,
    Disabled,
    Enabled
};

const std::vector<std::string> CPUClocks{
    "1785",
    "1683",
    "1581",
    "1428",
    "1326",
    "1224",
    "1122",
    "1020",
    "918",
    "816",
    "714",
    "612",
    "0"};

const std::vector<std::string> MEMClocks{
    "1600",
    "1331",
    "1065",
    "800",
    "665",
    "0"};

const std::vector<std::string> GPUClocks{
    "921",
    "844",
    "768",
    "691",
    "614",
    "537",
    "460",
    "384",
    "307",
    "230",
    "153",
    "76",
    "0"};

namespace Utils::clk
{
ClkState getClkState();
u64 getCurrentPorgramId();
std::string getProgramName(u64 programId);
int getConfigValuePos(const std::vector<std::string> values, std::string value);
} // namespace Utils::clk