#include <windows.h>

#pragma comment (lib, "OpenGL32.lib")
#include <SFML\Graphics.hpp>

#include "Interface.hpp"
#include "Wrapper.hpp"
#include "Grid.hpp"

int WINAPI WinMain(HINSTANCE hThis, HINSTANCE hPrev, LPSTR lpsz, int nFStil)
{
    sf::Clock clock;

    sf::RenderWindow window({ 1024, 768, 32 }, "OFNLAB"/*, sf::Style::Fullscreen*/);
    window.setFramerateLimit(60);

    sf::Image icon;
    icon.loadFromFile("assets/icon.png");
    window.setIcon(64, 64, icon.getPixelsPtr());

    ofn::Variable::discretization.initialize(10);

    Grid grid;
    grid.setPosition(25, 25);
    Interface ui(window, grid);
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ui.update(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                grid.refresh({ visibleArea.width, visibleArea.height });
                ui.updateGrid();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        ui.update();

        window.clear(sf::Color(255, 255, 255));
        window.draw(grid);
        ui.display();
        window.display();

        clock.restart();
    }
}