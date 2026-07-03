//===============================
// Fichier : Player.cpp
// Description : Implémentation de la classe Player pour gérer le personnage du joueur
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
#include "Player.h"

Player::Player(sf::Texture &texture, sf::Sprite &sprite, const std::string& tf, int frameWidth, int frameHeight)
	: playerFrames({0.3f, 8, 0, -1}), rows ({0, 0, 0, 0})
{
	textureFile = tf;
	fL = frameWidth;
	fH = frameHeight;
	playerFrames.cf = 0;

	// Chargement de la texture du joueur
	if (!texture.loadFromFile(textureFile))
	{
		std::cout << "Failed to load player texture!" << std::endl;
		return;
	}

	sprite.setTexture(texture);

	// Calcul du nombre de colonnes et de lignes dans la texture
	int x = columns * fL;                   // Position X de la partie de la texture à afficher
	int y = lignes * fH;                    // Position Y de la partie de la texture à afficher

	// Sprite du joueur : on affiche la partie de la texture qui correspond au personnage (32x32 pixels)
	sf::IntRect r({ x,y }, { fL,fH });
	rectSource = r;
	sprite.setTextureRect(rectSource);

	sprite.setOrigin({ fL/2.f, fH/2.f });       // Centrer l'origine du sprite
	sprite.setPosition({ 400.f, 300.f });   // Position initiale du joueur au centre de la fenêtre
	sprite.setScale({ 1.0f, 1.0f });        // Modifier la taille du sprite du joueur

	// Initialiser les positions X et Y du rectangle source pour l'animation du joueur
	rectSource.position.x = 0;

	// Les lignes pour l'état idle sont différentes pour chaque direction
	int IdleRow =
		DInfo.direction == Direction::DOWN ? 9 :
		DInfo.direction == Direction::UP ? 10 :
		DInfo.direction == Direction::LEFT ? 12 :
		DInfo.direction == Direction::RIGHT ? 11 : 9;

	IdleRow -= 1;

	// Initialiser la position Y du rectangle source en fonction de la direction du joueur
	rectSource.position.y = IdleRow * fH;

	sprite.setTextureRect(rectSource);

	playerState.previousState = playerState.state;
	DInfo.previousDirection = DInfo.direction;

	animationClock.restart();
}

void Player::setPlayerPosition(sf::Sprite& sprite, float x, float y) {
	// Si le mouvement du joueur est verrouillé, on ne change pas sa position
	if (lock) return;
	sprite.setPosition({ x, y });
}

sf::FloatRect Player::getHitbox(const sf::Sprite& sprite) const {
	// Custom hitbox dimensions (change this to fit your sprite)
	float width = 16.f;
	float height = 24.f;

	sf::Vector2f pos = sprite.getPosition();

	// Calculate the top-left coordinate of the hitbox relative to centered origin
	// This positions the hitbox at the bottom-center of the character's sprite
	return sf::FloatRect(
		{ pos.x - width / 2.f, pos.y - 10.f },
		{ width, height }
	);
}

void Player::initialState(std::string characterName) {
	player.name = characterName;
	player.health = 100;
	player.maxHealth = 100;
	player.attackPower = 10;
	player.defense = 5;
	player.speed = (1.25f * 60);
}

void Player::handleInput(sf::Sprite& sprite, float dt) {
	// Si le mouvement du joueur est verrouillé ou s'il est mort, on ne gère pas les entrées clavier
	if (lock || playerState.dead) return;

	// Vecteur de mouvement du joueur en fonction des entrées clavier
	sf::Vector2f movement = { 0.f, 0.f };

	// Si le joueur est en train d'attaquer, déplacer légèrement le personnage dans la direction de l'attaque
	sprite.move(AInfo.attackVelocity * dt);

	// Appliquer une friction pour réduire progressivement la vitesse d'attaque
	AInfo.attackVelocity *= 0.9f;

	movement = inputManager.handleMovement(sprite, playerState, player, heldState, DInfo, AInfo);

	sprite.move(movement * player.speed * dt);

	if (AInfo.attackPressed && !AInfo.attacking && AInfo.attackCooldown.getElapsedTime().asSeconds() >= AInfo.attackCooldownTime) {
		playerState.state = PlayerState::ATTACKING;
		AInfo.attacking = true;
		AInfo.attackCooldown.restart();

		AInfo.attackDirection = DInfo.direction;
	}

	AInfo.attackPressed = false; // Réinitialiser l'état d'attaque après la gestion des entrées clavier
}

