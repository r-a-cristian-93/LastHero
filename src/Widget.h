#ifndef WIDGET
#define	WIDGET

#include "SFML/Graphics.hpp"
#include "Border.h"
#include "CommonBehaviour.h"
#include "WCText.h"

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
	enum class ScrollType: size_t {
		NONE = 0,
		HORIZONTAL,
		VERTICAL,
	};

	enum State: size_t {
		DEFAULT = 0,
		HOVER,
		FOCUS,
		ACTIVE,
		DISABLED,
		COUNT,
	};

	std::tuple<WCText*> components;

	template<class C>
	void add(C* c) {
		std::get<C*>(components) = c;
	}

	template<class C>
	C* get() const {
		return std::get<C*>(components);
	}

	std::vector<Widget> childs;

	// position relative to parent, values [0, 1].
	// a parent can be another Widget or the application window.
	sf::Vector2f pos;

	sf::Vector2i pos_rel;
	sf::Vector2i pos_abs;
	sf::Vector2i size;
	std::vector<sf::Drawable*> drawables;
	std::vector<WidgetFx> fx;
	WidgetFx* current_fx;
	size_t on_click;
	State state;
	sf::Color state_colors[State::COUNT];

	void updateChildPos(Widget& child);

//box
	sf::Sprite* background;
	sf::Vector2i bg_offset;
	Border* border;
//box

//scroll
	ScrollType scroll;
	Widget* scroll_track;
	Widget* scroll_thumb;
	sf::RenderTexture* scroll_content_tex;
	sf::Sprite* scroll_content_sprite;
	float scroll_pos; // percentage
//scroll


public:
	void setOrigin(sf::Vector2f origin);
	void setPosition(sf::Vector2f pos);
	void setPosRel(sf::Vector2i pos);
	void setPosAbs(sf::Vector2i pos);
	void setSize(sf::Vector2i s);
	void setColor(sf::Color color);

	void addChild(Widget& child);
	void addScrollThumb(Widget& thumb);
	void addScrollTrack(Widget& track);
	std::vector<sf::Drawable*>& getDrawables();
	std::vector<Widget>& getChilds();

	void update();
	sf::FloatRect getGlobalBounds();

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
		LINK_SECONDARY_ROUNDS,
		LINK_SECONDARY_ROUNDS_CURRENT,
		LINK_WINDOW_RESOLUTION,
		LINK_WINDOW_STYLE,
		LINK_MUSIC_VOLUME,
		LINK_SFX_VOLUME,
		LINK_COUNT,
	};

	void setText(std::string t, sf::Font& font, unsigned int size);
	void setText(std::string t);
	void setText(sf::Text& t);
	void setTextColor(sf::Color color);
	void updateText();
	void updateOrigin();
//text

	Widget();
	Widget(const Widget& w);
	~Widget();
};

#endif
