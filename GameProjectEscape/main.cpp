#include <iostream>

#include <SFML/Graphics.hpp>

/* Fonction pour gérer l'animation de marche du personnage */
void walking(bool& walk, sf::Clock& clock, float frameTime, sf::IntRect& rectSource, sf::Sprite& player, int col) 
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
void idle(bool& notMoving, sf::Clock& clock, float frameTime, sf::IntRect& rectSource, sf::Sprite& player, int col)
{
	if (!notMoving)
	{
		return;
	}
	if (notMoving)
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

void jumping() 
{
	// Code pour gérer le saut du personnage
}

void animation() 
{
	// Code pour gérer l'animation du personnage

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

	float speed = 1.f; // Vitesse de déplacement du joueur

	bool walk = false; // Variable pour indiquer si le joueur est en train de marcher ou non
	bool notMoving = true; // Variable pour indiquer si le joueur est en train de rester immobile ou non

	// Horloge pour gérer l'animation du personnage ou autre chose liée au temps
	sf::Clock clock;
	float frameTime = 0.5f;

	// Game loop
    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

		// Déplacement du joueur avec les touches fléchées
		frameTime = 0.2f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			// Pour que le personnage regarde vers la gauche
			player.setScale({ -1.f, 1.f });

			player.move({ -speed, 0.f }); walk = true; notMoving = false;
			walking(walk, clock, frameTime, rect, player, 2);
		}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			// Pour que le personnage regarde vers la droite
			player.setScale({ 1.f, 1.f });

			player.move({ speed, 0.f }); walk = true; notMoving = false;
			walking(walk, clock, frameTime, rect, player, 2);
		}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			player.move({ 0.f, -speed }); walk = true; notMoving = false;
			walking(walk, clock, frameTime, rect, player, 2);
		}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			player.move({ 0.f, speed }); walk = true; notMoving = false;
			walking(walk, clock, frameTime, rect, player, 2);
		}

		frameTime = 0.3f;
		idle(notMoving, clock, frameTime, rect, player, 0);

		walk = false;
		notMoving = true;

        window.clear(sf::Color(50,50,50));
        window.draw(player);
        window.display();
    }
}