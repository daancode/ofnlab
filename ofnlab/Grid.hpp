#pragma once
#include <SFML\Graphics.hpp>

class Grid : public sf::Transformable, public sf::Drawable
{
public:
    void create(sf::Vector2f size, sf::Vector2i range);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void refresh(sf::Vector2f size);
    sf::Vector2f getPointCoords(sf::Vector2f point);

private:
    void refresh();

// Members.
    sf::Vector2f m_size;
    sf::VertexArray m_vertices;
    sf::Vector2i m_range;
};