#pragma once
#include <SFML\Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "Grid.hpp"
#include "Wrapper.hpp"

struct WrapperInitializer
{
    char name[20] = { "" };
    float value[4] = { 0.f,0.f,0.f,0.f };
    float color[3] = { 0.f, 0.f, 0.f };
    bool visible = true;
    bool negative = false;
};

class Interface
{
public:
    Interface(sf::RenderWindow& window, Grid& grid);
    ~Interface();

    void update(sf::Event& event);
    void updateGrid();
    void update();
    void display();

private:
    void addNewVariable();
    void calculationDialog();

// Members
    Grid *m_grid;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    std::vector<Wrapper> m_wrappers;
    sf::RenderWindow *m_window;
    sf::Clock m_clock;
    sf::Time m_delta;

    int m_range[2];
    int currentIndex{ 0 };
};