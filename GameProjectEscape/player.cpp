//===============================
// Fichier : player.cpp
// Description : Implémentation de la classe Player pour gérer le personnage du joueur
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
#include "player.h"
#include <iostream>

//===============================
// Création du sprite du joueur
//===============================
Player::Player(sf::Texture &texture, sf::Sprite &sprite, const std::string& tf)
	: state(PlayerState::IDLE), previousState(PlayerState::IDLE), 
	  direction(Direction::RIGHT), speed(0.3f)
{
	textureFile = tf;

	// Chargement de la texture du joueur
	if (!texture.loadFromFile(textureFile))
	{
		std::cout << "Failed to load player texture!" << std::endl;
		return;
	}

	sprite.setTexture(texture);

	// Paramètres pour la partie de la texture à afficher (correspondant au personnage)
	int fL = 32;                            // FrameLargeur : Largeur de la partie de la texture à afficher
	int fH = 32;                            // FrameHauteur : Hauteur de la partie de la texture à afficher
	int columns = 0;                        // Nombre de colonnes dans la texture
	int lignes = 0;                         // Nombre de lignes dans la texture

	// Calcul du nombre de colonnes et de lignes dans la texture
	int x = columns * fL;                   // Position X de la partie de la texture à afficher
	int y = lignes * fH;                    // Position Y de la partie de la texture à afficher

	// Sprite du joueur : on affiche la partie de la texture qui correspond au personnage (32x32 pixels)
	sf::IntRect r({ x,y }, { fL,fH });
	rectSource = r;
	sprite.setTextureRect(rectSource);

	sprite.setOrigin({ 16.f, 16.f });       // Centrer l'origine du sprite
	sprite.setPosition({ 400.f, 300.f });   // Position initiale du joueur au centre de la fenêtre
	sprite.setScale({ 1.0f, 1.0f });        // Modifier la taille du sprite du joueur
}

//===============================
// Gestion des entrées clavier pour déplacer le joueur et changer son état
//===============================
void Player::handleInput(sf::Sprite& sprite) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		sprite.move({ -speed, 0.f });
		boolState.walk = true; boolState.idle = false;
		direction = Direction::LEFT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		sprite.move({ speed, 0.f });
		boolState.walk = true; boolState.idle = false;
		direction = Direction::RIGHT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		sprite.move({ 0.f, -speed });
		boolState.walk = true; boolState.idle = false;
		direction = Direction::UP;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		sprite.move({ 0.f, speed });
		boolState.walk = true; boolState.idle = false;
		direction = Direction::DOWN;
	}

	if (boolState.walk && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
	{
		speed = 0.7f; boolState.run = true;
	}
	else
	{
		speed = 0.3f; boolState.run = false;
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		boolState.walk = false; boolState.idle = true;
	}
}

//===============================
// Mise à jour de la position du joueur en fonction des entrées clavier et de l'état du joueur
//===============================
void Player::update(float dt, const sf::RenderWindow& window, sf::Sprite& sprite) {
	// Gestion des entrées clavier pour déplacer le joueur et changer son état
	handleInput(sprite);

	// Empêcher le personnage de sortir de la fenêtre de jeu
	sf::Vector2f pos = sprite.getPosition();
	sf::Vector2u size = window.getSize();

	float half = 16.f;

	if (pos.x - half < 0) pos.x = half;
	if (pos.x + half > size.x) pos.x = size.x - half;
	if (pos.y - half < 0) pos.y = half;
	if (pos.y + half > size.y) pos.y = size.y - half;

	sprite.setPosition(pos);

	if (direction == Direction::RIGHT)
		sprite.setScale({ 1.f, 1.f });	// Pour que le personnage regarde vers la droite
	else if (direction == Direction::LEFT)
		sprite.setScale({ -1.f, 1.f }); // Pour que le personnage regarde vers la gauche

	// Mise à jour de l'état du joueur en fonction des booléens
	if (boolState.run) state = PlayerState::RUNNING;
	else if (boolState.walk) state = PlayerState::WALKING;
	else state = PlayerState::IDLE;

	// Mise à jour de l'animation du joueur en fonction de son état et de sa direction
	updateAnimation(dt, sprite);
}

//===============================
// Mise à jour de l'animation du joueur en fonction de son état et de sa direction
//===============================
void Player::updateAnimation(float deltaTime, sf::Sprite& sprite) {
	// Si l'état du joueur a changé, on réinitialise l'animation
	if (state != previousState)
	{
		rectSource.position.x = 0;
		rectSource.position.y = (
			state == PlayerState::IDLE ? 0 :
			state == PlayerState::WALKING ? 2 * 32 :
			state == PlayerState::RUNNING ? 3 * 32 : 0);

		sprite.setTextureRect(rectSource);

		animationClock.restart();
		previousState = state;
	}

	// Déterminer le temps entre les frames et le nombre de frames en fonction de l'état du joueur
	float frameTime = (state == PlayerState::RUNNING) ? 0.1f : 0.3f;
	int maxFrames = (state == PlayerState::IDLE) ? 2 :
					(state == PlayerState::WALKING) ? 4 :
					(state == PlayerState::RUNNING) ? 8 : 1;

	// Mettre à jour l'animation du joueur en fonction du temps écoulé
	if (animationClock.getElapsedTime().asSeconds() >= frameTime) {
		rectSource.position.x += 32;
		if (rectSource.position.x >= maxFrames * 32) {
			rectSource.position.x = 0;
		}
		sprite.setTextureRect(rectSource);
		animationClock.restart();
	}
}

//===============================
// Affichage du sprite du joueur à l'écran
//===============================
void Player::draw(sf::RenderWindow& window, sf::Sprite& sprite) {
	window.draw(sprite);
}