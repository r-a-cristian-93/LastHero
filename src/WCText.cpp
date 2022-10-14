#include "WCText.h"
#include <sstream>

WCText::WCText()
    :m_text(nullptr)
    {}

void WCText::setText(std::string t, sf::Font& font, unsigned int size) {
    delete m_text;
	m_text = new sf::Text(t, font, size);
}

void WCText::setText(std::string t) {
	if (m_text != nullptr) {
		m_text->setString(t);
	}
}

void WCText::setText(sf::Text& t) {
	delete m_text;
	m_text = new sf::Text(t);
}

void WCText::link(Link* _link) {
    m_link = _link;
}

void WCText::updateOrigin() {
	sf::FloatRect b = m_text->getLocalBounds();
	m_text->setOrigin(b.left + b.width/2, b.top + b.height/2);
}

void WCText::setColor(sf::Color color) {
	if (m_text) m_text->setFillColor(color);
}

void WCText::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // first update link


	states.transform *= getTransform();
	target.draw(*m_text, states);
}