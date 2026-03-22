#include <iostream>

#include <SFML/Graphics.hpp>

// Enumération pour les différents états du joueur
enum class PlayerState
{
	IDLE,
	WALKING,
	RUNNING,
	ATTACKING,
};

// Enumération pour les différentes directions du joueur
enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum class KeyState
{
	PRESSED,
	RELEASED
};

enum class Keys
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SHIFT
};

// Structure pour stocker les différents états du joueur sous forme de booléens
struct boolPlayerState
{
	bool walk = false;
	bool idle = false;
	bool run = false;
	bool attack = false;
};

/* Fonction pour gérer les entrées clavier et mettre à jour les états du joueur */
void keyboardInput(boolPlayerState& boolState, Direction& direction, sf::Sprite& player, float& speed)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		if (boolState.run)
			speed = 0.5f; // Vitesse de course

		player.move({ -speed, 0.f });
		boolState.walk = true; boolState.idle = false;
		direction = Direction::LEFT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		if (boolState.run)
			speed = 0.5f; // Vitesse de course

		player.move({ speed, 0.f });
		boolState.walk = true; boolState.idle = false;
		direction = Direction::RIGHT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		if (boolState.run)
			speed = 0.5f; // Vitesse de course

		player.move({ 0.f, -speed });
		boolState.walk = true; boolState.idle = false;
		direction = Direction::UP;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		if (boolState.run)
			speed = 0.5f; // Vitesse de course

		player.move({ 0.f, speed });
		boolState.walk = true; boolState.idle = false;
		direction = Direction::DOWN;
	}

	if (boolState.walk && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
	{
		boolState.run = true;
	}
	else
	{
		speed = 0.3f; boolState.run = false;
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key ::W) &&
		!sf ::Keyboard ::isKeyPressed(sf ::Keyboard ::Key ::Down) && !sf ::Keyboard ::isKeyPressed(sf ::Keyboard ::Key ::S))
	{
		boolState.walk = false; boolState.idle = true;
	}
}

/* Fonction pour empêcher le personnage de sortir de la fenêtre de jeu (Fait par chatGpt) */
void keepInsideWindow(sf::Sprite& player, const sf::RenderWindow& window)
{
	sf::Vector2f pos = player.getPosition();
	sf::Vector2u size = window.getSize();

	float half = 16.f;

	if (pos.x - half < 0) pos.x = half;
	if (pos.x + half > size.x) pos.x = size.x - half;
	if (pos.y - half < 0) pos.y = half;
	if (pos.y + half > size.y) pos.y = size.y - half;

	player.setPosition(pos);
}

/* Fonction pour gérer l'animation de marche du personnage */
void walkAnimation(sf::Clock& clock, float frameTime, sf::IntRect& rectSource, sf::Sprite& player, int col) 
{
	std::cout << "Walking..." << std::endl;
	if (clock.getElapsedTime().asSeconds() >= frameTime) 
	{
		// Changement de colonne pour la texture d'animation de marche
		rectSource.position.y = col * 32;	// Position Y de la partie de la texture à afficher pour la marche

		rectSource.position.x += 32;		// Avancer à la frame suivante de l'animation de marche

		// 4 frames d'animation (4*32 = 128)
		if (rectSource.position.x >= 128) {
			rectSource.position.x = 0;		// Revenir à la première frame
		}

		player.setTextureRect(rectSource);

		clock.restart();
	}
}

/* Fonctions pour gérer l'animation d'inactivité du personnage */
void idleAnimation(sf::Clock& clock, float frameTime, sf::IntRect& rectSource, sf::Sprite& player, int col)
{
	std::cout << "Idle..." << std::endl;
	if (clock.getElapsedTime().asSeconds() >= frameTime)
	{
		// Changement de colonne pour la texture d'animation de marche
		rectSource.position.y = col * 32;	// Position Y de la partie de la texture à afficher pour la marche

		rectSource.position.x += 32;		// Avancer à la frame suivante de l'animation de marche

		// 2 frames d'animation (2*32 = 64)
		if (rectSource.position.x >= (64)) {
			rectSource.position.x = 0;		// Revenir à la première frame
		}

		player.setTextureRect(rectSource);

		clock.restart();
	}
}

