#ifndef WIDGET
#define	WIDGET

#include "SFML/Graphics.hpp"
#include "Box.h"
#include "CommonBehaviour.h"
#include "WCText.h"
#include "WCBox.h"
#include "WCImage.h"
#include "Action.h"

struct WidgetFx {
	//fx type
	enum Type: int {
		NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_IN_OUT,
	};

	//data index
	enum DataIndex: int {
		CURRENT_FRAME = 0,
		FRAMES_IN,
		FRAMES_OUT,
		OPACITY_HI,
		OPACITY_LOW,
		COUNT
	};

	BCondition cond;
	Type type = Type::NONE;
	int data[DataIndex::COUNT] = {0};
};


class Widget: public sf::Drawable, public sf::Transformable {
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	enum State: size_t {
		DEFAULT = 0,
		HOVER,
		FOCUS,
		ACTIVE,
		DISABLED,
		COUNT,
	};

	std::tuple<WCText*, WCBox*, WCImage*> components;

	template<class C>
	void add(C* c) {
		std::get<C*>(components) = c;
	}

	template<class C>
	C* get() const {
		return std::get<C*>(components);
	}

	std::vector<Widget> childs;

	sf::Vector2i pos_rel;
	sf::Vector2i pos_abs;
	sf::Vector2i size;
	std::vector<WidgetFx> fx;
	WidgetFx* current_fx;
	State state;
	sf::Color state_colors[State::COUNT];

	void updateChildPos(Widget& child);

public:
	void setPosRel(sf::Vector2i pos);
	void setPosAbs(sf::Vector2i pos);
	void setSize(sf::Vector2i s);
	void setColor(sf::Color color);

	void addChild(Widget& child);
	std::vector<Widget>& getChilds();

	void update();
	void update(sf::Vector2f parent_size, sf::Vector2f parent_pos);			//should be private

	sf::FloatRect getGlobalBounds();
	sf::FloatRect getLocalBounds();
	sf::Vector2f getPosition();
	sf::Vector2i getSize();


//text
	void setText(std::string t, sf::Font& font, unsigned int size);
	void setText(std::string t);
	void setText(sf::Text& t);
	void setTextColor(sf::Color color);
	void updateOrigin();
//text

	Widget();
	Widget(const Widget& w);
	~Widget();
};

#endif
