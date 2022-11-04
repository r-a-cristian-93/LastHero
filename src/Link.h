#include <string>

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

    virtual Link* clone() const = 0;
    void setData(const void* data);
    Link::Target& getTarget();
    virtual std::string getString() = 0;

protected:
     Link::Target m_target;
     const void* m_data;

     Link(Link::Target target);
};

class LinkInt: public Link {
public:
    LinkInt(Link::Target target);
    LinkInt(const LinkInt& link);

    Link* clone() const;
    std::string getString();
};

class LinkString: public Link {
public:
    LinkString(Link::Target target);
    LinkString(const LinkString& link);

    Link* clone() const;
    std::string getString();
};