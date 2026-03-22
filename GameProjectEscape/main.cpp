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

struct boolPlayerState
{
	bool walk = false;
	bool idle = false;
	bool run = false;
	bool attack = false;
};

/* Fonction pour gérer l'animation de marche du personnage */
void walkAnimation(bool& walk, sf::Clock& clock, float frameTime, sf::IntRect& rectSource, sf::Sprite& player, int col) 
{
	if (!walk)
	{
		return;
	}
	if (walk) 
	{
		std::cout << "Walking..." << std::endl;
		if (clock.getElapsedTime().asSeconds() >= frameTime) 
		{
			// Changement de colonne pour la texture d'animation de marche
			rectSource.position.y = col * 32; // Position Y de la partie de la texture à afficher pour la marche

			rectSource.position.x += 32;

			// 4 frames d'animation (4*32 = 128)
			if (rectSource.position.x >= 128) {
				rectSource.position.x = 0; // Revenir à la première frame
			}

			player.setTextureRect(rectSource);

			clock.restart();
		}
	}
}

/* Fonctions pour gérer l'animation d'inactivité du personnage */
void idleAnimation(bool& idle, sf::Clock& clock, float frameTime, sf::IntRect& rectSource, sf::Sprite& player, int col)
{
	if (!idle)
	{
		return;
	}
	if (idle)
	{
		std::cout << "Idle..." << std::endl;
		if (clock.getElapsedTime().asSeconds() >= frameTime)
		{
			// Changement de colonne pour la texture d'animation de marche
			rectSource.position.y = col * 32; // Position Y de la partie de la texture à afficher pour la marche

			rectSource.position.x += 32;

			// 4 frames d'animation (2*32 = 64)
			if (rectSource.position.x >= (64)) {
				rectSource.position.x = 0; // Revenir à la première frame
			}

			player.setTextureRect(rectSource);

			clock.restart();
		}
	}
}

void running()
{
	// Code pour gérer la course du personnage
}

void attacking() 
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

	float speed = 0.5f; // Vitesse de déplacement du joueur

	Direction direction = Direction::RIGHT; // Variable pour stocker la direction actuelle du joueur
	PlayerState state = PlayerState::IDLE;	// Variable pour stocker l'état actuel du joueur

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

		// Déplacement du joueur avec les touches fléchées
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			player.move({ -speed, 0.f }); boolState.walk = true; boolState.idle = false;
			direction = Direction::LEFT;
		}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			player.move({ speed, 0.f }); boolState.walk = true; boolState.idle = false;
			direction = Direction::RIGHT;
		}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			player.move({ 0.f, -speed }); boolState.walk = true; boolState.idle = false;
		}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			player.move({ 0.f, speed }); boolState.walk = true; boolState.idle = false;
		}

		if (direction == Direction::RIGHT)
			player.setScale({ 1.f, 1.f }); // Pour que le personnage regarde vers la droite
		else
			player.setScale({ -1.f, 1.f }); // Pour que le personnage regarde vers la gauche

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			boolState.walk = false; boolState.idle = true;
		}

		if (boolState.walk) state = PlayerState::WALKING;
		else state = PlayerState::IDLE;

		switch (state)
		{
		case PlayerState::IDLE:
			idleAnimation(boolState.idle, clock, 0.3f, rect, player, 0);
			break;
		case PlayerState::WALKING:
			walkAnimation(boolState.walk, clock, 0.3f, rect, player, 2);
			break;
		case PlayerState::RUNNING:
			
			break;
		case PlayerState::ATTACKING:
			break;
		default:
			break;
		}

		/*walk = false;
		boolState.idle = true;*/

        window.clear(sf::Color(50,50,50));
        window.draw(player);
        window.display();
    }
}