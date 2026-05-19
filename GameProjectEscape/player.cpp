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
// Initialisation des informations du joueur (nom, santé, puissance d'attaque, défense, vitesse, etc.)
//===============================
void Player::initialState(std::string characterName) {
	player.name = characterName;
	player.health = 100;
	player.maxHealth = 100;
	player.attackPower = 10;
	player.defense = 5;
	player.speed = 5;
}

//===============================
// Gestion des entrées clavier pour déplacer le joueur et changer son état
//===============================
void Player::handleInput(sf::Sprite& sprite) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) boolState.dead = false; // Si le joueur est vivant, on réinitialise l'état de mort

	if (boolState.dead) return; // Si le joueur est endommagé ou mort, on ne gère pas les entrées clavier

	sf::Vector2f movement = { 0.f, 0.f };

	bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);

	bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

	bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);

	bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);

	if (left && !right)
	{
		movement.x = -1.f;
		direction = Direction::LEFT;
	}
	else if (right && !left)
	{
		movement.x = 1.f;
		direction = Direction::RIGHT;
	}

	if (up && !down)
	{
		movement.y = -1.f;
	}
	else if (down && !up)
	{
		movement.y = 1.f;
	}

	bool moving = (movement.x != 0.f || movement.y != 0.f);

	boolState.run = moving && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift);

	boolState.attack = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	boolState.walk = moving && !boolState.run;

	boolState.idle = !moving && !boolState.attack;

	speed = boolState.run ? 0.7f : 0.3f;

	sprite.move(movement * speed);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J)) boolState.damaged = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) boolState.dead = true;
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
	if (boolState.dead) state = PlayerState::DEAD;
	else if (boolState.damaged) state = PlayerState::DAMAGED;
	else if (boolState.attack) state = PlayerState::ATTACKING;
	else if (boolState.run) state = PlayerState::RUNNING;
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
			state == PlayerState::RUNNING ? 3 * 32 :
			state == PlayerState::ATTACKING ? 8 * 32 :
			state == PlayerState::DAMAGED ? 6 * 32 :
			state == PlayerState::DEAD ? 7 * 32 : 0);

		sprite.setTextureRect(rectSource);

		animationClock.restart();
		previousState = state;
	}

	// Déterminer le temps entre les frames et le nombre de frames en fonction de l'état du joueur
	float frameTime = (state == PlayerState::RUNNING) ? 0.09f :
					  (state == PlayerState::ATTACKING) ? 0.09f :
					  (state == PlayerState::DAMAGED) ? 0.1f :
					  (state == PlayerState::DEAD) ? 0.1f : 0.3f;
	int maxFrames = (state == PlayerState::IDLE) ? 2 :
					(state == PlayerState::WALKING) ? 4 :
					(state == PlayerState::RUNNING) ? 8 : 
					(state == PlayerState::ATTACKING) ? 8 :
					(state == PlayerState::DAMAGED) ? 3 :
					(state == PlayerState::DEAD) ? 8 : 1;
	// Mettre à jour l'animation du joueur en fonction du temps écoulé
	if (animationClock.getElapsedTime().asSeconds() >= frameTime) {
		rectSource.position.x += 32;
		if (rectSource.position.x >= maxFrames * 32) {
			if (state == PlayerState::DEAD)
			{
				rectSource.position.x = (maxFrames - 1) * 32;
			}
			else if (state == PlayerState::DAMAGED)
			{
				rectSource.position.x = 0;
				boolState.damaged = false; // Réinitialiser l'état de dommage après l'animation
			}
			else
			{
				rectSource.position.x = 0;
			}
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