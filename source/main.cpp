#define TESLA_INIT_IMPL
#include "main.h"

tsl::elm::ToggleListItem *ToggleClkItem;
std::vector<ClkConfigListItem *> ConfigItems;

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

        tsl::elm::List *clkList = new tsl::elm::List();

        ToggleClkItem = new tsl::elm::ToggleListItem("sys-clk", state == ClkState::Enabled);
        ToggleClkItem->setStateChangedListener(Utils::clk::ToggleClkModule);
        clkList->addItem(ToggleClkItem);

        ClkConfigListItem *DockedCPU = new ClkConfigListItem("Docked CPU Clock", CPUClocks, "docked_cpu");
        clkList->addItem(DockedCPU);
        ConfigItems.push_back(DockedCPU);

        ClkConfigListItem *DockedGPU = new ClkConfigListItem("Docked GPU Clock", GPUClocks, "docked_gpu");
        clkList->addItem(DockedGPU);
        ConfigItems.push_back(DockedGPU);

        ClkConfigListItem *DockedMEM = new ClkConfigListItem("Docked MEM Clock", MEMClocks, "docked_mem");
        clkList->addItem(DockedMEM);
        ConfigItems.push_back(DockedMEM);

        ClkConfigListItem *HandheldCPU = new ClkConfigListItem("Handheld CPU Clock", CPUClocks, "handheld_cpu");
        clkList->addItem(HandheldCPU);
        ConfigItems.push_back(HandheldCPU);

        ClkConfigListItem *HandheldGPU = new ClkConfigListItem("Handheld GPU Clock", GPUClocks, "handheld_gpu");
        clkList->addItem(HandheldGPU);
        ConfigItems.push_back(HandheldGPU);

        ClkConfigListItem *HandheldMEM = new ClkConfigListItem("Handheld MEM Clock", MEMClocks, "handheld_mem");
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
        R_ASSERT(pmshellInitialize());
        R_ASSERT(nsInitialize());
        R_ASSERT(pminfoInitialize());
    }

    virtual void exitServices() override
    {
        //Config will be saved in destructor of ClkConfigListemItem
        pmshellExit();
        nsExit();
        pminfoExit();
    }

    virtual void onShow() override
    {
        if (ToggleClkItem != nullptr)
            ToggleClkItem->setState(Utils::clk::getClkState() == ClkState::Enabled);

        for (ClkConfigListItem *configItem : ConfigItems)
            if (configItem != nullptr)
                configItem->setConfigCurValue();
    }

    virtual void onHide() override
    {
        ClkState state = Utils::clk::getClkState();
        if ((int)state < 0)
        {
            this->close();
            return;
        }

        for (ClkConfigListItem *configItem : ConfigItems)
            if (configItem != nullptr)
                Utils::clk::ChangeConfiguration(configItem);
    }

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override { return initially<GuiMain>(); }
};

int main(int argc, char **argv)
{
    return tsl::loop<SysClkOverlay>(argc, argv);
}