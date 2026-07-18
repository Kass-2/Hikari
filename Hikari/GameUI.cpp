//===============================
// Fichier : GameUI.cpp
// Description : Implémentation de la classe GameUI pour gérer l'interface de jeu (UI and UX)
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
#include <iostream>
#include <vector>
#include <string>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>

#include "GameUI.h"
#include "Functions.h"
#include "MiniMap.h"
#include "Camera.h"


GameUI::GameUI(std::string fontPath, std::vector<std::string> menuLabels, std::string name, int size, unsigned int wWidth, unsigned wHeight)
	: labels(menuLabels), menuHeaderName(name), menuHeaderSize(size), 
	menuHeader(font, menuHeaderName, menuHeaderSize), 
	windowWidth(wWidth), windowHeight(wHeight)
{
	if (!font.openFromFile(fontPath))
	{
		std::cerr << "Erreur lors du chargement de la police de texte" << std::endl;
		return;
	}
}

void GameUI::buttonColors(sf::Color bColor, sf::Color tColor, sf::Color bFill, sf::Color tFill) {
	sf::Color backgroundColor = bColor;
	sf::Color textColor = tColor;
	sf::Color backgroundFill = bFill;
	sf::Color textFill = tFill;
}

void GameUI::initializeHeader(sf::Color headerColor, float headerPositionX, float headerPositionY) {
	menuHeaderColor = headerColor;

	// À Continuer avec le header et tout...

	// Mets la couleur de l'en-tête
	menuHeader.setFillColor(menuHeaderColor);

	// Positionnement de l'en-tête du menu
	menuHeader.setPosition({ headerPositionX, headerPositionY });
}

void GameUI::initializeButtons(float positionX, float positionY, 
	float buttonW, float buttonH, int buttonTextSize, 
	float textOffSetX = 12.f, float textOffSetY = 8.f,
	int oldWWidth = 960, int oldWHeight = 576)
{
	// Enregistrer les données du bouton pour pouvoir les réutilisés
	oldButtonData.initialButtonX = positionX;
	oldButtonData.initialButtonY = positionY;
	oldButtonData.buttonW = buttonW;
	oldButtonData.buttonH = buttonH;
	oldButtonData.buttonTextSize = buttonTextSize;
	oldButtonData.textOffSetX = textOffSetX;
	oldButtonData.textOffSetY = textOffSetY;

	// Calculer la position et taille d'un bouton
	startX = calculatePositionX(positionX, oldWWidth, windowWidth);
	startY = calculatePositionY(positionY, oldWHeight, windowHeight);
	buttonWidth = calculatePositionX(buttonW, oldWWidth, windowWidth);
	buttonHeight = calculatePositionY(buttonH, oldWHeight, windowHeight);
	spacing = calculatePositionY(buttonH, oldWHeight, windowHeight);

	initializeBPosition(buttonTextSize, textOffSetX, textOffSetY, oldWWidth, oldWHeight);
}

void GameUI::initializeBPosition(int buttonTextSize, float textOffSetX, float textOffSetY, int oldWWidth, int oldWHeight) {
	// Placement des boutons du menu
	for (size_t i = 0; i < labels.size(); ++i) {
		MenuButton button(font);

		// Configuration de l'arrière du bouton
		button.background.setSize({ buttonWidth, buttonHeight });
		button.background.setPosition({ startX, startY + i * (buttonHeight + spacing) });

		// Remplissage du texte des boutons
		if (i == 0) {
			button.background.setFillColor(backgroundFill);
		}
		else {
			button.background.setFillColor(backgroundColor);
		}

		// Taille de la police du texte à l'intérieur du bouton
		int buttonSize = calculateSize(14, 960, 576, windowWidth, windowHeight);

		// Configuration du texte à l'intérieur du bouton
		button.text = sf::Text(font, labels[i], buttonSize);
		if (i == 0) {
			button.text.setFillColor(textFill);
		}
		else {
			button.text.setFillColor(textColor);
		}

		float buttonOffSetX = calculateSizeX(textOffSetX, oldWWidth, windowWidth);
		float buttonOffSetY = calculateSizeY(textOffSetY, oldWHeight, windowHeight);

		// Centrer le texte verticalement à l'intérieur du rectangle
		button.text.setPosition({ startX + buttonOffSetX, startY + i * (buttonHeight + spacing) + buttonOffSetY });

		menu.push_back(button);
	}
}

