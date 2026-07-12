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
#include "HealthBar.h"
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
#include <string>

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

//==============================
// Fonction pour calculer une taille en fonction de la taille de la fenêtre
// Paramètres:
// - baseFontSize : la taille de la police d'origine
// - baseWindowWidth : la largeur de la fenêtre d'origine
// - baseWindowHeight : la hauteur de la fenêtre d'origine
// - currentWindowWidth : la largeur de la fenêtre actuelle
// - currentWindowHeight : la hauteur de la fenêtre actuelle
// Description:
// La taille est basé sur une ancienne taille de fenêtre et une taille précédente
//==============================
static int calculateSize(int baseSize, int baseWindowWidth, int baseWindowHeight, int currentWindowWidth, int currentWindowHeight) {
	float PX = static_cast<float>(baseSize) / baseWindowWidth;	// Pourcentage de la position X de l'en-tête par rapport à la largeur de la fenêtre (960)
	float PY = static_cast<float>(baseSize) / baseWindowHeight;	// Pourcentage de la position Y de l'en-tête par rapport à la hauteur de la fenêtre (576)

	float sizeX = currentWindowWidth * PX;	// Taille de la position X de l'en-tête par rapport à la largeur de la fenêtre (960)
	float sizeY = currentWindowHeight * PY;	// Taille de la position Y de l'en-tête par rapport à la hauteur de la fenêtre (576)

	return int(((sizeX + sizeY) / 2));
}

//==============================
// Fonction pour calculer une taille X en fonction de la taille de la fenêtre
// Paramètres:
// - baseFontSize : la taille de la police d'origine
// - baseWindowWidth : la largeur de la fenêtre d'origine
// - currentWindowWidth : la largeur de la fenêtre actuelle
// Description:
// La taille est basé sur une ancienne taille X de fenêtre et une taille X précédente
//==============================
static float calculateSizeX(float baseSize, int baseWindowWidth, int currentWindowWidth) {
	float PX = baseSize / baseWindowWidth;	// Pourcentage de la position X de l'en-tête par rapport à la largeur de la fenêtre (960)
	
	float sizeX = currentWindowWidth * PX;	// Taille de la position X de l'en-tête par rapport à la largeur de la fenêtre (960)
	
	return sizeX;
}

//==============================
// Fonction pour calculer une taille Y en fonction de la taille de la fenêtre
// Paramètres:
// - baseFontSize : la taille de la police d'origine
// - baseWindowHeight : la hauteur de la fenêtre d'origine
// - currentWindowHeight : la hauteur de la fenêtre actuelle
// Description:
// La taille est basé sur une ancienne taille Y de fenêtre et une taille Y précédente
//==============================
static float calculateSizeY(float baseSize, int baseWindowHeight, int currentWindowHeight) {
	float PY = baseSize / baseWindowHeight;	// Pourcentage de la position Y de l'en-tête par rapport à la hauteur de la fenêtre (576)
	
	float sizeY = currentWindowHeight * PY;	// Taille de la position Y de l'en-tête par rapport à la hauteur de la fenêtre (576)
	
	return sizeY;
}

