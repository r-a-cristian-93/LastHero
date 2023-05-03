#include "Widget.h"
#include <iostream>

Widget::Widget()
	:pos_rel(0, 0)
	,pos_abs(0, 0)
	,size(0, 0)
	,current_fx(nullptr)
	,state(State::DEFAULT)
	,state_colors({})
	{
		add<WCText>(nullptr);
	}

Widget::Widget(const Widget& w)
	:pos_rel(w.pos_rel)
	,pos_abs(w.pos_abs)
	,size(w.size)
	,childs(w.childs)
	,fx(w.fx)
	,current_fx(nullptr)
	,state(w.state)
	,state_colors()
{
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
	if (get<WCText>()) delete get<WCText>();
	if (get<WCBox>()) delete get<WCBox>();
	if (get<WCImage>()) delete get<WCImage>();
}

sf::FloatRect Widget::getGlobalBounds() {
	if (get<WCText>() != nullptr) {
		return get<WCText>()->getGlobalBounds();
	}
	else if (get<WCBox>() != nullptr) {
		return get<WCBox>()->getGlobalBounds();
	}

	return sf::FloatRect();
}

sf::FloatRect Widget::getLocalBounds() {
	if (get<WCImage>() != nullptr) {
		return get<WCImage>()->getLocalBounds();
	}
	else if (get<WCBox>() != nullptr) {
		//return get<WCBox>()->getLocalBounds();
	}
	else {
		return {0.0f, 0.0f, 0.0f, 0.0f};
	}
}

sf::Vector2i Widget::getSize() {
	if (get<WCImage>() != nullptr) {
		return get<WCImage>()->getSize();
	}
	if (get<WCBox>() != nullptr) {
		return get<WCBox>()->getSize();
	}
	else {
		return {0, 0};
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

	if (get<WCBox>() != nullptr) {
		get<WCBox>()->setPosition(pos_abs.x, pos_abs.y);
	}

	if (get<WCText>() != nullptr) {
		get<WCText>()->setPosition(pos_abs.x, pos_abs.y);
	}
	if (get<WCImage>() != nullptr) {
		get<WCImage>()->setPosition(pos_abs.x, pos_abs.y);
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
	}

	for (Widget& w : getChilds()) {
		w.update(sf::Vector2f(getSize()), getPosition());
	}
}

sf::Vector2f Widget::getPosition() {
	if (get<WCImage>() != nullptr) {
		return get<WCImage>()->getPosition();
	}
	else {
		return {0.0f, 0.0f};
	}
}



void Widget::update() {
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

		if (get<WCImage>()) {
			if (fx.type = WidgetFx::Type::FADE_IN_OUT) {
				sf::Color color = get<WCImage>()->getColor();
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

				get<WCImage>()->setColor(color);
			}
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
		//get<WCBox>()->setBackgroundColor(color);
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
		target.draw(*get<WCBox>(), states);
	}
}