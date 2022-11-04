#include "Link.h"

Link::Link(Link::Target target)
    :m_target(target)
    ,m_data(nullptr)
    {}

void Link::setData(const void* data) {
    m_data = data;
}

Link::Target& Link::getTarget() {
    return m_target;
}




LinkInt::LinkInt(Link::Target target)
    :Link(target)
    {}

LinkInt::LinkInt(const LinkInt& link)
    :Link(link.m_target)
{
    m_data = link.m_data;
}

Link* LinkInt::clone() const {
    return new LinkInt(*this);
}

std::string LinkInt::getString () {
    if (m_data != nullptr) {
        return  std::to_string(*(static_cast<const int*>(m_data)));
    }
    else {
        return "";
    }
}




LinkString::LinkString(Link::Target target)
    :Link(target)
    {}

LinkString::LinkString(const LinkString& link)
    :Link(link.m_target)
{
    m_data = link.m_data;
}

Link* LinkString::clone() const {
    return new LinkString(*this);
}

std::string LinkString::getString () {
    if (m_data != nullptr) {
        return *(static_cast<const std::string*>(m_data));
    }
    else {
        return "";
    }
}