//=========================================================
// Classe Player qui hérite de Entity et qui gère les mouvements, les animations et les états du joueur
// Cette classe est responsable de la mise à jour de la position du joueur en fonction des entrées clavier, 
// de la gestion des animations en fonction de l'état du joueur (idle, walk, run, attack) et de l'affichage 
// du sprite du joueur à l'écran.
// \author Aboubacar Sanogo
// \version 1.0
// \note Cette classe utilise une structure booléenne pour stocker les différents états du joueur, ainsi qu'une énumération pour les différentes directions du joueur.
// \warning Cette classe est un exemple de base pour la gestion d'un personnage joueur dans un jeu 2D, et peut être améliorée et étendue pour inclure des fonctionnalités supplémentaires telles que les collisions, les interactions avec d'autres entités, etc.
// 
// \todo Ajouter la gestion des collisions avec les murs et les autres entités du jeu
// \todo Ajouter la gestion des interactions avec les objets du jeu (ramasser des objets, ouvrir des portes, etc.)
// \todo Ajouter la gestion de la santé du joueur et des dégâts subis
// \todo Ajouter la gestion de l'inventaire du joueur et des objets ramassés
// \todo Ajouter la gestion des compétences et des attaques du joueur
// \todo Ajouter la gestion de l'expérience et du niveau du joueur
// \todo Ajouter la gestion de l'animation du joueur en fonction de l'état et de la direction du joueur
//=========================================================

#pragma once
#include "entity.h"
#include <vector>
#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

// Structure pour stocker les différents états du joueur sous forme de booléens
struct boolPlayerState
{
	bool damaged = false;
	bool dead = false;
	bool walk = false;
	bool idle = false;
	bool run = false;
	bool attack = false;
};

// Structure pour stocker les informations du joueur
struct PlayerInfo
{
	std::string name;
	float health;
	float maxHealth;
	float attackPower;
	float defense;
	float speed;
};

// Enumération pour les différentes directions du joueur
enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// Enumération pour les différents états du joueur
enum class PlayerState
{
	DAMAGED,
	DEAD,
	IDLE,
	WALKING,
	RUNNING,
	ATTACKING,
};

// Enumération pour les différentes entrées clavier
enum class KeyState
{
	PRESSED,
	RELEASED
};

// Enumération pour les différentes touches du clavier
enum class Keys
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SHIFT
};

//=========================================================
// Classe Player qui hérite de Entity et qui gère les mouvements, les animations et les états du joueur
// Cette classe est responsable de la mise à jour de la position du joueur en fonction des entrées clavier,
// de la gestion des animations en fonction de l'état du joueur (idle, walk, run, attack) et de l'affichage
// du sprite du joueur à l'écran.
//==========================================================
class Player {
public:
	//===============================
	// Création du sprite du joueur
	//===============================
	Player(sf::Texture& texture, sf::Sprite& sprite, const std::string& textureFile);
	
	//===============================
	// Mise à jour de la position du joueur en fonction des entrées clavier et de l'état du joueur
	//===============================
	void update(float dt, const sf::RenderWindow & window, sf::Sprite& sprite);
	
	//===============================
	// Initialisation des informations du joueur (nom, santé, puissance d'attaque, défense, vitesse, etc.)
	//===============================
	void initialState(std::string);

	//===============================
	// Gestion des événements clavier pour mettre à jour les booléens correspondants aux touches pressées ou relâchées
	//===============================
	void handleEvent(const sf::Event& event);

	//===============================
	// Affichage du sprite du joueur à l'écran
	//===============================
	void draw(sf::RenderWindow& window, sf::Sprite& sprite);

private:
	//===============================
	// Gestion des entrées clavier pour déplacer le joueur et changer son état
	//===============================
	void handleInput(sf::Sprite& sprite);

	//===============================
	// Mise à jour de l'animation du joueur en fonction de son état et de sa direction
	//===============================
	void updateAnimation(float deltaTime, sf::Sprite& sprite);
	
	// Texture et sprite du joueur
	std::string textureFile;

	// Rectangle source pour l'animation du joueur
	sf::IntRect rectSource;
	// Horloge pour gérer le temps entre les frames d'animation
	sf::Clock animationClock;

	// Direction actuelle du joueur
	Direction direction;
	// Structure pour stocker les différents états du joueur sous forme de booléens
	boolPlayerState boolState;
	// État actuel du joueur
	PlayerState state;
	// État précédent du joueur pour gérer les transitions d'animation
	PlayerState previousState;
	// Informations sur le joueur
	PlayerInfo player;

	// Booléens pour gérer le déplacement à gauche
	bool leftHeld = false;
	// Booléens pour gérer le déplacement à droite
	bool rightHeld = false;
	// Booléens pour gérer le déplacement en haut
	bool upHeld = false;
	// Booléens pour gérer le déplacement en bas
	bool downHeld = false;

	// Booléens pour gérer l'inventaire du joueur
	bool inventory = false;
	// Booléens pour gérer l'interaction avec les objets du jeu
	bool interact = false;

	// Booléen pour gérer l'attaque du joueur
	bool attackPressed = false;
	// Booléen pour savoir si le joueur est en train d'attaquer
	bool attacking = false;

	// Mémoriser la dernière direction horizontale
	Direction lastHorizontal = Direction::RIGHT;
	// Mémoriser la dernière direction verticale
	Direction lastVertical = Direction::DOWN;

	// Vitesse de déplacement du joueur
	PlayerInfo speed;
};