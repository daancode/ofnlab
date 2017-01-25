#include "Grid.hpp"
#include <ofn++\Variable.hpp>

void Grid::create(sf::Vector2f size, sf::Vector2i range)
{
    m_size = { size.x, size.y };
    m_range = range;
    m_vertices.setPrimitiveType(sf::Lines);
    refresh();
    //setOrigin({ m_size.x / 2, m_size.y / 2 });
}

void Grid::refresh()
{
    m_vertices.clear();
    int totalRange = (m_range.y + abs(m_range.x));

    sf::Vector2f offset = {
        m_size.x / totalRange,
        m_size.y / ofn::Variable::discretization.ratio
    };

    for (int i = 0; i < ofn::Variable::discretization.ratio; ++i) {
        m_vertices.append({ { 0, offset.y * i }, sf::Color(240, 240, 240) });
        m_vertices.append({ { m_size.x, offset.y * i }, sf::Color(240, 240, 240) });
    }

    for (int i = 0; i <= totalRange; ++i) {
        if (i == abs(m_range.x)) {
            m_vertices.append({ { offset.x * i, 0 }, sf::Color::Black });
            m_vertices.append({ { offset.x * i, m_size.y }, sf::Color::Black });
            m_vertices.append({ { offset.x * i - 5, 0 }, sf::Color::Black });
            m_vertices.append({ { offset.x * i + 5, 0 }, sf::Color::Black });
        }
        else {
            m_vertices.append({ { offset.x * i, 0 }, sf::Color(240, 240, 240) });
            m_vertices.append({ { offset.x * i, m_size.y }, sf::Color(240, 240, 240) });
            m_vertices.append({ { offset.x * i, m_size.y - 5 }, sf::Color::Black });
            m_vertices.append({ { offset.x * i, m_size.y + 5 }, sf::Color::Black });
        }
    }

    m_vertices.append({ { m_size.x, m_size.y }, sf::Color::Black });
    m_vertices.append({ { 0.f, m_size.y }, sf::Color::Black });
}

void Grid::refresh(sf::Vector2f size)
{
    create({ size.x - 50, size.y - 50 }, m_range);
    //setPosition(size.x / 2, size.y / 2);
}

sf::Vector2f Grid::getPointCoords(sf::Vector2f point)
{
    sf::Vector2f newPoint;
    newPoint.x = static_cast<float>(
        m_size.x / (m_range.y + abs(m_range.x))) * (abs(m_range.x) + point.x
    );

    if (point.y >= 1) {
        newPoint.y = 0;
    }
    else if(point.y == 0) {
        newPoint.y = m_size.y;
    }
    else {
        newPoint.y = m_size.y - (m_size.y * point.y);
    }

    newPoint = getTransform().transformPoint(newPoint);
    return newPoint;
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_vertices, states);
}