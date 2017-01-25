#include "Wrapper.hpp"

Wrapper::Wrapper(Grid& grid, ofn::Variable var, sf::Color color)
{
    m_grid = &grid;
    m_color = color;
    m_var = var;
    m_visible = true;
    //m_negative = var.getOrder() == ofn::Order::Negative;
    update();
}

void Wrapper::update()
{
    int index = 0;
    m_vertices.clear();
    for (int i = 0; i < m_var.discretization.size; ++i) {
        m_vertices.emplace_back(m_grid->getPointCoords({
            m_var[i], ofn::Variable::discretization.yAxis[index++]
        }), m_color);
    }
}

void Wrapper::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_vertices.empty()) {
        target.draw(&m_vertices[0], m_vertices.size(), sf::LineStrip, states);
        for (int i = 0; i < m_vertices.size(); ++i) {
            int size = 2;
            auto circle = std::move(sf::CircleShape(size));
            circle.setOrigin(size, size);
            circle.setFillColor(m_vertices[i].color);
            circle.setPosition(m_vertices[i].position);
            target.draw(circle, states);
        }

        auto circle = std::move(sf::CircleShape(6));
        circle.setOrigin(6, 6);
        circle.setFillColor(m_vertices[0].color);
        circle.setPosition(m_vertices[0].position);
        target.draw(circle, states);
    }
}