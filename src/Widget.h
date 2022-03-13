#ifndef WIDGET
#define	WIDGET

#include "SFML/Graphics.hpp"

class Widget {
protected:
	std::vector<Widget*> childs;

	sf::Vector2i pos_rel;
	sf::Vector2i pos_abs;
	sf::Vector2i size;
	std::vector<sf::Drawable*> drawables;

	void updateChildPos(Widget& child);

public:
	void setPosRel(sf::Vector2i pos);
	virtual void setPosAbs(sf::Vector2i pos) = 0;
	void setSize(sf::Vector2i s);

	void addChild(Widget* child);
	virtual std::vector<sf::Drawable*>& getDrawables();
	std::vector<Widget*>& getChilds();

	Widget();
	~Widget();
};

#endif
