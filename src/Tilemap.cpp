#include "Tilemap.h"

void Tilemap::setTexture(sf::Texture& texture) {
	m_tileset = &texture;
}

bool Tilemap::loadLevel(sf::Vector2u tile_size, const int* tiles, sf::Vector2u map_size) {
	bounds = sf::FloatRect(0, 0, map_size.x*tile_size.x, map_size.y*tile_size.y);

	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(map_size.x * map_size.y * 4);

	// populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < map_size.x; ++i)
		for (unsigned int j = 0; j < map_size.y; ++j)
		{
			// get the current tile number
			int tile_number = tiles[i + j * map_size.x];

			// find its position in the tileset texture
			int tu = tile_number % (m_tileset->getSize().x / tile_size.x);
			int tv = tile_number / (m_tileset->getSize().x / tile_size.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(i + j * map_size.x) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * tile_size.x, j * tile_size.y);
			quad[1].position = sf::Vector2f((i + 1) * tile_size.x, j * tile_size.y);
			quad[2].position = sf::Vector2f((i + 1) * tile_size.x, (j + 1) * tile_size.y);
			quad[3].position = sf::Vector2f(i * tile_size.x, (j + 1) * tile_size.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * tile_size.x, tv * tile_size.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size.x, tv * tile_size.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size.x, (tv + 1) * tile_size.y);
			quad[3].texCoords = sf::Vector2f(tu * tile_size.x, (tv + 1) * tile_size.y);
		}

	return true;
}

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = m_tileset;
	target.draw(m_vertices, states);
}

const sf::FloatRect& Tilemap::getBounds() {
	return bounds;
}
