#include "WCText.h"

WCText::WCText()
    :m_text(nullptr)
    {}
WCText::WCText(const WCText& wct)
	:m_text(new sf::Text(*wct.m_text))

	{
		m_link = wct.m_link->clone();
	}

void WCText::setText(std::string t, sf::Font& font, unsigned int size) {
    delete m_text;
	m_text = new sf::Text(t, font, size);
	updateOrigin();
}

void WCText::setText(std::string t) {
	if (m_text != nullptr) {
		if (m_text->getString() != t) {
			m_text->setString(t);
			updateOrigin();
		}
	}
}

void WCText::setText(sf::Text& t) {
	delete m_text;
	m_text = new sf::Text(t);
	updateOrigin();
}

void WCText::setColor(sf::Color color) {
	if (m_text) m_text->setFillColor(color);
}

void WCText::setPosition(sf::Vector2i p) {
	if (m_text != nullptr) {
		m_text->setPosition(p.x+10, p.y+10);
	}
}

void WCText::setLink(Link* _link) {
    m_link = _link;
}

bool WCText::hasLink() {
	if (m_link != nullptr) {
		return true;
	}
	else {
		return false;
	}
}

Link& WCText::getLink() {
	return *m_link;
}

void WCText::updateOrigin() {
	sf::FloatRect b = m_text->getLocalBounds();
	m_text->setOrigin(b.left + b.width/2, b.top + b.height/2);
}

void WCText::update() {
	if (m_link != nullptr) {
		this->setText(m_link->getString());
	}
}

void WCText::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(*m_text, states);
}

WCText::~WCText() {
	delete m_text;
	delete m_link;
}