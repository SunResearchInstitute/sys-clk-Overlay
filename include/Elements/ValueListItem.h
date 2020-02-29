#pragma once
#include <string>
#include <tesla.hpp>

class ValueListItem : public tsl::elm::ListItem
{
public:
    ValueListItem(std::string text, const std::vector<std::string> values, int defaultPos, const std::string data) : tsl::elm::ListItem(text), m_values(values), m_curValue(defaultPos), m_extdata(data) {}
    ~ValueListItem() {}

    virtual void draw(tsl::gfx::Renderer *renderer) override
    {
        if (this->m_valueWidth == 0)
        {
            auto [width, height] = renderer->drawString(this->m_values.at(this->m_curValue).c_str(), false, 0, 0, 20, tsl::style::color::ColorTransparent);
            this->m_valueWidth = width;
        }

        renderer->drawRect(this->getX(), this->getY(), this->getWidth(), 1, a({0x5, 0x5, 0x5, 0xF}));
        renderer->drawRect(this->getX(), this->getY() + this->getHeight(), this->getWidth(), 1, a({0x5, 0x5, 0x5, 0xF}));

        renderer->drawString(this->m_text.c_str(), false, this->getX() + 20, this->getY() + 45, 23, a({0xF, 0xF, 0xF, 0xF}));
        renderer->drawString(this->m_values.at(m_curValue).c_str(), false, this->getX() + this->getWidth() - this->m_valueWidth - 20, this->getY() + 45, 20, this->m_faint ? a({0x6, 0x6, 0x6, 0xF}) : a({0x5, 0xC, 0xA, 0xF}));
    }

    virtual bool onClick(u64 keys) override
    {
        if (keys)
        {
            if (keys & KEY_A)
                m_curValue++;
            else if (keys & KEY_X)
                m_curValue--;

            long int size = m_values.size();
            if (m_curValue < 0)
                m_curValue = size - 1;
            if (size <= m_curValue)
                m_curValue = 0;

            return true;
        }
        return false;
    }

    virtual inline void setValue(int curValue, bool faint = false) final
    {
        this->m_curValue = curValue;
        this->m_faint = faint;
        this->m_valueWidth = 0;
    }

    int getCurValue() { return this->m_curValue; }
    void setCurValue(int value) { this->m_curValue = value; }

    const std::string getExtData() { return this->m_extdata; }

    const std::vector<std::string> getValues() { return this->m_values; }

protected:
    const std::vector<std::string> m_values;
    int m_curValue;
    const std::string m_extdata;
};