void Player::update(float dt, const sf::RenderWindow& window, sf::Sprite& sprite) {
	// Gestion des entrées clavier pour déplacer le joueur et changer son état
	handleInput(sprite, dt);

	// Empêcher le personnage de sortir de la fenêtre de jeu
	sf::Vector2f pos = sprite.getPosition();
	sf::Vector2u size = window.getSize();

	// Calculer les limites du sprite pour éviter que le personnage ne sorte de la fenêtre
	float halfX = fL / 5.f;
	float halfY = fH / 3.f;

	if (pos.x - halfX < 0) pos.x = halfX;
	if (pos.x + halfX > size.x) pos.x = size.x - halfX;
	if (pos.y - halfY < 0) pos.y = halfY;
	if (pos.y + halfY > size.y) pos.y = size.y - halfY;

	// Appliquer les limites à la position du sprite
	sprite.setPosition(pos);

	// Mise à jour de l'état du joueur en fonction des booléens si le joueur n'est pas verrouillé
	if (!lock) handlePlayerState(playerState, AInfo);

	// Mise à jour de l'animation du joueur en fonction de son état et de sa direction
	updateAnimation(dt, sprite);
}

void Player::updateAnimation(float deltaTime, sf::Sprite& sprite) {
	// Variable pour l'animation du joueur : on utilise la largeur et la hauteur d'une frame pour calculer les positions dans la texture
	Animations playerAnimation(fH, fL);

	// Determiner les lignes de la texture pour chaque état du joueur en fonction de la direction du joueur
	handleRows(DInfo, rows);

	// Déterminer la ligne cible dans la feuille de sprites pour l'état actuel
	int targetRow = 0;
	if (playerState.state == PlayerState::IDLE)
		targetRow = rows.idleRow;
	else if (playerState.state == PlayerState::WALKING)
		targetRow = rows.walkingRow;
	else if (playerState.state == PlayerState::RUNNING)
		targetRow = rows.runningRow;
	else if (playerState.state == PlayerState::ATTACKING)
		targetRow = rows.attackingRow;
	else if (playerState.state == PlayerState::DAMAGED)
		targetRow = rows.damagedRow;
	else if (playerState.state == PlayerState::DEAD)
		targetRow = rows.deadRow;

	// Vérifier si la ligne de la texture a changé
	bool rowChanged = !AInfo.attacking && (rectSource.position.y != targetRow * fH);

	// Déterminer si l'animation doit être réinitialisée
	bool animationChanged =
		(playerState.state != playerState.previousState)
		|| (!AInfo.attacking && DInfo.direction != DInfo.previousDirection)
		|| rowChanged;

	//// Déterminer si l'animation doit être réinitialisée en fonction du changement d'état ou de direction du joueur
	//bool animationChanged =
	//	(playerState.state != playerState.previousState)
	//	|| (!AInfo.attacking && DInfo.direction != DInfo.previousDirection);

	// Déterminer la ligne de la texture à utiliser en fonction de l'état du joueur
	resetPlayer(animationChanged, sprite, rows, playerAnimation);

	// Déterminer le temps entre les frames et le nombre de frames en fonction de l'état du joueur
	if (playerState.state == PlayerState::RUNNING) {
		playerFrames.ft = 0.07f;
		playerFrames.mf = 8;
	}
	else if (playerState.state == PlayerState::WALKING) {
		playerFrames.ft = 0.1f;
		playerFrames.mf = 8;
	}
	else if (playerState.state == PlayerState::ATTACKING) {
		playerFrames.ft = 0.06f;
		playerFrames.mf = 8;
	}
	else if (playerState.state == PlayerState::IDLE) {
		playerFrames.ft = 0.2f;
		playerFrames.mf = 8;
	}
	else if (playerState.state == PlayerState::DAMAGED) {
		playerFrames.ft = 0.05f;
		playerFrames.mf = 4;
	}
	else if (playerState.state == PlayerState::DEAD) {
		playerFrames.ft = 0.1f;
		playerFrames.mf = 7;
	}
	else {
		playerFrames.ft = 0.3f;
		playerFrames.mf = 8;
	}

	// Mettre à jour l'animation du joueur en fonction du temps écoulé
	if (animationClock.getElapsedTime().asSeconds() >= playerFrames.ft) {
		rectSource.position.x += fL;

		// Mettre à jour les frames précédentes et actuelles de l'animation du joueur
		playerFrames.cf = rectSource.position.x / fL;

		// Pendant l'attaque, maintenir la direction précédente pour que le personnage ne change pas de direction pendant l'animation d'attaque
		if (AInfo.attacking)
		{
			// Mémoriser la direction de l'attaque pour éviter que le personnage ne change de direction pendant l'animation d'attaque
			DInfo.direction = AInfo.attackDirection;

			// De plus, déplacer légèrement le personnage dans la direction de l'attaque pendant les premières frames
			// dans la direction de l'attaque pour donner une impression de mouvement
			if (playerFrames.cf == 2 && playerFrames.pf != 2)
			{
				float dashSpeed = 50.f;

				switch (DInfo.direction)
				{
				case Direction::LEFT:
					AInfo.attackVelocity = { -dashSpeed, 0.f };
					break;

				case Direction::RIGHT:
					AInfo.attackVelocity = { dashSpeed, 0.f };
					break;

				case Direction::UP:
					AInfo.attackVelocity = { 0.f, -dashSpeed };
					break;

				case Direction::DOWN:
					AInfo.attackVelocity = { 0.f, dashSpeed };
					break;
				}
			}
		}

		// TODO : Revoir ce code
		if (playerFrames.cf >= 8)
		{
			// Remettre le personnage un peu en arrière à la fin de l'animation
			sprite.move(-AInfo.attackVelocity * 0.5f); // Ajustez la valeur pour contrôler la distance de recul)
		}

		if (rectSource.position.x >= playerFrames.mf * fL) {
			// Si l'animation est terminée, on réinitialise la position de la source en fonction de l'état du joueur
			if (playerState.state == PlayerState::DEAD)
			{
				rectSource.position.x = (playerFrames.mf - 1) * fL;
			}
			else if (playerState.state == PlayerState::DAMAGED)
			{
				rectSource.position.x = 0;
				playerState.damaged = false; // Réinitialiser l'état de dommage après l'animation
			}
			else if (playerState.state == PlayerState::ATTACKING)
			{
				// Réinitialiser l'état d'attaque après l'animation
				AInfo.attacking = false;

				rectSource.position.x = 0;

				playerState.state = PlayerState::IDLE; // Revenir à l'état idle après l'animation d'attaque
			}
			else
			{
				rectSource.position.x = 0;
			}
		}
		sprite.setTextureRect(rectSource);
		animationClock.restart();
	}
	playerFrames.pf = playerFrames.cf;
}

