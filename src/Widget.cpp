#include "Widget.h"
#include <iostream>

Widget::Widget()
	:pos_rel(0, 0)
	,pos_abs(0, 0)
	,size(0, 0)
	,box(nullptr)
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
	{
		add<WCText>(nullptr);
	}

Widget::Widget(const Widget& w)
	:pos_rel(w.pos_rel)
	,pos_abs(w.pos_abs)
	,m_origin(w.m_origin)
	,m_position(w.m_position)
	,size(w.size)
	,box(nullptr)
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
{
	if (w.box) setBorder(*w.box);
	if (w.scroll_track) addScrollTrack(*w.scroll_track);
	if (w.scroll_thumb) addScrollThumb(*w.scroll_thumb);

	if (w.get<WCImage>()) {
		add<WCImage>(new WCImage(*w.get<WCImage>()));
	}
	if (w.get<WCText>()) {
		add<WCText>(new WCText(*w.get<WCText>()));
	}
	if (w.get<WCBox>()) {
		add<WCBox>(new WCBox(*w.get<WCBox>()));
	}
}


Widget::~Widget() {
	delete box;
	delete scroll_content_sprite;
	delete scroll_content_tex;
	if (get<WCText>()) delete get<WCText>();
	if (get<WCBox>()) delete get<WCBox>();
}

sf::FloatRect Widget::getGlobalBounds() {
	if (box) {
		return sf::FloatRect(pos_abs.x, pos_abs.y, size.x, size.y);
	}
	else if (get<WCText>() != nullptr) {
		return get<WCText>()->getGlobalBounds();
	}

	return sf::FloatRect();
}

sf::FloatRect Widget::getLocalBounds() {
	if (get<WCImage>() != nullptr) {
		return get<WCImage>()->getLocalBounds();
	}
	else {
		return {0.0f, 0.0f, 0.0f, 0.0f};
	}
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


	if (box) box->match(sf::IntRect(pos_abs.x, pos_abs.y, size.x, size.y));

	if (get<WCBox>() != nullptr) {
		get<WCBox>()->setPosition(pos_abs.x + bg_offset.x, pos_abs.y + bg_offset.y);
	}

	if (get<WCText>() != nullptr) {
		get<WCText>()->setPosition(pos_abs.x, pos_abs.y);
	}
}

void Widget::setSize(sf::Vector2i s) {
	size = s;

	if (get<WCBox>() != nullptr) {
		get<WCBox>()->setSize(s);
	}
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
	bg_offset = {offset, offset};

	if (get<WCBox>() == nullptr) {
		add<WCBox>(new WCBox());
		get<WCBox>()->setBackground(tex, offset);
		get<WCBox>()->setPosition(pos_abs.x+bg_offset.x, pos_abs.y+bg_offset.y);
		get<WCBox>()->setSize(size);
	}
}

void Widget::setBackground(sf::Sprite& sprite, sf::Vector2i offset) {
	bg_offset = offset;

	if(get<WCBox>() == nullptr) {
		add<WCBox>(new WCBox());
		get<WCBox>()->setBackground(sprite, offset);
		get<WCBox>()->setPosition(pos_abs.x+bg_offset.x, pos_abs.y+bg_offset.y);
	}
}

void Widget::setBackgroundColor(sf::Color color) {
	if (get<WCBox>() != nullptr) {
		get<WCBox>()->setBackgroundColor(color);
	}
}

void Widget::setBorder(Box& b) {
	if (!box) {
		box = new Box(b);

		box->match(sf::IntRect(pos_abs.x, pos_abs.y, size.x, size.y));

		for (sf::Sprite* sprite:box->getSprites()) {
			drawables.push_back(sprite);
		}
	}
}





// TEXT
void Widget::updateOrigin() {
	if (get<WCText>() !=nullptr) {
		sf::FloatRect b = get<WCText>()->getLocalBounds();
		get<WCText>()->setOrigin(b.left + b.width/2, b.top + b.height/2);
	}
}

void Widget::setText(std::string t, sf::Font& font, unsigned int size) {
	if (get<WCText>() != nullptr) {
		get<WCText>()->setText(t, font, size);
	}
	else {
		WCText* wct = new WCText();
		wct->setText(t, font, size);
		add<WCText>(wct);
	}

	updateOrigin();
}



// BOX and TEXT


void Widget::update(sf::Vector2f parent_size, sf::Vector2f parent_pos) {
	if (get<WCImage>() != nullptr) {
		WCImage& wci = *get<WCImage>();
		sf::FloatRect bounds = wci.getLocalBounds();
		wci.setOrigin(bounds.width * m_origin.x, bounds.height * m_origin.y);
		wci.setPosition(parent_pos.x + parent_size.x * m_position.x, parent_pos.y + parent_size.y * m_position.y);
	}
}

void Widget::updatePosition(sf::Vector2f parent_size, sf::Vector2f parent_pos) {
	update (parent_size, parent_pos);

	for (Widget& w : getChilds()) {
		sf::FloatRect bounds = getLocalBounds();
		w.updatePosition({bounds.width, bounds.height}, m_position);
	}
}



void Widget::update() {
	if (on_click) setColor(state_colors[state]);

	if (get<WCText>() != nullptr) {
		get<WCText>()->update();
		updateOrigin();
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
			//sf::Color color = background->getColor();
			sf::Color color;
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

			//background->setColor(color);
		}
	}

	if (!childs.empty()) {
		for (int i=0; i<childs.size(); i++) {
			childs[i].update();
		}
	}
}

void Widget::setColor(sf::Color color) {
	if (get<WCBox>() != nullptr) {
		get<WCBox>()->setBackgroundColor(color);
	}
	if (get<WCText>() != nullptr) {
		get<WCText>()->setFillColor(color);
	}
}

void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (get<WCImage>() != nullptr) {
		target.draw(*get<WCImage>(), states);
	}
	if (get<WCText>() != nullptr) {
		target.draw(*get<WCText>(), states);
	}
	if (get<WCBox>() != nullptr) {
		//target.draw(*get<WCBox>(), states);
	}
}