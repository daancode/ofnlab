#pragma once
#include <SFML\Graphics.hpp>
#include <ofn++\Variable.hpp>

#include "Grid.hpp"

struct Wrapper : public sf::Drawable
{
    Wrapper(Grid& grid, ofn::Variable var, sf::Color color);
    void update();
    
    Grid *m_grid;
    ofn::Variable m_var;
    sf::Color m_color;
    char m_varname[5];
    bool m_visible;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    std::vector<sf::Vertex> m_vertices;
};