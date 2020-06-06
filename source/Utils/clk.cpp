#include <clk.h>

namespace Utils::clk
{
    void ToggleClkModule(bool toggleState)
    {
        tsl::hlp::doWithSDCardHandle([toggleState] {
            if (toggleState)
            {
                NcmProgramLocation programLocation{
                    .program_id = sysClkTid,
                    .storageID = NcmStorageId_None,
                };
                u64 pid;
                if (R_SUCCEEDED(pmshellLaunchProgram(0, &programLocation, &pid)))
                {
                    mkdir(FLAGSDIR, 0777);
                    fclose(fopen(BOOT2FLAG, "w"));
                }
            }
            else
            {
                if (R_SUCCEEDED(pmshellTerminateProgram(sysClkTid)))
                {
                    remove(BOOT2FLAG);
                }
            }
        });
    }

    void ChangeConfiguration(ClkConfigListItem *item)
    {
        tsl::hlp::doWithSDCardHandle([&item] {
            u64 CurProgramId = Utils::clk::getCurrentProgramId();
            std::string programName = Utils::clk::getProgramName(CurProgramId);
            std::stringstream ss;
            ss << 0 << std::hex << std::uppercase << CurProgramId;
            std::string buff = ss.str();
            mkdir(CONFIGDIR, 0777);
            std::ofstream file{CONFIG_INI};

            std::string configName = item->getConfigName();
            std::string selectedValue = item->getValues().at(item->getCurValue());
            simpleIniParser::Ini *config = simpleIniParser::Ini::parseFile(CONFIG_INI);

            simpleIniParser::IniSection *section = config->findSection(buff, false);
            if (section == nullptr)
            {
                section = new simpleIniParser::IniSection(simpleIniParser::IniSectionType::Section, buff);
                config->sections.push_back(section);
            }

            if (section->findFirstOption(configName) == nullptr)
            {
                if (selectedValue != "0")
                    section->options.push_back(new simpleIniParser::IniOption(simpleIniParser::IniOptionType::Option, configName, selectedValue));
            }
            else
            {
                if (selectedValue != "0")
                    section->findFirstOption(configName)->value = selectedValue;
                else
                    section->options.erase(findIT(section->options, section->findFirstOption(configName)));
            }

            if (section->findFirstOption(programName, false, simpleIniParser::IniOptionType::SemicolonComment, simpleIniParser::IniOptionSearchField::Value) == nullptr)
                section->options.insert(section->options.begin(), new simpleIniParser::IniOption(simpleIniParser::IniOptionType::SemicolonComment, "", programName));

            config->writeToFile(CONFIG_INI);
            delete config;
        });
    }

    int getConfigValuePos(const std::vector<std::string> &values, std::string value)
    {
        int result = 0;
        tsl::hlp::doWithSDCardHandle([&result, values, value] {
            u64 programId = getCurrentProgramId();
            std::string programName = getProgramName(programId);
            std::stringstream ss;
            ss << 0 << std::hex << std::uppercase << programId;
            std::string buff = ss.str();
            simpleIniParser::Ini *config = simpleIniParser::Ini::parseFile(CONFIG_INI);
            simpleIniParser::IniSection *section = config->findSection(buff, false);

            if (section != nullptr)
            {
                simpleIniParser::IniOption *option = section->findFirstOption(value);
                if (option != nullptr)
                    result = findInVector<std::string>(values, option->value);
            }

            delete config;
        });
        return result;
    }

    ClkState getClkState()
    {
        ClkState clkState;
        tsl::hlp::doWithSDCardHandle([&clkState] {
            u64 pid = 0;

            if (R_SUCCEEDED(pmdmntGetProcessId(&pid, sysClkTid)))
            {
                if (pid > 0)
                {
                    //note that this returns instantly
                    //because the file might not exist but still be running
                    clkState = ClkState::Enabled;
                    return;
                }
                else
                    clkState = ClkState::Error;
            }
            else
                clkState = ClkState::Disabled;

            if (!std::filesystem::exists(PROGRAMDIR))
                clkState = ClkState::NotFound;
        });
        return clkState;
    }

    u64 getCurrentProgramId()
    {
        Result rc;
        u64 proccessId;
        u64 programId;
        rc = pmdmntGetApplicationProcessId(&proccessId);
        if (R_SUCCEEDED(rc))
        {
            rc = pminfoGetProgramId(&programId, proccessId);
            if (R_FAILED(rc))
                return 0x0100000000001000ULL;
            else
                return programId;
        }
        else
            return 0x0100000000001000ULL;
    }

    std::string getProgramName(u64 programId)
    {
        if (programId == 0x0100000000001000ULL)
            return std::string("Home Menu");

        static NsApplicationControlData appControlData;
        size_t appControlDataSize = 0;
        NacpLanguageEntry *languageEntry = nullptr;
        Result rc;

        memset(&appControlData, 0x00, sizeof(NsApplicationControlData));

        rc = nsGetApplicationControlData(NsApplicationControlSource::NsApplicationControlSource_Storage, programId, &appControlData, sizeof(NsApplicationControlData), &appControlDataSize);
        if (R_FAILED(rc))
        {
            std::stringstream ss;
            ss << 0 << std::hex << std::uppercase << programId;
            return ss.str();
        }
        rc = nacpGetLanguageEntry(&appControlData.nacp, &languageEntry);
        if (R_FAILED(rc))
        {
            std::stringstream ss;
            ss << 0 << std::hex << std::uppercase << programId;
            return ss.str();
        }
        return std::string(languageEntry->name);
    }
} // namespace Utils::clk