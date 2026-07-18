//===============================
// Fichier : GameUI.h
// Description : Implémentation de la classe GameUI pour gérer l'interface de jeu (UI and UX)
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include <string>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "MiniMap.h"
#include "Camera.h"

// Énumération pour les états du jeu
enum class GameState {
	Menu,
	PauseMenu,
	Playing
};

// Énumération pour les différents menu du jeu
enum class MenuIndex {
	MAIN,
	PAUSE,
};

// Structure pour stocker les éléments du menu
struct MenuButton {
	sf::RectangleShape background;
	sf::Text text;

	MenuButton(const sf::Font& font) : text(font) {}
};

struct OldButtonData {
	float initialButtonX;
	float initialButtonY;
	float buttonW;
	float buttonH;
	int buttonTextSize;
	float textOffSetX = 12.f;
	float textOffSetY = 8.f;
	int oldWWidth = 960;
	int oldWHeight = 576;
};

//===============================
// Police principale du jeu
//===============================
const std::string PressStart2P = "Assets/PressStart2P-Regular.ttf";

//===============================
// Constantes de couleurs
//===============================
sf::Color WHITE_LESS(224, 224, 224);
sf::Color SUBTLE_WHITE(255, 255, 255, 38); 
sf::Color SUBTLE_BLACK(0, 0, 0, 38);
sf::Color WHITE_GRAY(86, 86, 86);

class GameUI {
public:
	//===============================
	// Constructeur pour l'interface
	//===============================
	GameUI(std::string fontPath, std::vector<std::string> menuLabels, 
		std::string name, int size, unsigned int wWidth, unsigned wHeight);

	//==============================
	// Fonction pour initialiser la couleur des boutons (texte et background)
	//==============================
	void buttonColors(sf::Color bColor, sf::Color tColor, sf::Color bFill, sf::Color tFill);

	//==============================
	// Fonction pour initialiser la couleur et la position de l'en-tête
	//==============================
	void initializeHeader(sf::Color headerColor, float offsetMultiplierX, float positionMultiplierY);

	//==============================
	// Fonction pour initialiser la position des boutons, leur taille et leur espacement
	//==============================
	void initializeButtons(float positionX, float positionY, float buttonW, float buttonH, int buttonTextSize, float textOffSetX, float textOffSetY, int oldWWidth, int oldWHeight);

	//==============================
	// Fonction pour repositionner les menus
	//==============================
	//void resetMultiUIPosition(std::vector<GameUI&> menus, unsigned int newWindowWidth, unsigned int newWindowHeight);

	//==============================
	// Fonction pour repositionner le menu
	//==============================
	void resetUIPosition(unsigned int newWindowWidth, unsigned int newWindowHeight, float headerPositionX, float headerPositionY);

	//==============================
	// Fonction pour activer les fonctionnalité de la souris (mouse hover et mouse click)
	//==============================
	void activateMouseUI(const sf::Event& event, sf::RenderWindow& window, 
		Camera& playerCamera, MiniMap& miniMap, GameState& currentGameState, 
		bool& isFullScreen);

	//==============================
	// Fonction pour activer les fonctionnalité des différents menus
	//==============================
	void executeMenu(int index, sf::RenderWindow& window, Camera playerCamera, MiniMap minimap);

	//==============================
	// Fonction pour activer ou désactiver le pleine écran
	//==============================
	void toggleFullScreen(bool& isFullScreen, sf::RenderWindow& window, Camera playerCamera, MiniMap minimap);

	//==============================
	// Fonction pour résumer le jeu
	//==============================
	void resumeGame();

	//==============================
	// Fonction pour avoir la position X des boutons
	//==============================
	float getStartX() const;
private:
	//==============================
	// Fonction pour initialiser la position des boutons (texte et background)
	//==============================
	void initializeBPosition(int buttonTextSize, float textOffSetX, float textOffSetY, int oldWWidth, int oldWHeight);

	//===============================

	unsigned int windowWidth;			// Largeur de la fenêtre
	unsigned windowHeight;				// Hauteur de la fenêtre

	sf::Font font;						// Police du menu

	std::string menuHeaderName;			// Nom de l'en-tête du menu
	int menuHeaderSize;					// Taille de l'en-tête du menu
	sf::Text menuHeader;				// En-tête
	sf::Color menuHeaderColor;			// Couleur de l'en-tête

	sf::Color backgroundColor;			// Couleur arrière du bouton
	sf::Color textColor;				// Couleur du texte du bouton
	sf::Color backgroundFill;			// Couleur arrière du bouton (Remplie)
	sf::Color textFill;					// Couleur du texte du bouton (Remplie)

	std::vector<std::string> labels;	// Étiquette du menu

	std::vector<MenuButton> menu;		// Vecteur pour les différents bouton du menu
	int selectedIndex = 0;				// Index sélectionner par l'utilisateur

	float startX;						// Position en X du menu
	float startY;						// Position en Y du menu
	float buttonWidth;					// Largeur d'un bouton
	float buttonHeight;					// Hauteur d'un bouton
	float spacing;						// Espace entre chaque bouton

	float initialButtonX;				// Position X du premier bouton du menu
	float initialButtonY;				// Position Y du premier bouton du menu

	OldButtonData oldButtonData;		// Ancienne données d'un boutons à l'initialisation pour facilité le replacement

	MenuIndex menuIndex;				// Index pour les différents menus

	GameState gameState;				// État du jeu

	bool fullScreen;					// Toggle pour le pleine écran
};
