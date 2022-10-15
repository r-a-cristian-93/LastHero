#ifndef WC_TEXT
#define WC_TEXT

#include "SFML/Graphics.hpp"
#include "Link.h"

class WCText: public sf::Drawable, public sf::Transformable {
private:
    sf::Text* m_text;
	Link* m_link;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	void setText(std::string text, sf::Font& font, unsigned int size);
	void setText(std::string text);
	void setText(sf::Text& text);
	void setColor(sf::Color color);
	void setPosition(sf::Vector2i p);

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