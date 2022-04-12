#ifndef WIDGET
#define	WIDGET

#include "SFML/Graphics.hpp"
#include "Border.h"

class Widget {
protected:
	std::vector<Widget*> childs;

	sf::Vector2i pos_rel;
	sf::Vector2i pos_abs;
	sf::Vector2i size;
	std::vector<sf::Drawable*> drawables;

	void updateChildPos(Widget& child);



//box
	sf::Sprite* background;
	sf::Vector2i bg_offset;
	Border* border;
//box

//text
	size_t link_text;
	int* link_int;
	sf::Text* text;
//text

public:
	void setPosRel(sf::Vector2i pos);
	void setPosAbs(sf::Vector2i pos);
	void setSize(sf::Vector2i s);
	void setColor(sf::Color color);

	void addChild(Widget* child);
	std::vector<sf::Drawable*>& getDrawables();
	std::vector<Widget*>& getChilds();

	void update();


// box
	void setBackground(sf::Texture& tex, int offset);
	void setBackground(sf::Sprite& sprite, sf::Vector2i offset);
	void setBorder(Border& b);
	void setBackgroundColor(sf::Color color);
// box

//text
	enum {
		LINK_NONE,
		LINK_PLAYER_HP,
		LINK_BASE_HP,
		LINK_TOTAL_KILLS,
	};

	void setText(std::string t, sf::Font& font, unsigned int size);
	void setText(std::string t);
	void setTextColor(sf::Color color);
	void linkToInt(int& value);
	void updateText();
	void updateOrigin();
//text

	Widget();
	~Widget();

	// be careful with this one! Do not use it to delete widgets from Assets"
	void clearData();
};

#endif
