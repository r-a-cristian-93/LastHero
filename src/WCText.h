#ifndef WC_TEXT
#define WC_TEXT

#include "SFML/Graphics.hpp"
#include "Link.h"

class WCText: public sf::Text {
private:
	Link* m_link;

public:
	void setText(std::string text, sf::Font& font, unsigned int size);
	void setText(std::string text);
	void setText(sf::Text& text);

	void setLink(Link* link);
    bool hasLink();
    Link& getLink();

	void update();
	void updateOrigin();

    WCText();
    WCText(const WCText& wct);
    ~WCText();
};

#endif