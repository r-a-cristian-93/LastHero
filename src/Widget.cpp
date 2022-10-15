#include "Widget.h"
#include <iostream>
#include "SharedResources.h"

Widget::Widget()
	:pos_rel(0, 0)
	,pos_abs(0, 0)
	,size(0, 0)
	,background(nullptr)
	,bg_offset(0,0)
	,border(nullptr)
	,text(nullptr)
	,link_int(nullptr)
	,link_str(nullptr)
	,link(0)
	,current_fx(nullptr)
	,scroll(ScrollType::NONE)
	,scroll_track(nullptr)
	,scroll_thumb(nullptr)
	,scroll_content_tex(nullptr)
	,scroll_content_sprite(nullptr)
	,scroll_pos()
	,state(State::DEFAULT)
	,on_click(0)
	,state_colors({})
	,m_pos(0,0)
	,m_anchor(Widget::Anchor::WINDOW)
	,m_origin(Widget::Origin::TOP_CENTER)
	{
		add<WCText>(nullptr);
	}

Widget::Widget(const Widget& w)
	:pos_rel(w.pos_rel)
	,pos_abs(w.pos_abs)
	,size(w.size)
	,background(nullptr)
	,border(nullptr)
	,text(nullptr)
	,link_int(w.link_int)
	,link_str(w.link_str)
	,link(w.link)
	,childs(w.childs)
	,fx(w.fx)
	,current_fx(nullptr)
	,scroll(w.scroll)
	,scroll_track(nullptr)
	,scroll_thumb(nullptr)
	,scroll_content_tex(nullptr)
	,scroll_content_sprite(nullptr)
	,scroll_pos(w.scroll_pos)
	,state(w.state)
	,on_click(w.on_click)
	,state_colors(w.state_colors)
	,m_pos(w.m_pos)
	,m_anchor(w.m_anchor)
	,m_origin(w.m_origin)
{
	if (w.background) setBackground(*w.background, w.bg_offset);
	if (w.border) setBorder(*w.border);
	if (w.text) setText(*w.text);
	if (w.scroll_track) addScrollTrack(*w.scroll_track);
	if (w.scroll_thumb) addScrollThumb(*w.scroll_thumb);

	if (w.get<WCText>()) {
		add<WCText>(new WCText(*w.get<WCText>()));
	}
}


Widget::~Widget() {
	delete background;
	delete border;
	delete text;
	delete scroll_content_sprite;
	delete scroll_content_tex;
	if (get<WCText>()) delete get<WCText>();
}

sf::FloatRect Widget::getGlobalBounds() {
	if (border) {
		return sf::FloatRect(pos_abs.x, pos_abs.y, size.x, size.y);
	}
	else if (background) {
		sf::FloatRect rect = background->getGlobalBounds();

		rect.left -= bg_offset.x;
		rect.top -= bg_offset.y;
		rect.width += bg_offset.x*2;
		rect.height += bg_offset.y*2;

		return rect;
	}

	else if (text) {
		return text->getGlobalBounds();
	}

	return sf::FloatRect();
}

void Widget::setPosRel(sf::Vector2i p) {
	pos_rel = p;

	if (!childs.empty()) {
		for (int i=0; i<childs.size(); i++) {
			updateChildPos(childs[i]);
		}
	}
}

void Widget::setPosAbs(sf::Vector2i p) {
	pos_abs = p + pos_rel;

	if (!childs.empty()) {
		for (int i=0; i<childs.size(); i++) {
			updateChildPos(childs[i]);
		}
	}

	if (background) background->setPosition(pos_abs.x + bg_offset.x, pos_abs.y + bg_offset.y);
	if (border) border->match(sf::IntRect(pos_abs.x, pos_abs.y, size.x, size.y));
	if (text) text->setPosition(pos_abs.x, pos_abs.y);

	if (get<WCText>() != nullptr) {
		get<WCText>()->setPosition(pos_abs);
	}
}

void Widget::setSize(sf::Vector2i s) {
	size = s;
}

void Widget::addChild(Widget& child) {
	updateChildPos(child);
	childs.push_back(child);
}

void Widget::addScrollThumb(Widget& thumb) {
	addChild(thumb);
	scroll_thumb = &childs.back();
}

void Widget::addScrollTrack(Widget& track) {
	addChild(track);
	scroll_track = &childs.back();
}

