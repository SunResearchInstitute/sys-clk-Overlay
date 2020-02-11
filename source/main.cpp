#include "main.h"

tsl::element::ToggleListItem *toggleItem;
std::vector<ValueListItem *> ValueListItems;
u64 programId;

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

        programId = Utils::clk::getCurrentProgramId();
        this->setSubtitle(Utils::clk::getProgramName(programId));
        tsl::element::List *clkList = new tsl::element::List();

        toggleItem = new tsl::element::ToggleListItem("sys-clk", state == ClkState::Enabled);
        toggleItem->setStateChangeListener(Utils::clk::EnableClkModule);
        clkList->addItem(toggleItem);

        ValueListItem *DockedCPU = new ValueListItem("Docked CPU Clock", CPUClocks, Utils::clk::getConfigValuePos(CPUClocks, "docked_cpu"), "docked_cpu");
        clkList->addItem(DockedCPU);
        ValueListItems.push_back(DockedCPU);

        ValueListItem *DockedGPU = new ValueListItem("Docked GPU Clock", GPUClocks, Utils::clk::getConfigValuePos(GPUClocks, "docked_gpu"), "docked_gpu");
        clkList->addItem(DockedGPU);
        ValueListItems.push_back(DockedGPU);

        ValueListItem *DockedMEM = new ValueListItem("Docked MEM Clock", MEMClocks, Utils::clk::getConfigValuePos(MEMClocks, "docked_mem"), "docked_mem");
        clkList->addItem(DockedMEM);
        ValueListItems.push_back(DockedMEM);

        ValueListItem *HandheldCPU = new ValueListItem("Handheld CPU Clock", CPUClocks, Utils::clk::getConfigValuePos(CPUClocks, "handheld_cpu"), "handheld_cpu");
        clkList->addItem(HandheldCPU);
        ValueListItems.push_back(HandheldCPU);

        ValueListItem *HandheldGPU = new ValueListItem("Handheld GPU Clock", GPUClocks, Utils::clk::getConfigValuePos(GPUClocks, "handheld_gpu"), "handheld_gpu");
        clkList->addItem(HandheldGPU);
        ValueListItems.push_back(HandheldGPU);

        ValueListItem *HandheldMEM = new ValueListItem("Handheld MEM Clock", MEMClocks, Utils::clk::getConfigValuePos(MEMClocks, "handheld_mem"), "handheld_mem");
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
        smExit();

    } // Called once before the overlay Exits. Exit services here

    virtual void onOverlayShow(tsl::Gui *gui)
    {
        if (programId != Utils::clk::getCurrentProgramId())
        {
            gui->closeGui();
            gui->playIntroAnimation();
            return;
        }

        if (toggleItem != nullptr)
            toggleItem->setState(Utils::clk::getClkState() == ClkState::Enabled);

        for (ValueListItem *item : ValueListItems)
            item->setCurValue(Utils::clk::getConfigValuePos(item->getValues(), item->getExtData()));

        gui->playIntroAnimation();
    }

    void onOverlayHide(tsl::Gui *gui)
    {
        for (ValueListItem *item : ValueListItems)
            Utils::clk::ChangeConfiguration(item->getValues(), item->getCurValue(), item->getExtData());

        gui->playOutroAnimation();
    }
};

// This function gets called on startup to create a new Overlay object
tsl::Overlay *overlayLoad()
{
    return new SysClkOverlay();
}