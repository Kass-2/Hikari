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
#include <SFML/Graphics.hpp>
#include <optional>
#include <SFML/System/Clock.hpp>

// Structure pour stocker les éléments à rendre avec leur position Y
struct RenderItem {
	float y;
	std::function<void()> drawCall;
};

// Énumération pour les états du jeu
enum class GameState {
	Menu,
	PauseMenu,
	Playing
};

// Structure pour stocker les éléments du menu
struct MenuButton {
	sf::RectangleShape background;
	sf::Text text;

	MenuButton(const sf::Font& font) : text(font) {}
};

//===============================
// Constantes de couleurs
//===============================
sf::Color WHITE_LESS(224, 224, 224);
sf::Color TRANSPARENT_WHITE(255, 255, 255, 128);

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
	// Création du menu principal
	//===============================

	// Police pour le texte du menu
	sf::Font font;
	if (!font.openFromFile("Assets/PressStart2P-Regular.ttf"))
	{
		std::cerr << "Erreur lors du chargement de la police de texte" << std::endl;
		return -1;
	}

	std::vector<std::string> pauseLabels = {
		"RESUME", "SETTINGS", "EXIT TO MAIN MENU",
	};

	std::vector<MenuButton> pauseMenu;
	int selectedPauseIndex = 0;

	float startX = 20.f;
	float startY = 100.f;
	float buttonWidth = 300.f;
	float buttonHeight = 40.f;
	float spacing = 10.f;

	// En-tête "PAUSE MENU"
	sf::Text pauseHeader(font, "PAUSE MENU", 20);
	pauseHeader.setFillColor(sf::Color::Black);
	pauseHeader.setPosition({ (startX * 1.5f), 60.f });

	// Génération les boutons du menu de pause
	for (size_t i = 0; i < pauseLabels.size(); ++i) {
		MenuButton button(font);

		// Configuration du fond du bouton
		button.background.setSize({ buttonWidth, buttonHeight });
		button.background.setPosition({ startX, startY + i * (buttonHeight + spacing) });

		// Remplissage du texte des boutons
		if (i == 0) {
			button.background.setFillColor(sf::Color::Red);
		}
		else {
			button.background.setFillColor(sf::Color(WHITE_LESS));
		}

		// Configuration du texte à l'intérieur du bouton
		button.text = sf::Text(font, pauseLabels[i], 15);
		button.text.setFillColor(sf::Color::Black);

		// Centrer le texte verticalement à l'intérieur du rectangle
		button.text.setPosition({ startX + 15.f, startY + i * (buttonHeight + spacing) + 12.f });

		pauseMenu.push_back(button);
	}

	//===============================
	// Création du sprite et de la texture du joueur
	//===============================
	sf::Texture texture;
	sf::Sprite sprite(texture);

	Player playerCharacter(texture, sprite, "Assets/adventurerFull.png", 96, 80);
	playerCharacter.initialState("Sid");
	
	//===============================
	// Création du sprite et de la texture du personnage de test
	//===============================
	sf::Texture testTexture;
	sf::Sprite testSprite(testTexture);

	Player testCharacter(testTexture, testSprite, "Assets/adventurerFull.png", 96, 80);
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

	//===============================
	// Variables pour la boucle principale du jeu
	//===============================
	std::vector<Player> players;

	// Horloge pour gérer le temps écoulé entre les mises à jour
	sf::Clock deltaClock;

	// Variable pour afficher ou non les hitboxes des personnages
	bool showHitboxes = false;

	// Variable pour gérer l'état du jeu (menu de pause ou en cours de jeu)
	GameState currentGameState = GameState::Playing;

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

			// --- GAMEPLAY CONTROLS ---
			if (currentGameState == GameState::Playing) {
				if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
					if (keyPressed->code == sf::Keyboard::Key::Escape) {
						currentGameState = GameState::PauseMenu; // Pause the game
					}
				}
			}
			// --- PAUSE MENU CONTROLS ---
			else if (currentGameState == GameState::PauseMenu) {

				// Mouse Hover Logic
				if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
					sf::Vector2f mousePos = window.mapPixelToCoords(mouseMoved->position);

					for (size_t i = 0; i < pauseMenu.size(); ++i) {
						if (pauseMenu[i].background.getGlobalBounds().contains(mousePos)) {
							// Reset old selected button to default look
							pauseMenu[selectedPauseIndex].background.setFillColor(sf::Color(WHITE_LESS));
							pauseMenu[selectedPauseIndex].text.setFillColor(sf::Color::Black);

							// Highlight new hovered button
							selectedPauseIndex = i;
							pauseMenu[selectedPauseIndex].background.setFillColor(sf::Color::Red);
							//pauseMenu[selectedPauseIndex].text.setFillColor(sf::Color::Black);
						}
					}
				}

				// Mouse Click Logic
				if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
					if (mousePressed->button == sf::Mouse::Button::Left) {
						sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);

						for (size_t i = 0; i < pauseMenu.size(); ++i) {
							if (pauseMenu[i].background.getGlobalBounds().contains(mousePos)) {
								if (i == 0) currentGameState = GameState::Playing; // Resume
								else if (i == 3) currentGameState = GameState::Menu; // Main Menu (Implement state as needed)
								else if (i == 2) window.close(); // Exit
							}
						}
					}
				}

				// Keyboard Logic (Unpause with ESC)
				if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
					if (keyPressed->code == sf::Keyboard::Key::Escape) {
						currentGameState = GameState::Playing;
					}
				}
			}

			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->code == sf::Keyboard::Key::H &&
					keyPressed->code == sf::Keyboard::Key::LShift &&
					keyPressed->code == sf::Keyboard::Key::LControl) {
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

		sf::FloatRect playerHitbox;
		sf::FloatRect testHitbox;

		if (currentGameState == GameState::Playing) {
			playerCharacter.update(dt, window, sprite);
			testCharacter.update(dt, window, testSprite);

			// Récupérer les hitboxes des deux personnages pour la détection de collision
			playerHitbox = playerCharacter.getHitbox(sprite);
			testHitbox = testCharacter.getHitbox(testSprite);

			// Détection de collision
			resolveCollision(sprite, playerHitbox, testHitbox);

			// Mettre à jour la position de la caméra pour suivre le joueur
			playerCamera.follow(playerCharacter, sprite);
			playerCamera.clamped(playerCharacter, sprite);
		}

		// Pour le rendu, on efface la fenêtre avec une couleur noire
        window.clear(sf::Color::Black);

		if (currentGameState == GameState::Playing || currentGameState == GameState::PauseMenu) {
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

			if (currentGameState == GameState::PauseMenu) {
				// Draw a dark full-screen overlay to dim the game world
				sf::RectangleShape dimOverlay(sf::Vector2f(1920.f, 1080.f));
				dimOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // Alpha 150 dims background
				window.draw(dimOverlay);

				// Draw Pause Menu UI elements
				window.draw(pauseHeader);
				for (const auto& button : pauseMenu) {
					window.draw(button.background);
					window.draw(button.text);
				}
			}
		}

		// À supprimer
		/*if (currentGameState == GameState::PauseMenu) {
			for (int i = 0; i < MENU_ITEM_COUNT; i++) {
				window.draw(pauseMenu[i]);
			}
		}
		else if (currentGameState == GameState::Playing) {
			window.draw(gameText);
		}*/

        window.display();
    }
	return 0;
}