//==============================
// Fonction pour recalculer la position des menus
//==============================
void recalculateLayout(
	unsigned int width, unsigned int height,
	sf::Font& mainMenuFont,
	sf::Font& pauseMenuFont,
	sf::Text& MMHeader, std::vector<MenuButton>& mainMenu,
	sf::Text& pauseHeader, std::vector<MenuButton>& pauseMenu)
{
	// Update main header size and position
	int MMHeaderSize = calculateSize(70, 960, 576, width, height);
	MMHeader.setCharacterSize(MMHeaderSize);
	float startXMain = width * 0.0208f;
	float startYMain = height * 0.694f;
	float MMHeaderOffsetY = calculateSizeY(50.f, 576, height);
	MMHeader.setPosition({ (startXMain * 1.5f), MMHeaderOffsetY });

	// Recalculate Main Menu buttons
	float buttonWidthMain = width * 0.156f;
	float buttonHeightMain = height * 0.052f;
	float spacingMain = height * 0.0173f;
	int mainButtonSize = calculateSize(14, 960, 576, width, height);
	float buttonOffSetXMain = calculateSizeX(12.f, 960, width);
	float buttonOffSetYMain = calculateSizeY(8.f, 576, height);

	for (size_t i = 0; i < mainMenu.size(); ++i) {
		mainMenu[i].background.setSize({ buttonWidthMain, buttonHeightMain });
		mainMenu[i].background.setPosition({ startXMain, startYMain + i * (buttonHeightMain + spacingMain) });
		mainMenu[i].text.setCharacterSize(mainButtonSize);
		mainMenu[i].text.setPosition({ startXMain + buttonOffSetXMain, startYMain + i * (buttonHeightMain + spacingMain) + buttonOffSetYMain });
	}

	// Recalculate Pause Header size and position
	int pauseHeaderSize = calculateSize(30, 960, 576, width, height);
	pauseHeader.setCharacterSize(pauseHeaderSize);
	float startX = width * 0.0208f;
	float startY = height * 0.1736f;
	float pauseHeaderOffsetY = calculateSizeY(30.f, 576, height);
	pauseHeader.setPosition({ (startX * 1.75f), pauseHeaderOffsetY });

	// Recalculate Pause Menu buttons
	float buttonWidth = width * 0.3125f;
	float buttonHeight = height * 0.0694f;
	float spacing = height * 0.0173f;
	int pauseButtonSize = calculateSize(14, 960, 576, width, height);
	float buttonOffSetX = calculateSizeX(15.f, 960, width);
	float buttonOffSetY = calculateSizeY(14.f, 576, height);

	for (size_t i = 0; i < pauseMenu.size(); ++i) {
		pauseMenu[i].background.setSize({ buttonWidth, buttonHeight });
		pauseMenu[i].background.setPosition({ startX, startY + i * (buttonHeight + spacing) });
		pauseMenu[i].text.setCharacterSize(pauseButtonSize);
		pauseMenu[i].text.setPosition({ startX + buttonOffSetX, startY + i * (buttonHeight + spacing) + buttonOffSetY });
	}
}

//===============================
// Constantes de couleurs
//===============================
sf::Color WHITE_LESS(224, 224, 224);
sf::Color SUBTLE_WHITE(255, 255, 255, 38);
sf::Color SUBTLE_BLACK(0, 0, 0, 38);
sf::Color WHITE_GRAY(86, 86, 86);

//===============================
// Constantes pour la taille des tuiles et les dimensions de la carte
//===============================
unsigned int windowWidth = 1440;
unsigned int windowHeight = 864;
const int TILE_SIZE = 32;

//const int MAP_WIDTH = windowWidth / TILE_SIZE;   // Nombre de tuiles en largeur
//const int MAP_HEIGHT = windowHeight / TILE_SIZE; // Nombre de tuiles en hauteur
const int MAP_WIDTH = 30;   // Nombre de tuiles en largeur
const int MAP_HEIGHT = 18;	// Nombre de tuiles en hauteur
const int MAX_TILES = MAP_WIDTH * MAP_HEIGHT;     // Nombre total de tuiles dans la carte

//===============================
// Police principale du jeu
//===============================
const std::string PressStart2P = "Assets/PressStart2P-Regular.ttf";

