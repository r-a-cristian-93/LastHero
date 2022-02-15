#include <SFML/Graphics.hpp>
#include <vector>

class ParticlesEmitter: public sf::Drawable {
	struct Particle {
		sf::Vector2f velocity;
		int lifetime = 0;
	};

	sf::Vector2f m_position;
	std::vector<Particle> m_particles;
	sf::VertexArray m_vertices;
	float m_size;
	sf::Color m_color;

	void resetParticles(size_t count, float size) {
		m_particles = std::vector<Particle>(count);
		m_vertices = sf::VertexArray(sf::Quads, count*4);
		m_size = size;

		for (size_t p = 0; p < m_particles.size(); p++) {
			resetParticle(p, true);
		}
	}

	void resetParticle(size_t index, bool first) {
		int x=0, y=0;

		m_vertices[4*index + 0].position = m_position + sf::Vector2f(x, y);
		m_vertices[4*index + 1].position = m_position + sf::Vector2f(x + m_size, y);
		m_vertices[4*index + 2].position = m_position + sf::Vector2f(x + m_size, y + m_size);
		m_vertices[4*index + 3].position = m_position + sf::Vector2f(x, y + m_size);

		unsigned char alpha = rand()%155 + 100;
		sf::Color color = {m_color.r, m_color.g, m_color.b, alpha};

		if (first) color.a = 0;

		m_vertices[4*index + 0].color = color;
		m_vertices[4*index + 1].color = color;
		m_vertices[4*index + 2].color = color;
		m_vertices[4*index + 3].color = color;

		m_particles[index].velocity = sf::Vector2f( ((float)rand()/RAND_MAX) * 10 - 5, ((float)rand()/RAND_MAX) * 10 - 5 );

		m_particles[index].lifetime = rand()%60 + 30;
	}

public:
	ParticlesEmitter() {}

	ParticlesEmitter(sf::Vector2f position, sf::Color color, size_t count, float size)
		:m_position(position)
		,m_color(color)
	{
		resetParticles(count, size);
	}

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_vertices);
    }

	void update() {
		for (std::size_t index=0; index<m_particles.size(); index++) {
			if (m_particles[index].lifetime == 0) {
				resetParticle(index, false);
			}

			m_vertices[4*index + 0].position += m_particles[index].velocity;
			m_vertices[4*index + 1].position += m_particles[index].velocity;
			m_vertices[4*index + 2].position += m_particles[index].velocity;
			m_vertices[4*index + 3].position += m_particles[index].velocity;

			m_particles[index].lifetime--;
		}
	}
};


