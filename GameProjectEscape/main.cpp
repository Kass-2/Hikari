#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>

int main()
{
	// Create the main window
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Game Project Escape Prototype");
	window.setFramerateLimit(60);

	// Creation of our player
	sf::RectangleShape player({40.f, 40.f});
	player.setFillColor(sf::Color::Green);
    player.setPosition({ 400.f, 300.f });

	float playerSpeed = 5.f; // Speed of the player

    // Main game loop
    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) player.move({-playerSpeed, 0.f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) player.move({playerSpeed, 0.f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) player.move({0.f, -playerSpeed});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) player.move({0.f, playerSpeed});

        window.clear();
        window.draw(player);
        window.display();
    }
}