int main()
{
	//===============================
	// Creation de la fenêtre de jeu
	//===============================
    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "Hikari");
	window.setFramerateLimit(60);
	//window.setVerticalSyncEnabled(true);

	//===============================
	// Création du menu principal
	//===============================
	// Police pour le texte du menu principal
	sf::Font mainMenuFont;
	if (!mainMenuFont.openFromFile(PressStart2P))
	{
		std::cerr << "Erreur lors du chargement de la police de texte" << std::endl;
		return -1;
	}

	// Vecteur pour les différents étiquettes du "Main Menu"
	std::vector<std::string> menuLabels = {
		"PLAY", "SETTINGS", "CREDITS", "QUIT",
	};

	// Vecteur pour les différents bouton du "Main Menu"
	std::vector<MenuButton> mainMenu;
	int selectedMainIndex = 0;

	float startXMain = windowWidth * 0.0208f;
	float startYMain = windowHeight * 0.694f;
	float buttonWidthMain = windowWidth * 0.156f;
	float buttonHeightMain = windowHeight * 0.052f;
	float spacingMain = windowHeight * 0.0173f;

	// Taille de la police de l'en-tête
	int MMHeaderSize = calculateSize(70, 960, 576, windowWidth, windowHeight);	// Taille de la police de l'en-tête
	// En-tête "MAIN MENU"
	sf::Text MMHeader(mainMenuFont, "HIKARI", MMHeaderSize);
	MMHeader.setFillColor(sf::Color::White);

	// Positionnement Y de l'en-tête du menu principal
	float MMHeaderOffsetY = calculateSizeY(50.f, 576, windowHeight);	
	MMHeader.setPosition({ (startXMain * 1.5f), MMHeaderOffsetY });

	// Génération les boutons du menu principal
	for (size_t i = 0; i < menuLabels.size(); ++i) {
		MenuButton mainButton(mainMenuFont);

		// Configuration de l'arrière du bouton
		mainButton.background.setSize({ buttonWidthMain, buttonHeightMain });
		mainButton.background.setPosition({ startXMain, startYMain + i * (buttonHeightMain + spacingMain) });

		// Remplissage du texte des boutons
		if (i == 0) {
			mainButton.background.setFillColor(sf::Color::Transparent);
		}
		else {
			mainButton.background.setFillColor(sf::Color::Transparent);
		}

		// Taille de la police du texte à l'intérieur du bouton
		int mainButtonSize = calculateSize(14, 960, 576, windowWidth, windowHeight);	// Taille de la police de l'en-tête
		// Configuration du texte à l'intérieur du bouton
		mainButton.text = sf::Text(mainMenuFont, menuLabels[i], mainButtonSize);
		if (i == 0) {
			mainButton.text.setFillColor(sf::Color::White);
		}
		else {
			mainButton.text.setFillColor(sf::Color::Black);
		}

		float buttonOffSetXMain = calculateSizeX(12.f, 960, windowWidth);
		float buttonOffSetYMain = calculateSizeY(8.f, 576, windowHeight);
		// Centrer le texte verticalement à l'intérieur du rectangle
		mainButton.text.setPosition({ startXMain + buttonOffSetXMain, startYMain + i * (buttonHeightMain + spacingMain) + buttonOffSetYMain });

		mainMenu.push_back(mainButton);
	}
	
	//===============================
	// Création du menu de pause
	//===============================
	// Police pour le texte du menu de pause
	sf::Font pauseMenuFont;
	if (!pauseMenuFont.openFromFile(PressStart2P))
	{
		std::cerr << "Erreur lors du chargement de la police de texte" << std::endl;
		return -1;
	}

	// Vecteur pour les différents étiquettes du "Pause Menu"
	std::vector<std::string> pauseLabels = {
		"RESUME", "SETTINGS", "EXIT TO MAIN MENU",
	};

	// Vecteur pour les différents bouton du "Pause Menu"
	std::vector<MenuButton> pauseMenu;
	int selectedPauseIndex = 0;

	float startX = windowWidth * 0.0208f;
	float startY = windowHeight * 0.1736f;
	float buttonWidth = windowWidth * 0.3125f;
	float buttonHeight = windowHeight * 0.0694f;
	float spacing = windowHeight * 0.0173f;

	// Taille de la police de l'en-tête
	int pauseHeaderSize = calculateSize(30, 960, 576, windowWidth, windowHeight);
	// En-tête "PAUSE MENU"
	sf::Text pauseHeader(pauseMenuFont, "PAUSE MENU", pauseHeaderSize);
	pauseHeader.setFillColor(sf::Color::White);

	// Positionnement Y de l'en-tête du menu de pause
	float pauseHeaderOffsetY = calculateSizeY(30.f, 576, windowHeight);
	pauseHeader.setPosition({ (startX * 1.75f), pauseHeaderOffsetY });

	// Génération les boutons du menu de pause
	for (size_t i = 0; i < pauseLabels.size(); ++i) {
		MenuButton button(pauseMenuFont);

		// Configuration de l'arrière du bouton
		button.background.setSize({ buttonWidth, buttonHeight });
		button.background.setPosition({ startX, startY + i * (buttonHeight + spacing) });

		// Remplissage du texte des boutons
		if (i == 0) {
			button.background.setFillColor(sf::Color(SUBTLE_BLACK));
		}
		else {
			button.background.setFillColor(sf::Color::Transparent);
		}

		int pauseButtonSize = calculateSize(14, 960, 576, windowWidth, windowHeight);	// Taille de la police de l'en-tête
		// Configuration du texte à l'intérieur du bouton
		button.text = sf::Text(pauseMenuFont, pauseLabels[i], pauseButtonSize);
		if (i == 0) {
			button.text.setFillColor(sf::Color::White);
		}
		else {
			button.text.setFillColor(sf::Color::Black);
		}

		float buttonOffSetX = calculateSizeX(15.f, 960, windowWidth);
		float buttonOffSetY = calculateSizeY(14.f, 576, windowHeight);
		// Centrer le texte verticalement à l'intérieur du rectangle
		button.text.setPosition({ startX + buttonOffSetX, startY + i * (buttonHeight + spacing) + buttonOffSetY });

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
	Camera playerCamera(windowWidth, windowHeight, MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, 0.7f);

	//=================================
	// Création de la barre de vie du joueur
	//=================================
	HealthBar playerHealthBar("Assets/healthBarContainer.png", 32, 48, 8, "Assets/fireHealthBar.png", 32, 48, 8, 0.1f);

	//===============================
	// Création de la mini-carte
	//===============================
	MiniMap miniMap((float)(MAP_WIDTH),
					(float)(MAP_HEIGHT),
					(float)windowWidth,
					(float)windowHeight, TILE_SIZE);

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
	GameState currentGameState = GameState::Menu;

	bool isFullscreen = false;

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

			switch (currentGameState) {
			case GameState::Menu:
				// Gérer les événements du menu principal ici
				// Mouse Hover Logic
				if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
					sf::Vector2f mousePos = window.mapPixelToCoords(mouseMoved->position);

					for (size_t i = 0; i < mainMenu.size(); ++i) {
						if (mainMenu[i].background.getGlobalBounds().contains(mousePos)) {
							// Reset old selected button to default look
							mainMenu[selectedMainIndex].background.setFillColor(sf::Color::Transparent);
							mainMenu[selectedMainIndex].text.setFillColor(sf::Color::Black);

							// Highlight new hovered button
							selectedMainIndex = i;

							// Put the new hovered button to highlighted look
							mainMenu[selectedMainIndex].background.setFillColor(sf::Color::Transparent);
							mainMenu[selectedMainIndex].text.setFillColor(sf::Color::White);
						}
					}
				}

				// Mouse Click Logic
				if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
					if (mousePressed->button == sf::Mouse::Button::Left) {
						sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);

						for (size_t i = 0; i < mainMenu.size(); ++i) {
							if (mainMenu[i].background.getGlobalBounds().contains(mousePos)) {
								if (i == 0) currentGameState = GameState::Playing; // Resume
								else if (i == 1) {
									isFullscreen = !isFullscreen;
									if (isFullscreen) {
										window.create(sf::VideoMode::getDesktopMode(), "Hikari",
											sf::Style::Default, sf::State::Fullscreen);
									}
									else {
										window.create(sf::VideoMode({ 1440, 864 }), "Hikari",
											sf::Style::Default, sf::State::Windowed);
									}
									window.setFramerateLimit(60);

									// Retrieve the actual new window dimensions
									windowWidth = window.getSize().x;
									windowHeight = window.getSize().y;

									// Update UI Layout positions and sizes
									recalculateLayout(windowWidth, windowHeight, mainMenuFont,
										pauseMenuFont, MMHeader, mainMenu, pauseHeader, pauseMenu);

									// Update Camera and Minimap dimensions
									playerCamera.updateSize(windowWidth, windowHeight, 0.7f);
									miniMap.updateWindowSize(windowWidth, windowHeight);
								}
								else if (i == 2) ;
								else if (i == 3) window.close();
							}
						}
					}
				}

				// Keyboard Logic (Navigate Pause Menu)
				if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
					if (keyPressed->code == sf::Keyboard::Key::Up) {
						if (selectedMainIndex - 1 >= 0) {
							// Reset old selected button to default look
							mainMenu[selectedMainIndex].background.setFillColor(sf::Color::Transparent);
							mainMenu[selectedMainIndex].text.setFillColor(sf::Color::Black);
							
							// Move to the previous button in the menu
							selectedMainIndex--;
							
							// Put the new hovered button to highlighted look
							mainMenu[selectedMainIndex].background.setFillColor(sf::Color::Transparent);
							mainMenu[selectedMainIndex].text.setFillColor(sf::Color::White);
						}
					}
					else if (keyPressed->code == sf::Keyboard::Key::Down) {
						if (selectedMainIndex + 1 < mainMenu.size()) {
							// Reset old selected button to default look
							mainMenu[selectedMainIndex].background.setFillColor(sf::Color::Transparent);
							mainMenu[selectedMainIndex].text.setFillColor(sf::Color::Black);

							// Move to the next button in the menu
							selectedMainIndex++;

							// Put the new hovered button to highlighted look
							mainMenu[selectedMainIndex].background.setFillColor(sf::Color::Transparent);
							mainMenu[selectedMainIndex].text.setFillColor(sf::Color::White);
						}
					}
					else if (keyPressed->code == sf::Keyboard::Key::Enter) {
						if (selectedMainIndex == 0) currentGameState = GameState::Playing;
						else if (selectedMainIndex == 1) {
							isFullscreen = !isFullscreen;
							if (isFullscreen) {
								window.create(sf::VideoMode::getDesktopMode(), "Hikari",
									sf::Style::Default, sf::State::Fullscreen);
							}
							else {
								window.create(sf::VideoMode({ 1440, 864 }), "Hikari",
									sf::Style::Default, sf::State::Windowed);
							}
							window.setFramerateLimit(60);

							// Retrieve the actual new window dimensions
							windowWidth = window.getSize().x;
							windowHeight = window.getSize().y;

							// Update UI Layout positions and sizes
							recalculateLayout(windowWidth, windowHeight, mainMenuFont,
								pauseMenuFont, MMHeader, mainMenu, pauseHeader, pauseMenu);

							// Update Camera and Minimap dimensions
							playerCamera.updateSize(windowWidth, windowHeight, 0.7f);
							miniMap.updateWindowSize(windowWidth, windowHeight);
						}
						else if (selectedMainIndex == 2) ;
						else if (selectedMainIndex == 3) window.close();
					}
				}
				break;
			// --- GAMEPLAY CONTROLS ---
			case GameState::Playing:
				// Gérer les événements du jeu en cours ici
				if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
					if (keyPressed->code == sf::Keyboard::Key::Escape) {
						// Arreter le jeu et passer au menu de pause
						currentGameState = GameState::PauseMenu;
						
						// Réinitialiser les entrées du joueur pour éviter les mouvements pendant le menu de pause
						playerCharacter.resetInputs();				
					}
				}
				break;
			// --- PAUSE MENU CONTROLS ---
			case GameState::PauseMenu:
				// Gérer les événements du menu de pause ici

				// Mouse Hover Logic
				if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
					sf::Vector2f mousePos = window.mapPixelToCoords(mouseMoved->position);

					for (size_t i = 0; i < pauseMenu.size(); ++i) {
						if (pauseMenu[i].background.getGlobalBounds().contains(mousePos)) {
							// Reset old selected button to default look
							pauseMenu[selectedPauseIndex].background.setFillColor(sf::Color::Transparent);
							pauseMenu[selectedPauseIndex].text.setFillColor(sf::Color::Black);

							// Highlight new hovered button
							selectedPauseIndex = i;

							// Put the new hovered button to highlighted look
							pauseMenu[selectedPauseIndex].background.setFillColor(sf::Color(SUBTLE_BLACK));
							pauseMenu[selectedPauseIndex].text.setFillColor(sf::Color::White);
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
								else if (i == 1) {
									isFullscreen = !isFullscreen;
									if (isFullscreen) {
										window.create(sf::VideoMode::getDesktopMode(), "Hikari",
											sf::Style::Default, sf::State::Fullscreen);
									}
									else {
										window.create(sf::VideoMode({ 1440, 864 }), "Hikari",
											sf::Style::Default, sf::State::Windowed);
									}
									window.setFramerateLimit(60);

									// Retrieve the actual new window dimensions
									windowWidth = window.getSize().x;
									windowHeight = window.getSize().y;

									// Update UI Layout positions and sizes
									recalculateLayout(windowWidth, windowHeight, mainMenuFont,
										pauseMenuFont, MMHeader, mainMenu, pauseHeader, pauseMenu);

									// Update Camera and Minimap dimensions
									playerCamera.updateSize(windowWidth, windowHeight, 0.7f);
									miniMap.updateWindowSize(windowWidth, windowHeight);
								}
								else if (i == 2) currentGameState = GameState::Menu; // Go to main menu
							}
						}
					}
				}

				// Keyboard Logic (Navigate Pause Menu)
				if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Up) {
                        if (selectedPauseIndex - 1 >= 0) {
							// Reset old selected button to default look
                            pauseMenu[selectedPauseIndex].background.setFillColor(sf::Color::Transparent);
							pauseMenu[selectedPauseIndex].text.setFillColor(sf::Color::Black);

							// Move to the previous button in the menu
                            selectedPauseIndex--;

							// Put the new hovered button to highlighted look
							pauseMenu[selectedPauseIndex].background.setFillColor(sf::Color(SUBTLE_BLACK));
                            pauseMenu[selectedPauseIndex].text.setFillColor(sf::Color::White);
                        }
                    } 
                    else if (keyPressed->code == sf::Keyboard::Key::Down) {
                        if (selectedPauseIndex + 1 < pauseMenu.size()) {
							// Reset old selected button to default look
                            pauseMenu[selectedPauseIndex].background.setFillColor(sf::Color::Transparent);
							pauseMenu[selectedPauseIndex].text.setFillColor(sf::Color::Black);

							// Move to the next button in the menu
                            selectedPauseIndex++;

							// Put the new hovered button to highlighted look
							pauseMenu[selectedPauseIndex].background.setFillColor(sf::Color(SUBTLE_BLACK));
                            pauseMenu[selectedPauseIndex].text.setFillColor(sf::Color::White);
                        }
                    } 
                    else if (keyPressed->code == sf::Keyboard::Key::Enter) {
                        if (selectedPauseIndex == 0) currentGameState = GameState::Playing;
                        else if (selectedPauseIndex == 1) {
							isFullscreen = !isFullscreen;
							if (isFullscreen) {
								window.create(sf::VideoMode::getDesktopMode(), "Hikari",
									sf::Style::Default, sf::State::Fullscreen);
							}
							else {
								window.create(sf::VideoMode({ 1440, 864 }), "Hikari",
									sf::Style::Default, sf::State::Windowed);
							}
							window.setFramerateLimit(60);

							// Retrieve the actual new window dimensions
							windowWidth = window.getSize().x;
							windowHeight = window.getSize().y;

							// Update UI Layout positions and sizes
							recalculateLayout(windowWidth, windowHeight, mainMenuFont,
								pauseMenuFont, MMHeader, mainMenu, pauseHeader, pauseMenu);

							// Update Camera and Minimap dimensions
							playerCamera.updateSize(windowWidth, windowHeight, 0.7f);
							miniMap.updateWindowSize(windowWidth, windowHeight);
						}
						else if (selectedPauseIndex == 2) currentGameState = GameState::Menu;
                    }
                }

				// Keyboard Logic (Unpause with ESC)
				if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
					if (keyPressed->code == sf::Keyboard::Key::Escape) {
						currentGameState = GameState::Playing;
					}
				}
				break;
			}

			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->code == sf::Keyboard::Key::H &&
					keyPressed->shift &&
					keyPressed->control) {
					showHitboxes = !showHitboxes;
				}
			}

			/*for (int i = 0; i < players.size(); ++i) {
				players[i].handleEvent(*event);
			}*/

			miniMap.handleEvent(*event);

			// Pour éviter que le joueur ne se déplace lorsqu'il est dans le menu de pause
			if (currentGameState == GameState::Playing) {
				playerCharacter.handleEvent(*event);
				testCharacter.handleEvent(*event);
			}
        }

		sf::FloatRect playerHitbox;
		sf::FloatRect testHitbox;

		if (currentGameState == GameState::Playing) {
			playerCharacter.update(dt, window, sprite);
			testCharacter.update(dt, window, testSprite);

			playerHealthBar.update(playerCharacter.getHealth(), playerCharacter.getMaxHealth(), dt);

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

		if (currentGameState == GameState::Menu) {
			// Dessiner une image de fond pour le menu principal
			/*sf::Texture backgroundTexture;
			if (!backgroundTexture.loadFromFile("Assets/MainMenuBackground.png")) {
				std::cerr << "Erreur lors du chargement de l'image de fond" << std::endl;
				return -1;
			}
			sf::Sprite backgroundSprite(backgroundTexture);
			window.draw(backgroundSprite);*/

			// Dessiner un background sombre pour le menu principal (Temporaire)
			sf::RectangleShape mainMenuBackground(sf::Vector2f(windowWidth, windowHeight));
			mainMenuBackground.setFillColor(sf::Color(58, 58, 58));
			window.draw(mainMenuBackground);

			// Draw Main Menu UI elements
			window.draw(MMHeader);
			for (const auto& button : mainMenu) {
				window.draw(button.background);
				window.draw(button.text);
			}
		}
		// Pour le rendu, on vérifie si le jeu est en cours ou en pause pour dessiner les éléments du jeu
		else if (currentGameState == GameState::Playing || currentGameState == GameState::PauseMenu) {
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

			// Dessiner les hitboxes si l'option est activée
			if (showHitboxes) {
				drawHitboxOutline(window, playerHitbox);
				drawHitboxOutline(window, testHitbox);
			}
			
			// Reset view to default for UI rendering)
			window.setView(window.getDefaultView()); 

			playerHealthBar.draw(window, { 20.f, 20.f });

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

        window.display();
    }
	return 0;
}
