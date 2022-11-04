#include "WCText.h"
#include <iostream>

WCText::WCText():
	m_link(nullptr)
    {}

WCText::WCText(const WCText& wct):
	m_link(nullptr)
{
	std::cout << "COPY WCT\n";

	this->setFont(*wct.getFont());
	this->setCharacterSize(wct.getCharacterSize());
	this->setString(wct.getString());
	this->setPosition(wct.getPosition());

	if (wct.m_link != nullptr) {
		setLink(wct.m_link->clone());
	}
}

void WCText::setText(std::string t, sf::Font& font, unsigned int size) {
	this->setFont(font);
	this->setCharacterSize(size);
	this->setString(t);
}

void WCText::setText(std::string t) {
	if (this->getString() != t) {
		this->setString(t);
	}
}

void WCText::setText(sf::Text& t) {
	this->setFont(*t.getFont());
	this->setCharacterSize(t.getCharacterSize());

	if (this->getString() != t.getString()) {
		this->setString(t.getString());
	}
}

void WCText::setColor(sf::Color color) {
	this->setFillColor(color);
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
	sf::FloatRect b = this->getLocalBounds();
	this->setOrigin(b.left + b.width/2, b.top + b.height/2);
}

void WCText::update() {
	if (m_link != nullptr) {
		this->setText(m_link->getString());
	}
}

WCText::~WCText() {
	delete m_link;
}