#include <valueListItem.h>

ValueListItem::ValueListItem(std::string text, const std::vector<std::string> values, int defaultPos, std::string data) : tsl::element::ListItem(text), m_values(values), m_curValue(defaultPos), extdata(data)
{
}

ValueListItem::~ValueListItem()
{
}

tsl::element::Element *ValueListItem::requestFocus(Element *oldFocus, FocusDirection direction)
{
    return ListItem::requestFocus(oldFocus, direction);
}

void ValueListItem::draw(tsl::Screen *screen, u16 x1, u16 y1)
{
    const auto [x, y] = this->getPosition();
    const auto [w, h] = this->getSize();

    ListItem::draw(screen, x1, y1);
    screen->drawString(m_values.at(m_curValue).c_str(), false, w - 8, y + 42, 19, a({0x5, 0xC, 0xA, 0xF}));
}

void ValueListItem::layout()
{
    ListItem::layout();
}

bool ValueListItem::onClick(s64 key)
{
    if (key & KEY_A)
    {
        m_curValue++;
        long int size = m_values.size();
        if (m_curValue < 0)
            m_curValue = size - 1;
        if (size <= m_curValue)
            m_curValue = 0;

        if (this->m_valueChangeListener != nullptr)
            this->m_valueChangeListener(this->m_values, this->m_curValue, this->extdata);

        return true;
    }

    return false;
}