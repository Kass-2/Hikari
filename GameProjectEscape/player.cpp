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

Player::Player(sf::Texture &texture, sf::Sprite &sprite, const std::string& tf)
	: state(PlayerState::IDLE), previousState(PlayerState::IDLE), 
	  direction(Direction::RIGHT)
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

void Player::initialState(std::string characterName) {
	player.name = characterName;
	player.health = 100;
	player.maxHealth = 100;
	player.attackPower = 10;
	player.defense = 5;
	player.speed = 0.5f;
}

void Player::handleInput(sf::Sprite& sprite) {
	// Si le bouton K est pressé, le joueur n'est plus mort
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) boolState.dead = false;

	// Si le joueur est mort, on ne gère pas les entrées clavier
	if (boolState.dead) return;

	sf::Vector2f movement = { 0.f, 0.f };

	// Mémoriser les états des touches pour gérer les cas où les deux touches sont pressées en même temps
	if (leftHeld && rightHeld)
	{
		if (lastHorizontal == Direction::LEFT)
		{
			movement.x = -1.f;
			direction = Direction::LEFT;
			if (!attacking)
			{
				sprite.setScale({ -1.f, 1.f }); // Pour que le personnage regarde vers la gauche
			}
		}
		else
		{
			movement.x = 1.f;
			direction = Direction::RIGHT;
			if (!attacking)
			{
				sprite.setScale({ 1.f, 1.f });	// Pour que le personnage regarde vers la droite
			}
		}
	}
	else if (leftHeld)
	{
		movement.x = -1.f;
		direction = Direction::LEFT;
		if (!attacking)
		{
			sprite.setScale({ -1.f, 1.f }); // Pour que le personnage regarde vers la gauche
		}
	}
	else if (rightHeld)
	{
		movement.x = 1.f;
		direction = Direction::RIGHT;
		if (!attacking)
		{
			sprite.setScale({ 1.f, 1.f });	// Pour que le personnage regarde vers la droite
		}
	}

	// Mémoriser la dernière direction horizontale pour gérer les cas où les deux touches sont pressées en même temps
	if (upHeld && downHeld)
	{
		if (lastVertical == Direction::UP)
		{
			movement.y = -1.f;
			direction = Direction::UP;
		}
		else
		{
			movement.y = 1.f;
			direction = Direction::DOWN;
		}
	}
	else if (upHeld)
	{
		movement.y = -1.f;
		direction = Direction::UP;
	}
	else if (downHeld)
	{
		movement.y = 1.f;
		direction = Direction::DOWN;
	}

	bool moving = (movement.x != 0.f || movement.y != 0.f);

	boolState.run = moving && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift);

	boolState.walk = moving && !boolState.run;

	boolState.idle = !moving && !boolState.attack;

	player.speed = boolState.run ? 1.0f : 0.5f;

	// Normaliser le vecteur de mouvement pour éviter que le personnage ne se déplace plus vite en diagonale
	if (movement.x != 0.f || movement.y != 0.f)
	{
		float length = std::sqrt(
			movement.x * movement.x +
			movement.y * movement.y);

		movement /= length;
	}

	sprite.move(movement * player.speed);

	// Gestion des entrées clavier pour changer l'état du joueur (dommage, mort)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J)) boolState.damaged = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) boolState.dead = true;

	if (attackPressed && !attacking) {
		state = PlayerState::ATTACKING;
		attacking = true;
	}

	attackPressed = false; // Réinitialiser l'état d'attaque après la gestion des entrées clavier
}

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

	// Mise à jour de l'état du joueur en fonction des booléens
	if (boolState.dead) state = PlayerState::DEAD;
	else if (boolState.damaged) state = PlayerState::DAMAGED;
	else if (attacking) state = PlayerState::ATTACKING;
	else if (boolState.run) state = PlayerState::RUNNING;
	else if (boolState.walk) state = PlayerState::WALKING;
	else state = PlayerState::IDLE;

	// Mise à jour de l'animation du joueur en fonction de son état et de sa direction
	updateAnimation(dt, sprite);
}

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
	float frameTime = 
		(state == PlayerState::RUNNING) ? 0.07f :
		(state == PlayerState::WALKING) ? 0.1f :
		(state == PlayerState::ATTACKING) ? 0.03f :
		(state == PlayerState::IDLE) ? 0.2f :
		(state == PlayerState::DAMAGED) ? 0.1f :
		(state == PlayerState::DEAD) ? 0.1f : 0.3f;
	int maxFrames = 
		(state == PlayerState::IDLE) ? 2 :
		(state == PlayerState::WALKING) ? 4 :
		(state == PlayerState::RUNNING) ? 8 :
		(state == PlayerState::ATTACKING) ? 8 :
		(state == PlayerState::DAMAGED) ? 3 :
		(state == PlayerState::DEAD) ? 8 : 1;

	// Mettre à jour l'animation du joueur en fonction du temps écoulé
	if (animationClock.getElapsedTime().asSeconds() >= frameTime) {
		rectSource.position.x += 32;
		if (rectSource.position.x >= maxFrames * 32) {
			// Si l'animation est terminée, on réinitialise la position de la source en fonction de l'état du joueur
			if (state == PlayerState::DEAD)
			{
				rectSource.position.x = (maxFrames - 1) * 32;
			}
			else if (state == PlayerState::DAMAGED)
			{
				rectSource.position.x = 0;
				boolState.damaged = false; // Réinitialiser l'état de dommage après l'animation
			}
			else if (state == PlayerState::ATTACKING)
			{
				// Réinitialiser l'état d'attaque après l'animation
				attacking = false;

				rectSource.position.x = 0;

				state = PlayerState::IDLE; // Revenir à l'état idle après l'animation d'attaque
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

void Player::handleEvent(const sf::Event& event)
{
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		switch (keyPressed->code)
		{
		case sf::Keyboard::Key::A:
		case sf::Keyboard::Key::Left:
			leftHeld = true;
			lastHorizontal = Direction::LEFT;
			break;

		case sf::Keyboard::Key::D:
		case sf::Keyboard::Key::Right:
			rightHeld = true;
			lastHorizontal = Direction::RIGHT;
			break;

		case sf::Keyboard::Key::W:
		case sf::Keyboard::Key::Up:
			upHeld = true;
			lastVertical = Direction::UP;
			break;

		case sf::Keyboard::Key::S:
		case sf::Keyboard::Key::Down:
			downHeld = true;
			lastVertical = Direction::DOWN;
			break;
		}
	}

	if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
	{
		switch (keyReleased->code)
		{
		case sf::Keyboard::Key::A:
		case sf::Keyboard::Key::Left:
			leftHeld = false;
			break;

		case sf::Keyboard::Key::D:
		case sf::Keyboard::Key::Right:
			rightHeld = false;
			break;

		case sf::Keyboard::Key::W:
		case sf::Keyboard::Key::Up:
			upHeld = false;
			break;

		case sf::Keyboard::Key::S:
		case sf::Keyboard::Key::Down:
			downHeld = false;
			break;
		case sf::Keyboard::Key::Space:
		case sf::Keyboard::Key::Enter:
			attackPressed = true;
			break;
		case sf::Keyboard::Key::I:
			inventory = !inventory; // Toggle inventory
			break;
		}
	}

	/*if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mouseButtonPressed->button == sf::Mouse::Button::Left)
		{
			attackPressed = true;
		}
	}*/
}

void Player::draw(sf::RenderWindow& window, sf::Sprite& sprite) {
	window.draw(sprite);
}