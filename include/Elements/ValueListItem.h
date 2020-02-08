#pragma once

#include <overlay/elements/element.hpp>
#include <overlay/elements/list_item.hpp>

#include <string>
#include <functional>

class ValueListItem : public tsl::element::ListItem
{
public:
    ValueListItem(std::string text, const std::vector<std::string> values, int defaultPos, std::string extData);
    ~ValueListItem();

    tsl::element::Element *requestFocus(Element *oldFocus, FocusDirection direction) override;

    void draw(tsl::Screen *screen, u16 x, u16 y) override;
    void layout() override;

    int getValue() { return this->m_curValue; }
    void setValue(int value) { this->m_curValue = value; }

    std::string getExtData() { return this->extdata; }
    void setExtData(std::string data) { this->extdata = data; }

    bool onClick(s64 key) override;

    void setStateChangeListener(std::function<void(const std::vector<std::string>, int, std::string)> valueChangeListener) { this->m_valueChangeListener = valueChangeListener; }

private:
    const std::vector<std::string> m_values;
    int m_curValue;
    std::function<void(const std::vector<std::string>, int, std::string)> m_valueChangeListener = nullptr;
    std::string extdata;
};
