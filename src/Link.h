#include <string>
#include <variant>

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

    Link::Target m_target;
    std::variant<std::monostate, int*, std::string*> m_data;

    Link(Link::Target target);
    Link(Link::Target target, int* data);
    Link(Link::Target target, std::string* data);
    Link(const Link& link);
    Link* clone() const;

    std::string getAsString();
};

