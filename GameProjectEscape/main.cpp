//===============================
// Fichier : main.cpp
// Description : Point d'entrée du jeu, création de la fenêtre, boucle principale du jeu
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

#include <iostream>
#include <SFML/Graphics.hpp>
#include "player.h"

int main()
{
	//===============================
	// Creation de la fenêtre de jeu
	//===============================
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Game Project Escape Prototype");
	window.setFramerateLimit(60);

	sf::Texture texture;
	sf::Sprite sprite(texture);

	Player player(texture, sprite, "player.png");

	//===============================
	// Loop principal du jeu
	//===============================
    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

		player.update(1.f / 60.f, window, sprite);

        window.clear(sf::Color(50,50,50));
		player.draw(window, sprite);
        window.display();
    }
}