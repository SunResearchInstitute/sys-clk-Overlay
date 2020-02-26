#include <valueListItem.h>

ValueListItem::ValueListItem(std::string text, const std::vector<std::string> values, int defaultPos, const std::string data) : tsl::elm::ListItem(text), m_values(values), m_curValue(defaultPos), extdata(data) {}

ValueListItem::~ValueListItem() {}

tsl::elm::Element *ValueListItem::requestFocus(Element *oldFocus, tsl::FocusDirection direction)
{
    return this;
}

void ValueListItem::draw(tsl::gfx::Renderer *renderer)
{
    if (this->m_valueWidth == 0)
    {
        auto [width, height] = renderer->drawString(m_values.at(m_curValue).c_str(), false, 0, 0, 20, tsl::style::color::ColorTransparent);
        this->m_valueWidth = width;
    }

    renderer->drawRect(this->getX(), this->getY(), this->getWidth(), 1, a({0x5, 0x5, 0x5, 0xF}));
    renderer->drawRect(this->getX(), this->getY() + this->getHeight(), this->getWidth(), 1, a({0x5, 0x5, 0x5, 0xF}));

    renderer->drawString(m_values.at(m_curValue).c_str(), false, this->getX() + 20, this->getY() + 45, 23, a({0xF, 0xF, 0xF, 0xF}));
    renderer->drawString(m_values.at(m_curValue).c_str(), false, this->getX() + this->getWidth() - this->m_valueWidth - 20, this->getY() + 45, 20, this->m_faint ? a({0x6, 0x6, 0x6, 0xF}) : a({0x5, 0xC, 0xA, 0xF}));
}

bool ValueListItem::onClick(u64 key)
{
    if (key & KEY_A)
    {
        m_curValue++;
        long int size = m_values.size();
        if (m_curValue < 0)
            m_curValue = size - 1;
        if (size <= m_curValue)
            m_curValue = 0;

        return true;
    }

    return false;
}