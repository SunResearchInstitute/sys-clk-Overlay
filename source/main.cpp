#include "main.h"

class GuiMain : public tsl::Gui
{
public:
    GuiMain() {}
    ~GuiMain() {}

    // Called when switching Guis to create the new UI
    virtual tsl::Element *createUI()
    {
        tsl::element::Frame *rootFrame = new tsl::element::Frame();

        ClkState state = Utils::clk::getClkState();
        if ((int)state < 0)
        {
            //Stolen from ovl-sysmodules
            tsl::element::CustomDrawer *warning = new tsl::element::CustomDrawer(0, 0, 100, FB_WIDTH, [](u16 x, u16 y, tsl::Screen *screen) {
                screen->drawString("\uE150", false, 180, 250, 90, tsl::a(0xFFFF));
                screen->drawString("Could not load sys-clk!", false, 110, 340, 25, tsl::a(0xFFFF));
            });

            rootFrame->addElement(warning);

            return rootFrame;
        }
        if (Utils::clk::getCurrentPorgramId() == 0)
        {
            tsl::element::CustomDrawer *warning = new tsl::element::CustomDrawer(0, 0, 100, FB_WIDTH, [](u16 x, u16 y, tsl::Screen *screen) {
                screen->drawString("\uE150", false, 180, 250, 90, tsl::a(0xFFFF));
                screen->drawString("Game is not open!", false, 110, 340, 25, tsl::a(0xFFFF));
            });

            rootFrame->addElement(warning);

            return rootFrame;
        }

        tsl::element::List *clkList = new tsl::element::List();

        clkList->addItem(new tsl::element::ToggleListItem("sys-clk", (state == ClkState::Enabled)));

        ValueListItem *DockedCPU = new ValueListItem("Docked CPU Clock", CPUClocks, Utils::clk::getConfigValuePos(CPUClocks, "docked_cpu"), "docked_cpu");
        DockedCPU->setStateChangeListener(ChangeConfiguration);
        clkList->addItem(DockedCPU);

        ValueListItem *DockedGPU = new ValueListItem("Docked GPU Clock", GPUClocks, Utils::clk::getConfigValuePos(GPUClocks, "docked_gpu"), "docked_gpu");
        DockedCPU->setStateChangeListener(ChangeConfiguration);
        clkList->addItem(DockedGPU);

        ValueListItem *DockedMEM = new ValueListItem("Docked MEM Clock", MEMClocks, Utils::clk::getConfigValuePos(MEMClocks, "docked_mem"), "docked_mem");
        DockedCPU->setStateChangeListener(ChangeConfiguration);
        clkList->addItem(DockedMEM);

        ValueListItem *HandheldCPU = new ValueListItem("Handheld CPU Clock", CPUClocks, Utils::clk::getConfigValuePos(CPUClocks, "hanhandle_cpu"), "hanhandle_cpu");
        DockedCPU->setStateChangeListener(ChangeConfiguration);
        clkList->addItem(HandheldCPU);

        ValueListItem *HandheldGPU = new ValueListItem("Handheld GPU Clock", GPUClocks, Utils::clk::getConfigValuePos(GPUClocks, "handheld_gpu"), "handheld_gpu");
        DockedCPU->setStateChangeListener(ChangeConfiguration);
        clkList->addItem(HandheldGPU);

        ValueListItem *HandheldMEM = new ValueListItem("Handheld MEM Clock", MEMClocks, Utils::clk::getConfigValuePos(MEMClocks, "handheld_mem"), "handheld_mem");
        DockedCPU->setStateChangeListener(ChangeConfiguration);
        clkList->addItem(HandheldMEM);

        rootFrame->addElement(clkList);

        return rootFrame;
    }
};

void ChangeConfiguration(const std::vector<std::string> configValues, int valueSelection, std::string configName)
{
    u64 programId = Utils::clk::getCurrentPorgramId();
    std::string programName = Utils::clk::getProgramName(programId);
    std::stringstream ss;
    ss << 0 << std::hex << std::uppercase << programId;
    auto buff = ss.str();
    mkdir(CONFIGDIR, 0777);
    fclose(fopen(CONFIG_INI, "a"));

    simpleIniParser::Ini *config = simpleIniParser::Ini::parseFile(CONFIG_INI);

    simpleIniParser::IniSection *section = config->findSection(buff, false);
    if (section == nullptr)
    {
        config->sections.push_back(new simpleIniParser::IniSection(simpleIniParser::IniSectionType::Section, buff));
        section = config->findSection(buff, false);
    }

    if (section->findFirstOption(configName) == nullptr)
        section->options.push_back(new simpleIniParser::IniOption(simpleIniParser::IniOptionType::Option, configName, configValues.at(valueSelection)));

    else
        section->findFirstOption(configName)->value = configValues.at(valueSelection);

    if (section->findFirstOption(programName, false, simpleIniParser::IniOptionType::SemicolonComment, simpleIniParser::IniOptionSearchField::Value) == nullptr)
        section->options.insert(section->options.begin(), new simpleIniParser::IniOption(simpleIniParser::IniOptionType::SemicolonComment, "", programName));

    config->writeToFile(CONFIG_INI);
    delete config;
}

class SysClkOverlay : public tsl::Overlay
{
public:
    SysClkOverlay() {}
    ~SysClkOverlay() {}

    tsl::Gui *onSetup()
    {
        pmdmntInitialize();
        return new GuiMain();
    } // Called once when the Overlay is created and should return the first Gui to load. Initialize services here

    virtual void onDestroy()
    {
        pmdmntExit();
    } // Called once before the overlay Exits. Exit services here
};

// This function gets called on startup to create a new Overlay object
tsl::Overlay *overlayLoad()
{
    return new SysClkOverlay();
}