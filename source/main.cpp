#define TESLA_INIT_IMPL
#include "main.h"

tsl::elm::ToggleListItem *ToggleClkItem;
std::vector<ValueListItem *> ConfigItems;
u64 CurProgramId;

class GuiMain : public tsl::Gui
{
public:
    virtual tsl::elm::Element *createUI() override
    {
        CurProgramId = Utils::clk::getCurrentProgramId();
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
        tsl::elm::List *clkList = new tsl::elm::List();

        ToggleClkItem = new tsl::elm::ToggleListItem("sys-clk", state == ClkState::Enabled);
        ToggleClkItem->setStateChangedListener(Utils::clk::ToggleClkModule);
        clkList->addItem(ToggleClkItem);

        ValueListItem *DockedCPU = new ValueListItem("Docked CPU Clock", CPUClocks, Utils::clk::getConfigValuePos(CPUClocks, "docked_cpu"), "docked_cpu");
        clkList->addItem(DockedCPU);
        ConfigItems.push_back(DockedCPU);

        ValueListItem *DockedGPU = new ValueListItem("Docked GPU Clock", GPUClocks, Utils::clk::getConfigValuePos(GPUClocks, "docked_gpu"), "docked_gpu");
        clkList->addItem(DockedGPU);
        ConfigItems.push_back(DockedGPU);

        ValueListItem *DockedMEM = new ValueListItem("Docked MEM Clock", MEMClocks, Utils::clk::getConfigValuePos(MEMClocks, "docked_mem"), "docked_mem");
        clkList->addItem(DockedMEM);
        ConfigItems.push_back(DockedMEM);

        ValueListItem *HandheldCPU = new ValueListItem("Handheld CPU Clock", CPUClocks, Utils::clk::getConfigValuePos(CPUClocks, "handheld_cpu"), "handheld_cpu");
        clkList->addItem(HandheldCPU);
        ConfigItems.push_back(HandheldCPU);

        ValueListItem *HandheldGPU = new ValueListItem("Handheld GPU Clock", GPUClocks, Utils::clk::getConfigValuePos(GPUClocks, "handheld_gpu"), "handheld_gpu");
        clkList->addItem(HandheldGPU);
        ConfigItems.push_back(HandheldGPU);

        ValueListItem *HandheldMEM = new ValueListItem("Handheld MEM Clock", MEMClocks, Utils::clk::getConfigValuePos(MEMClocks, "handheld_mem"), "handheld_mem");
        clkList->addItem(HandheldMEM);
        ConfigItems.push_back(HandheldMEM);

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
        for (ValueListItem *configItem : ConfigItems)
            if (configItem != nullptr)
                Utils::clk::ChangeConfiguration(configItem);

        pmshellExit();
        nsExit();
        pminfoExit();
    }

    virtual void onShow() override
    {
        if (CurProgramId != Utils::clk::getCurrentProgramId())
        {
            this->close();
            return;
        }

        if (ToggleClkItem != nullptr)
            ToggleClkItem->setState(Utils::clk::getClkState() == ClkState::Enabled);

        for (ValueListItem *configItem : ConfigItems)
            if (configItem != nullptr)
                configItem->setCurValue(Utils::clk::getConfigValuePos(configItem->getValues(), configItem->getExtData()));
    }

    virtual void onHide() override
    {
        for (ValueListItem *configItem : ConfigItems)
            if (configItem != nullptr)
                Utils::clk::ChangeConfiguration(configItem);
    }

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override { return initially<GuiMain>(); }
};

int main(int argc, char **argv)
{
    return tsl::loop<SysClkOverlay>(argc, argv);
}