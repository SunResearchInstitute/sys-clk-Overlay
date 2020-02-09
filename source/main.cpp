#include "main.h"

tsl::element::ToggleListItem *toggleItem;
std::vector<ValueListItem *> ValueListItems;

class GuiMain : public tsl::Gui
{
public:
    GuiMain()
    {
        this->setTitle("sys-clk-Overlay");
    }
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

        toggleItem = new tsl::element::ToggleListItem("sys-clk", state == ClkState::Enabled);
        toggleItem->setStateChangeListener(Utils::clk::EnableClkModule);
        clkList->addItem(toggleItem);

        ValueListItem *DockedCPU = new ValueListItem("Docked CPU Clock", CPUClocks, Utils::clk::getConfigValuePos(CPUClocks, "docked_cpu"), "docked_cpu");
        //DockedCPU->setStateChangeListener(ChangeConfiguration);
        clkList->addItem(DockedCPU);
        ValueListItems.push_back(DockedCPU);

        ValueListItem *DockedGPU = new ValueListItem("Docked GPU Clock", GPUClocks, Utils::clk::getConfigValuePos(GPUClocks, "docked_gpu"), "docked_gpu");
        //DockedGPU->setStateChangeListener(ChangeConfiguration);
        clkList->addItem(DockedGPU);
        ValueListItems.push_back(DockedGPU);

        ValueListItem *DockedMEM = new ValueListItem("Docked MEM Clock", MEMClocks, Utils::clk::getConfigValuePos(MEMClocks, "docked_mem"), "docked_mem");
        //DockedMEM->setStateChangeListener(ChangeConfiguration);
        clkList->addItem(DockedMEM);
        ValueListItems.push_back(DockedMEM);

        ValueListItem *HandheldCPU = new ValueListItem("Handheld CPU Clock", CPUClocks, Utils::clk::getConfigValuePos(CPUClocks, "handhandle_cpu"), "handhandle_cpu");
        //HandheldCPU->setStateChangeListener(ChangeConfiguration);
        clkList->addItem(HandheldCPU);
        ValueListItems.push_back(HandheldCPU);

        ValueListItem *HandheldGPU = new ValueListItem("Handheld GPU Clock", GPUClocks, Utils::clk::getConfigValuePos(GPUClocks, "handheld_gpu"), "handheld_gpu");
        //HandheldGPU->setStateChangeListener(ChangeConfiguration);
        clkList->addItem(HandheldGPU);
        ValueListItems.push_back(HandheldGPU);

        ValueListItem *HandheldMEM = new ValueListItem("Handheld MEM Clock", MEMClocks, Utils::clk::getConfigValuePos(MEMClocks, "handheld_mem"), "handheld_mem");
        //HandheldMEM->setStateChangeListener(ChangeConfiguration);
        clkList->addItem(HandheldMEM);
        ValueListItems.push_back(HandheldMEM);

        rootFrame->addElement(clkList);

        return rootFrame;
    }
};

class SysClkOverlay : public tsl::Overlay
{
public:
    SysClkOverlay() {}
    ~SysClkOverlay() {}

    tsl::Gui *onSetup()
    {
        smInitialize();
        pmdmntInitialize();
        pmshellInitialize();
        nsInitialize();
        pminfoInitialize();
        smExit();
        return new GuiMain();
    } // Called once when the Overlay is created and should return the first Gui to load. Initialize services here

    virtual void onDestroy()
    {
        for (ValueListItem *item : ValueListItems)
            Utils::clk::ChangeConfiguration(item->getValues(), item->getCurValue(), item->getExtData());
            
        pmshellExit();
        pmdmntExit();
        nsExit();
        pminfoExit();

    } // Called once before the overlay Exits. Exit services here

    virtual  void onOverlayShow(tsl::Gui *gui)
    {
        if (toggleItem != nullptr)
            toggleItem->setState(Utils::clk::getClkState() == ClkState::Enabled);

        for (ValueListItem *item : ValueListItems)
            item->setCurValue(Utils::clk::getConfigValuePos(item->getValues(), item->getExtData()));

        tsl::Gui::playIntroAnimation();
    }

    void onOverlayHide(tsl::Gui *gui)
    {
        for (ValueListItem *item : ValueListItems)
            Utils::clk::ChangeConfiguration(item->getValues(), item->getCurValue(), item->getExtData());

        tsl::Gui::playOutroAnimation();
    }
};

// This function gets called on startup to create a new Overlay object
tsl::Overlay *overlayLoad()
{
    return new SysClkOverlay();
}