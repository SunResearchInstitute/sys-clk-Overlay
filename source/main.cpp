#define TESLA_INIT_IMPL
#include "main.h"

tsl::elm::ToggleListItem *toggleItem;
std::vector<ValueListItem *> ValueListItems;
u64 programId;

class GuiMain : public tsl::Gui
{
public:
    virtual tsl::elm::Element *createUI() override
    {
        tsl::elm::OverlayFrame *rootFrame = new tsl::elm::OverlayFrame("sys-clk Overlay", "");
        ClkState state = Utils::clk::getClkState();

        if ((int)state < 0)
        {
            tsl::elm::CustomDrawer *warning = new tsl::elm::CustomDrawer([](tsl::gfx::Renderer *render, u16 x, u16 y, u16 w, u16 h) {
                render->drawString("\uE150", false, 180, 250, 90, a(0xFFFF));
                render->drawString("Could not load sys-clk!", false, 110, 340, 25, a(0xFFFF));
            });

            rootFrame->setContent(warning);
            return rootFrame;
        }

        programId = Utils::clk::getCurrentProgramId();
        tsl::elm::List *clkList = new tsl::elm::List();

        toggleItem = new tsl::elm::ToggleListItem("sys-clk", state == ClkState::Enabled);
        toggleItem->setStateChangedListener(Utils::clk::ToggleClkModule);
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

        rootFrame->setContent(clkList);

        return rootFrame;
    }
};

class SysClkOverlay : public tsl::Overlay
{
public:
    virtual void initServices() override
    {
        pmshellInitialize();
        nsInitialize();
        pminfoInitialize();
    }

    virtual void exitServices() override
    {
        for (ValueListItem *item : ValueListItems)
            Utils::clk::ChangeConfiguration(item->getValues(), item->getCurValue(), item->getExtData());

        pmshellExit();
        nsExit();
        pminfoExit();
    }

    virtual void onShow() override
    {
        if (programId != Utils::clk::getCurrentProgramId())
        {
            this->close();
            return;
        }

        if (toggleItem != nullptr)
            toggleItem->setState(Utils::clk::getClkState() == ClkState::Enabled);

        for (ValueListItem *item : ValueListItems)
            item->setCurValue(Utils::clk::getConfigValuePos(item->getValues(), item->getExtData()));
    }

    virtual void onHide() override
    {
        //Utils::clk::ToggleClkModule(toggleItem->getState());

        for (ValueListItem *item : ValueListItems)
            Utils::clk::ChangeConfiguration(item->getValues(), item->getCurValue(), item->getExtData());
    }

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override { return initially<GuiMain>(); }
};

int main(int argc, char **argv)
{
    return tsl::loop<SysClkOverlay>(argc, argv);
}