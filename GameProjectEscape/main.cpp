#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>

int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Game Project Escape Prototype");
	window.setFramerateLimit(60);

	// Creation of our player
	sf::RectangleShape player(sf::Vector2f(40.f, 40.f));
	player.setFillColor(sf::Color::Green);
	player.setPosition(sf::Vector2(400.f, 300.f));

	float playerSpeed = 5.f; // Speed of the player

    // Main game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) player.move(sf::Vector2(-playerSpeed, 0.f));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) player.move(sf::Vector2(playerSpeed, 0.f));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) player.move(sf::Vector2(0.f, -playerSpeed));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) player.move(sf::Vector2(0.f, playerSpeed));

        window.clear();
        window.draw(player);
        window.display();
    }
}