void Widget::updateChildPos(Widget& child) {
	child.setPosAbs(pos_abs);
}

std::vector<sf::Drawable*>& Widget::getDrawables() {
	return drawables;
}

std::vector<Widget>& Widget::getChilds() {
	return childs;
}

// BOX

void Widget::setBackground(sf::Texture& tex, int offset) {
	if (!background) {
		bg_offset = {offset, offset};
		background = new sf::Sprite(tex, sf::IntRect(0, 0, size.x-offset*2, size.y-offset*2));
		background->setPosition(pos_abs.x+bg_offset.x, pos_abs.y+bg_offset.y);
		drawables.push_back(background);
	}
}

void Widget::setBackground(sf::Sprite& sprite, sf::Vector2i offset) {
	if (!background) {
		bg_offset = offset;
		background = new sf::Sprite(sprite);
		background->setPosition(pos_abs.x+bg_offset.x, pos_abs.y+bg_offset.y);
		drawables.push_back(background);
	}
}

void Widget::setBackgroundColor(sf::Color color) {
	if (background) background->setColor(color);
}

void Widget::setBorder(Border& b) {
	if (!border) {
		border = new Border(b);

		border->match(sf::IntRect(pos_abs.x, pos_abs.y, size.x, size.y));

		for (sf::Sprite* sprite:border->getSprites()) {
			drawables.push_back(sprite);
		}
	}
}





// TEXT

void Widget::setText(std::string t, sf::Font& font, unsigned int size) {
	if (!text) {
		text = new sf::Text(t, font, size);
		//drawables.push_back(text);
		updateOrigin();
	}
}

void Widget::setText(std::string t) {
	if (text) {
		text->setString(t);
		updateOrigin();
	}
}

void Widget::setText(sf::Text& t) {
	delete text;

	text = new sf::Text(t);
	//drawables.push_back(text);
	updateOrigin();
}

void Widget::linkToInt(int& value) {
	link_int = & value;
}

void Widget::linkToStr(std::string& value) {
	link_str = &value;
}

void Widget::updateText() {
	if (link_int)  {
		setText(std::to_string(*link_int));
	}
	else if (link_str) {
		setText(*link_str);
	}

	updateOrigin();
}

void Widget::updateOrigin() {
	sf::FloatRect b = text->getLocalBounds();
	text->setOrigin(b.left + b.width/2, b.top + b.height/2);
}

void Widget::setTextColor(sf::Color color) {
	if (text) text->setFillColor(color);
}





// BOX and TEXT

void Widget::update() {
	if (on_click) setColor(state_colors[state]);

	if (text) {
		updateText();
		updateOrigin();
	}

	if (get<WCText>() != nullptr) {
		get<WCText>()->update();
	}

	if (current_fx) {
		WidgetFx& fx = *current_fx;

		int& current_frame = fx.data[WidgetFx::DataIndex::CURRENT_FRAME];
		const int& frames_in = fx.data[WidgetFx::DataIndex::FRAMES_IN];
		const int& frames_out = fx.data[WidgetFx::DataIndex::FRAMES_OUT];
		const int& opacity_hi = fx.data[WidgetFx::DataIndex::OPACITY_HI];
		const int& opacity_low = fx.data[WidgetFx::DataIndex::OPACITY_LOW];

		current_frame++;

		if (fx.type = WidgetFx::Type::FADE_IN_OUT) {
			sf::Color color = background->getColor();
			float alpha = 0;

			if (current_frame <= frames_in) {
				alpha =  (static_cast<float>(opacity_hi)/frames_in) * current_frame;
				color.a = static_cast<int>(alpha);
			}
			else {
				alpha = static_cast<float>(opacity_hi-opacity_low)/frames_out * ( frames_out - (current_frame - frames_in) );
				color.a = static_cast<int>(alpha);
			}

			if (current_frame > frames_in + frames_out) {
				current_frame = 0;
				current_fx = nullptr;
				color.a = opacity_low;
			}

			background->setColor(color);
		}
	}

	if (!childs.empty()) {
		for (int i=0; i<childs.size(); i++) {
			childs[i].update();
		}
	}
}

void Widget::setColor(sf::Color color) {
	if (text) text->setFillColor(color);
	if (background) background->setColor(color);
}
