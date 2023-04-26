#include "Link.h"

Link::Link(Link::Target target)
    :m_target(target)
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
    std::string** data_string = std::get_if<std::string*>(&m_data);
    int** data_int = std::get_if<int*>(&m_data);

    if (data_string != nullptr) {
        return **data_string;
    }
    else if (data_int != nullptr) {
        return std::to_string(**data_int);
    }
    else {
        return "";
    }
};