void Player::handlePlayerState(PlayerStates& playerState, AttackInfo& AInfo) const {
	if (playerState.dead) playerState.state = PlayerState::DEAD;
	else if (playerState.damaged) playerState.state = PlayerState::DAMAGED;
	else if (AInfo.attacking) playerState.state = PlayerState::ATTACKING;
	else if (playerState.run) playerState.state = PlayerState::RUNNING;
	else if (playerState.walk) playerState.state = PlayerState::WALKING;
	else playerState.state = PlayerState::IDLE;
}

void Player::handleRows(DirectionInfo& DInfo, SpriteRows& rows) const {
	// Déterminer la direction horizontale active en cas de conflit (A et D pressés en même temps)
	bool isMovingLeft = heldState.leftHeld && (!heldState.rightHeld || DInfo.
	lastHorizontal == Direction::LEFT);
	bool isMovingRight = heldState.rightHeld && (!heldState.leftHeld || DInfo.
		lastHorizontal == Direction::RIGHT);

	// Les lignes pour l'état d'attaque sont différentes pour chaque direction
	rows.attackingRow =
		DInfo.direction == Direction::DOWN ? 1 :
		DInfo.direction == Direction::UP ? 2 :
		DInfo.direction == Direction::LEFT ? 4 :
		DInfo.direction == Direction::RIGHT ? 3 : 1;
	// Les lignes pour l'état de marche sont différentes pour chaque direction
	rows.walkingRow =
		DInfo.direction == Direction::DOWN ? 33 :
		DInfo.direction == Direction::UP ? 34 :
		DInfo.direction == Direction::LEFT ? 36 :
		DInfo.direction == Direction::RIGHT ? 35 : 33;
	// Les lignes pour l'état de course sont différentes pour chaque direction
	rows.runningRow =
		DInfo.direction == Direction::DOWN ? 29 :
		DInfo.direction == Direction::UP ? 30 :
		DInfo.direction == Direction::LEFT ? 32 :
		DInfo.direction == Direction::RIGHT ? 31 : 29;
	// Les lignes pour l'état idle sont différentes pour chaque direction
	rows.idleRow =
		DInfo.direction == Direction::DOWN ? 25 :
		DInfo.direction == Direction::UP ? 26 :
		DInfo.direction == Direction::LEFT ? 28 :
		DInfo.direction == Direction::RIGHT ? 27 : 25;

	// Les lignes pour l'état de dommage sont différentes pour chaque direction
	rows.damagedRow =
		DInfo.direction == Direction::DOWN ? 21 :
		DInfo.direction == Direction::UP ? 22 :
		DInfo.direction == Direction::LEFT ? 24 :
		DInfo.direction == Direction::RIGHT ? 23 : 21;

	// Les lignes pour l'état de mort sont différentes pour chaque direction
	rows.deadRow =
		DInfo.direction == Direction::DOWN ? 13 :
		DInfo.direction == Direction::UP ? 14 :
		DInfo.direction == Direction::LEFT ? 16 :
		DInfo.direction == Direction::RIGHT ? 15 : 13;

	// Soustraire 1 à chaque ligne pour correspondre à l'index de la texture (commence à 0)
	if (rows.attackingRow > 0)
		rows.attackingRow -= 1;
	if (rows.walkingRow > 0)
		rows.walkingRow -= 1;
	if (rows.runningRow > 0)
		rows.runningRow -= 1;
	if (rows.idleRow > 0)
		rows.idleRow -= 1;
	if (rows.damagedRow > 0)
		rows.damagedRow -= 1;
	if (rows.deadRow > 0)
		rows.deadRow -= 1;
}

