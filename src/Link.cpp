#include "Link.h"

Link::Link(Link::Target target)
    :m_target(target)
	,m_data(std::monostate())
    {}

Link::Link(Link::Target target, int* data)
    :m_target(target)
    ,m_data(data)
    {}

Link::Link(Link::Target target, std::string* data)
    :m_target(target)
    ,m_data(data)
    {}

Link::Link(const Link& link) {
    m_data = link.m_data;
    m_target = link.m_target;
};

Link* Link::clone() const {
    return new Link(*this);
};

std::string Link::getAsString() {
	if (std::holds_alternative<std::string*>(m_data)) {
		return *(std::get<std::string*>(m_data));
	}
	else if (std::holds_alternative<int*>(m_data)) {
		return std::to_string(*(std::get<int*>(m_data)));
	}
	else {
		return "";
	}
};