#pragma once
#include <switch.h>
#include <filesystem>
#include <cstring>
#include <SimpleIniParser.hpp>
#include <sys/stat.h>
#include "helper.h"

#define CONTENTSDIR "sdmc:/atmosphere/contents/00FF0000636C6BFF"
#define CONFIGDIR "sdmc:/config/sys-clk"
#define CONFIG_INI CONFIGDIR "/config.ini"
#define FLAGSDIR CONTENTSDIR "/flags"
#define BOOT2FLAG FLAGSDIR "/boot2.flag"
#define PROGRAMDIR CONTENTSDIR "/exefs.nsp"

const u64 sysClkTid = 0x00FF0000636C6BFFULL;

enum class ClkState
{
    NotFound = -2,
    Error,
    Disabled,
    Enabled
};

const std::vector<std::string> CPUClocks{
    "0",
    "612",
    "714",
    "816",
    "918",
    "1020",
    "1122",
    "1224",
    "1326",
    "1428",
    "1581",
    "1683",
    "1785"};

const std::vector<std::string> MEMClocks{
    "0",
    "665",
    "800",
    "1065",
    "1331",
    "1600"};

const std::vector<std::string> GPUClocks{
    "0",
    "76",
    "153",
    "230",
    "307",
    "384",
    "460",
    "537",
    "614",
    "691",
    "768",
    "844",
    "921"};

namespace Utils::clk
{
void ToggleClkModule(bool toggleState);
void ChangeConfiguration(const std::vector<std::string> configValues, int valueSelection, std::string configName);
ClkState getClkState();
u64 getCurrentProgramId();
std::string getProgramName(u64 programId);
int getConfigValuePos(const std::vector<std::string> values, std::string value);
} // namespace Utils::clk