//===============================
// Fonction pour réinitialiser l'animation du joueur lorsque son état ou sa direction change
//===============================
void Player::resetPlayer(bool animationChanged, sf::Sprite& sprite, SpriteRows rows, Animations reset) {
	// Si l'état du joueur a changé, on réinitialise l'animation

	if (animationChanged)
	{
		if (playerState.state == PlayerState::IDLE)
			reset.resetAnimation(rectSource, rows.idleRow);
		else if (playerState.state == PlayerState::WALKING)
			reset.resetAnimation(rectSource, rows.walkingRow);
		else if (playerState.state == PlayerState::RUNNING)
			reset.resetAnimation(rectSource, rows.runningRow);
		else if (playerState.state == PlayerState::ATTACKING)
			reset.resetAnimation(rectSource, rows.attackingRow);
		else if (playerState.state == PlayerState::DAMAGED)
			reset.resetAnimation(rectSource, rows.damagedRow);
		else if (playerState.state == PlayerState::DEAD)
			reset.resetAnimation(rectSource, rows.deadRow);

		sprite.setTextureRect(rectSource);

		animationClock.restart();
		playerState.previousState = playerState.state;
		DInfo.previousDirection = DInfo.direction;
	}
}

void Player::handleEvent(const sf::Event& event)
{
	// Si le mouvement du joueur est verrouillé, on ne gère pas les événements clavier
	if (lock) return;

	inputManager.handleEvent(event, heldState, DInfo, AInfo, playerState, inventory);
}

sf::Vector2f Player::getPosition(const sf::Sprite& sprite) const
{
	return sprite.getPosition();
}

void Player::draw(sf::RenderWindow& window, sf::Sprite& sprite) {
	window.draw(sprite);
}