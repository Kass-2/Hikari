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

struct PlayerInfo
{
	std::string name;
	int health;
	int maxHealth;
	int attackPower;
	int defense;
	int speed;
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
	Player(sf::Texture& texture, sf::Sprite& sprite, const std::string& textureFile);
	void update(float dt, const sf::RenderWindow & window, sf::Sprite& sprite);
	void draw(sf::RenderWindow & window, sf::Sprite& sprite);
	void initialState(std::string);

private:
	void handleInput(sf::Sprite& sprite);
	void updateAnimation(float deltaTime, sf::Sprite& sprite);
	
	
	std::string textureFile;

	sf::IntRect rectSource;
	sf::Clock animationClock;

	Direction direction;
	boolPlayerState boolState;
	PlayerState state;
	PlayerState previousState;
	PlayerInfo player;

	float speed;
};