#ifndef TILEMAP
#define TILEMAP

#include <SFML/Graphics.hpp>

class Tilemap : public sf::Drawable, public sf::Transformable {
private:
	sf::VertexArray m_vertices;
	sf::Texture* m_tileset;
	sf::FloatRect bounds;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    bool loadLevel(sf::Vector2u tile_size, const int* tiles, sf::Vector2u map_size);
    void setTexture(sf::Texture& texture);
    const sf::FloatRect& getBounds();
};

#endif
