//===============================
// Fichier : main.cpp
// Description : Point d'entrée du jeu, création de la fenêtre, boucle principale du jeu
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

#include <iostream>
#include <SFML/Graphics.hpp>
#include "player.h"
#include "tileMap.h"

//===============================
// Constantes pour la taille des tuiles et les dimensions de la carte
//===============================
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 576;
const int TILE_SIZE = 32;

const int MAP_WIDTH = WINDOW_WIDTH / TILE_SIZE;   // Nombre de tuiles en largeur
const int MAP_HEIGHT = WINDOW_HEIGHT / TILE_SIZE; // Nombre de tuiles en hauteur
const int MAX_TILES = MAP_WIDTH * MAP_HEIGHT;     // Nombre total de tuiles dans la carte

int main()
{
	//===============================
	// Creation de la fenêtre de jeu
	//===============================
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Game Project Escape Prototype");
	window.setFramerateLimit(60);

	//===============================
	// Création du sprite et de la texture du joueur
	//===============================
	sf::Texture texture;
	sf::Sprite sprite(texture);

	Player playerCharacter(texture, sprite, "Assets/player.png");
	playerCharacter.initialState("Sid");

	//===============================
	// Création de la carte de tuiles
	//===============================
	TileMap tileMap(MAP_WIDTH, MAP_HEIGHT);

	int mapLayout[MAX_TILES] = { 0 };

	for (int i = 0; i < MAX_TILES; ++i) {
		mapLayout[i] = 356;
	};

	// Charger la map avec ces nouvelles dimensions
	if (!tileMap.load("Assets/Ground.png", sf::Vector2u(32, 32), mapLayout))
	{
		std::cerr << "Failed to load tile map!" << std::endl;
		return -1;
	}

	//===============================
	// Loop principal du jeu
	//===============================
    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

			playerCharacter.handleEvent(*event);
        }

		playerCharacter.update(1.f / 60.f, window, sprite);

        window.clear(sf::Color(50,50,50));
		window.draw(tileMap);
		playerCharacter.draw(window, sprite);
        window.display();
    }
	return 0;
}