/* Fonctions pour gérer l'animation de course du personnage */
void runningAnimation(sf::Clock& clock, float frameTime, sf::IntRect& rectSource, sf::Sprite& player, int col)
{
	std::cout << "Running..." << std::endl;
	if (clock.getElapsedTime().asSeconds() >= frameTime)
	{
		// Changement de colonne pour la texture d'animation de marche
		rectSource.position.y = col * 32;	// Position Y de la partie de la texture à afficher pour la marche

		rectSource.position.x += 32;		// Avancer à la frame suivante de l'animation de marche

		// 8 frames d'animation (8*32 = 256)
		if (rectSource.position.x >= (256)) {
			rectSource.position.x = 0;		// Revenir à la première frame
		}

		player.setTextureRect(rectSource);

		clock.restart();
	}
}

void attackingAnimation() 
{
	// Code pour gérer l'attaque du personnage
}

int main()
{
	//////////////////////////////////////////////////
	// Creation de la fenêtre de jeu
	//////////////////////////////////////////////////
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Game Project Escape Prototype");
	window.setFramerateLimit(60);

    //////////////////////////////////////////////////

	//////////////////////////////////////////////////
	// Création du sprite du joueur
	//////////////////////////////////////////////////

	sf::Texture playerTexture;
	// Chargement de la texture du joueur
    if (!playerTexture.loadFromFile("player.png"))
    {
        std::cout << "Failed to load player texture!" << std::endl;
        return -1;
	}

	// Paramètres pour la partie de la texture à afficher (correspondant au personnage)
	int fL = 32;                            // FrameLargeur : Largeur de la partie de la texture à afficher
	int fH = 32;                            // FrameHauteur : Hauteur de la partie de la texture à afficher
	int columns = 0;                        // Nombre de colonnes dans la texture
	int lignes = 0;                         // Nombre de lignes dans la texture

	// Calcul du nombre de colonnes et de lignes dans la texture
	int x = columns * fL;                   // Position X de la partie de la texture à afficher
	int y = lignes * fH;                    // Position Y de la partie de la texture à afficher

	sf::Sprite player(playerTexture);

	// Sprite du joueur : on affiche la partie de la texture qui correspond au personnage (32x32 pixels)
	sf::IntRect rect({ x,y }, { fL,fH });
    player.setTextureRect(rect);

	player.setOrigin({ 16.f, 16.f });       // Centrer l'origine du sprite
    player.setPosition({ 400.f, 300.f });   // Position initiale du joueur au centre de la fenêtre
	player.setScale({ 1.0f, 1.0f });        // Modifier la taille du sprite du joueur

	////////////////////////////////////////////////

	////////////////////////////////////////////////
	// Variables pour gérer le déplacement du joueur
	// et l'animation du personnage
	///////////////////////////////////////////////

	float speed = 0.3f; // Vitesse de déplacement du joueur

	Direction direction = Direction::RIGHT; // Variable pour stocker la direction actuelle du joueur
	PlayerState state = PlayerState::IDLE;	// Variable pour stocker l'état actuel du joueur
	static PlayerState previousState = PlayerState::IDLE; // Variable pour stocker l'état précédent du joueur (pour gérer le changement d'animation)

	boolPlayerState boolState;				// Variable booléenne pour stocker les différents états du joueur

	//////////////////////////////////////////////

	// Horloge pour gérer l'animation du personnage ou autre chose liée au temps
	sf::Clock clock;

	// Game loop
    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

		keyboardInput(boolState, direction, player, speed);

		keepInsideWindow(player, window);

		if (direction == Direction::RIGHT)
			player.setScale({ 1.f, 1.f });	// Pour que le personnage regarde vers la droite
		else if (direction == Direction::LEFT)
			player.setScale({ -1.f, 1.f }); // Pour que le personnage regarde vers la gauche

		// Mise à jour de l'état du joueur en fonction des booléens
		if (boolState.run)
			state = PlayerState::RUNNING;
		else if (boolState.walk)
			state = PlayerState::WALKING;
		else 
			state = PlayerState::IDLE;

		// Si l'état du joueur a changé, on réinitialise l'animation
		if (state != previousState)
		{
			rect.position.x = 0;
			rect.position.y = (state == PlayerState::IDLE ? 0 :
							   state == PlayerState::WALKING ? 2 * 32 :
							   state == PlayerState::RUNNING ? 3 * 32 : 0);

			player.setTextureRect(rect);

			clock.restart();
			previousState = state;
		}

		switch (state)
		{
		case PlayerState::IDLE:
			idleAnimation(clock, 0.3f, rect, player, 0);
			break;
		case PlayerState::WALKING:
			walkAnimation(clock, 0.3f, rect, player, 2);
			break;
		case PlayerState::RUNNING:
			runningAnimation(clock, 0.1f, rect, player, 3);
			break;
		case PlayerState::ATTACKING:
			break;
		default:
			break;
		}

        window.clear(sf::Color(50,50,50));
        window.draw(player);
        window.display();
    }
}