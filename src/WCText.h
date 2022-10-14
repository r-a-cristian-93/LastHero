#ifndef WC_TEXT
#define WC_TEXT

#include "SFML/Graphics.hpp"

class Link {
public:
	enum class Target: unsigned char {
		NONE = 0,
		PLAYER_HP,
		BASE_HP,
		TOTAL_KILLS,
		SECONDARY_ROUNDS,
		SECONDARY_ROUNDS_CURRENT,
		WINDOW_RESOLUTION,
		WINDOW_STYLE,
		MUSIC_VOLUME,
		SFX_VOLUME,
		COUNT,
	};

    virtual std::string getString() = 0;

protected:
    Link::Target target = Link::Target::NONE;
};

class LinkString: public Link {
private:
    const std::string* data;
    LinkString() = delete;

public:
    LinkString(Link::Target _target) {
        target = _target;
    }

    void setData(std::string* _data) {
        data = _data;
    }

    std::string getString() override {
        if (data != nullptr) return *data;
        else return "";
    }
};

class LinkInt: public Link {
private:
    const int* data;
    LinkInt() = delete;

public:
    LinkInt(Link::Target _target) {
        target = _target;
    }

    void setData(int* _data) {
        data = _data;
    }

    std::string getString() override {
        if (data != nullptr) return std::to_string(*data);
        else return "";
    }
};


class WCText: public sf::Drawable, public sf::Transformable {
private:
    sf::Text* text;
	Link* m_link;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	void setText(std::string text, sf::Font& font, unsigned int size);
	void setText(std::string text);
	void setText(sf::Text& text);
	void setColor(sf::Color color);
	void link(Link* link);
	void updateText();
	void updateOrigin();

    ~WCText();
};

#endif