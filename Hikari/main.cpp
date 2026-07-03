//===============================
// Fichier : main.cpp
// Description : Point d'entrée du jeu, création de la fenêtre, boucle principale du jeu
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
#include "Player.h"
#include <iostream>
#include "MapData.h"
#include "MiniMap.h"
#include "Camera.h"
#include "HitBox.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <algorithm>
#include <functional>
#include <vector>

// Structure pour stocker les éléments à rendre avec leur position Y
struct RenderItem {
	float y;
	std::function<void()> drawCall;
};

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
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Hikari");
	//window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	//===============================
	// Création du sprite et de la texture du joueur
	//===============================
	sf::Texture texture;
	sf::Sprite sprite(texture);

	Player playerCharacter(texture, sprite, "Assets/adventurer.png", 96, 80);
	playerCharacter.initialState("Sid");

	//===============================
	// Création du sprite et de la texture du personnage de test
	//===============================
	sf::Texture testTexture;
	sf::Sprite testSprite(testTexture);

	Player testCharacter(testTexture, testSprite, "Assets/adventurer.png", 96, 80);
	testCharacter.initialState("Test");
	testCharacter.setPlayerPosition(testSprite, 200.f, 200.f);

	// Verrouiller le mouvement du personnage de test pour éviter qu'il ne se déplace pendant les tests
	testCharacter.LockPlayer();

	//===============================
	// Création de la caméra
	//===============================
	Camera playerCamera(WINDOW_WIDTH, WINDOW_HEIGHT, MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, 0.7f);

	//===============================
	// Création de la mini-carte
	//===============================
	MiniMap miniMap((float)(MAP_WIDTH),
					(float)(MAP_HEIGHT),
					(float)WINDOW_WIDTH,
					(float)WINDOW_HEIGHT, TILE_SIZE);

	//===============================
	// Création de la carte de tuiles
	//===============================
	TileMap tileMap(MAP_WIDTH, MAP_HEIGHT);

	int map[MAX_TILES] = { 0 };
	int map_width = MAP_WIDTH;
	int map_height = MAP_HEIGHT;

	std::vector<int> mapData;

	// Charger les données de la carte à partir du fichier CSV
	loadCSV("Assets/InitialMap.csv", mapData, map_width, map_height);

	// Copier les données de la carte chargée dans le tableau 1D
	loadMap(map, mapData, MAX_TILES);

	// Charger la map avec ces nouvelles dimensions
	if (!tileMap.load("Assets/Tileset_Grass.png", sf::Vector2u(32, 32), map))
	{
		std::cerr << "Failed to load tile map!" << std::endl;
		return -1;
	}

	std::vector<Player> players;

	// Horloge pour gérer le temps écoulé entre les mises à jour
	sf::Clock deltaClock;

	// Variable pour afficher ou non les hitboxes des personnages
	bool showHitboxes = false;

	//===============================
	// Loop principal du jeu
	//===============================
    while (window.isOpen())
    {
		float dt = deltaClock.restart().asSeconds();

		if (dt > 0.1f) {
			dt = 0.1f;
		}

		//===============================
		// Gestion des événements de la fenêtre
		//===============================
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->code == sf::Keyboard::Key::H) {
					showHitboxes = !showHitboxes;
				}
			}

			/*for (int i = 0; i < players.size(); ++i) {
				players[i].handleEvent(*event);
			}*/

			miniMap.handleEvent(*event);
			playerCharacter.handleEvent(*event);
			testCharacter.handleEvent(*event);
        }

		playerCharacter.update(dt, window, sprite);
		testCharacter.update(dt, window, testSprite);

		// Récupérer les hitboxes des deux personnages pour la détection de collision
		sf::FloatRect playerHitbox = playerCharacter.getHitbox(sprite);
		sf::FloatRect testHitbox = testCharacter.getHitbox(testSprite);

		// Détection de collision
		resolveCollision(sprite, playerHitbox, testHitbox);

		// Mettre à jour la position de la caméra pour suivre le joueur
		playerCamera.follow(playerCharacter, sprite);
		playerCamera.clamped(playerCharacter, sprite);

		// Pour le rendu, on efface la fenêtre avec une couleur de fond gris foncé
        window.clear(sf::Color(50,50,50));

		// Appliquer la vue de la caméra à la fenêtre
		playerCamera.apply(window);

		window.draw(tileMap);

		std::vector<RenderItem> renderQueue;

		renderQueue.push_back({ sprite.getPosition().y, [&]() 
			{ playerCharacter.draw(window, sprite); } });
		renderQueue.push_back({ testSprite.getPosition().y, [&]() 
			{ testCharacter.draw(window, testSprite); } });

		// TODO : Ajouter d'autres éléments à la file d'attente de rendu si nécessaire

		// Trier par la position Y des éléments
		std::sort(renderQueue.begin(), renderQueue.end(), [](const RenderItem& a, const RenderItem& b) 
			{
				return a.y < b.y;
			});

		// Dessiner les éléments dans l'ordre trié
		for (const auto& item : renderQueue) {
			item.drawCall();
		}

		// testCharacter.draw(window, testSprite);
		// playerCharacter.draw(window, sprite);

		// Dessiner les hitboxes si l'option est activée
		if (showHitboxes) {
			drawHitboxOutline(window, playerHitbox);
			drawHitboxOutline(window, testHitbox);
		}

		miniMap.draw(window, tileMap, playerCharacter, sprite, testCharacter, testSprite);
		
		applyCameraUI(window);

        window.display();
    }
	return 0;
}