//void GameUI::resetMultiUIPosition(std::vector<GameUI&> menus, unsigned int newWindowWidth, unsigned int newWindowHeight) {
//	// Repositonner plusieurs menus
//	for (size_t i = 0; i < menus.size(); ++i) {
//		menus[i].resetUIPosition(newWindowWidth, newWindowHeight);
//	}
//}

void GameUI::resetUIPosition(unsigned int newWindowWidth, unsigned int newWindowHeight, float headerPositionX, float headerPositionY) {
	initializeHeader(menuHeaderColor, headerPositionX, headerPositionY);

	initializeButtons(oldButtonData.initialButtonX, oldButtonData.initialButtonY,
		oldButtonData.buttonW, oldButtonData.buttonH, oldButtonData.buttonTextSize,
		oldButtonData.textOffSetX, oldButtonData.textOffSetY);
}

void GameUI::activateMouseUI(const sf::Event& event, sf::RenderWindow& window, 
	Camera& playerCamera, MiniMap& miniMap, GameState &currentGameState, 
	bool &isFullScreen)
{
	gameState = currentGameState;
	fullScreen = isFullScreen;

	// Mouse Hover Logic
	if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
		sf::Vector2f mousePos = window.mapPixelToCoords(mouseMoved->position);

		for (size_t i = 0; i < menu.size(); ++i) {
			if (menu[i].background.getGlobalBounds().contains(mousePos)) {
				// Reset old selected button to default look
				menu[selectedIndex].background.setFillColor(backgroundColor);
				menu[selectedIndex].text.setFillColor(textColor);

				// Highlight new hovered button
				selectedIndex = i;

				// Put the new hovered button to highlighted look
				menu[selectedIndex].background.setFillColor(backgroundFill);
				menu[selectedIndex].text.setFillColor(textFill);
			}
		}
	}

	// Mouse Click Logic
	if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mousePressed->button == sf::Mouse::Button::Left) {
			sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);

			for (size_t i = 0; i < menu.size(); ++i) {
				if (menu[i].background.getGlobalBounds().contains(mousePos)) {
					executeMenu(i, window, playerCamera, miniMap);
					currentGameState = gameState;



					if (i == 0) currentGameState = GameState::Playing; // Resume
					else if (i == 1) {
						isFullScreen = !isFullScreen;
						if (isFullScreen) {
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
						/*resetUIPosition(windowWidth, windowHeight);

						recalculateLayout(windowWidth, windowHeight, mainMenuFont,
							pauseMenuFont, MMHeader, menu, pauseHeader, pauseMenu);*/

						// Update Camera and Minimap dimensions
						playerCamera.updateSize(windowWidth, windowHeight, 0.7f);
						miniMap.updateWindowSize(windowWidth, windowHeight);
					}
					else if (i == 2);
					else if (i == 3) window.close();
				}
			}
		}
	}
}

void GameUI::activateKeyUI() {

}

void GameUI::executeMenu(int index, sf::RenderWindow& window, Camera playerCamera, MiniMap minimap) {
	switch (menuIndex) {
	case MenuIndex::MAIN:
		if (index == 0) resumeGame(); // Resume
		else if (index == 1) toggleFullScreen(fullScreen, window, playerCamera, minimap);
		else if (index == 2);
		else if (index == 3) window.close();
		break;

	case MenuIndex::PAUSE:

		break;
	}
}

void GameUI::toggleFullScreen(bool& isFullScreen, sf::RenderWindow& window, Camera playerCamera, MiniMap minimap) {
	isFullScreen = !isFullScreen;
	if (isFullScreen) {
		window.create(sf::VideoMode::getDesktopMode(), "Hikari",
			sf::Style::Default, sf::State::Fullscreen);
	}
	else {
		window.create(sf::VideoMode({ getWindowWidth(), getWindowHeight()}), "Hikari",
			sf::Style::Default, sf::State::Windowed);
	}
	window.setFramerateLimit(60);

	// Retrieve the actual new window dimensions
	windowWidth = window.getSize().x;
	windowHeight = window.getSize().y;

	// Update UI Layout positions and sizes
	resetUIPosition(windowWidth, windowHeight, );

	// Update Camera and Minimap dimensions
	playerCamera.updateSize(windowWidth, windowHeight, 0.7f);
	minimap.updateWindowSize(windowWidth, windowHeight);
}

void GameUI::resumeGame() {
	gameState = GameState::Playing;
}

float GameUI::getStartX() const
{
	return startX;
}