//===============================
// Fichier : Camera.cpp
// Description : Implémentation de la classe Camera pour gérer les différentes caméras du jeu
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

//===============================
// Inclusion des bibliothèques nécessaires
//===============================
#include "Camera.h"
#include <algorithm>

Camera::Camera(int width, int height, 
	int map_width, int map_height, 
	int tile_size, float zoom) {
	// Créer une vue avec les dimensions de la fenêtre et appliquer un zoom
	sf::View c(sf::FloatRect({ 0.f, 0.f }, { (float)width, (float)height }));
	camera = c;
	camera.zoom(zoom);

	// Initialiser les dimensions de la carte et la taille des tuiles
	MAP_WIDTH = map_width;
	MAP_HEIGHT = map_height;
	TILE_SIZE = tile_size;
}

void Camera::clamped(const Player& player, const sf::Sprite& sprite) {
	
	sf::Vector2f playerPos = player.getPosition(sprite);

	// Taille de la caméra divisée par 2
	float halfWidth = camera.getSize().x / 2.f;
	float halfHeight = camera.getSize().y / 2.f;

	// Limites de la carte en pixels
	float minX = halfWidth;
	float maxX = (MAP_WIDTH * TILE_SIZE) - halfWidth;
	float minY = halfHeight;
	float maxY = (MAP_HEIGHT * TILE_SIZE) - halfHeight;

	// Clamping de la position de la caméra pour qu'elle reste dans la carte
	float clampedX = std::clamp(playerPos.x, minX, maxX);
	float clampedY = std::clamp(playerPos.y, minY, maxY);

	camera.setCenter({ clampedX, clampedY });
}

void Camera::follow(const Player& player, const sf::Sprite& sprite) {
	camera.setCenter(player.getPosition(sprite));
}

void Camera::apply(sf::RenderWindow& window) {
	window.setView(camera);
}

void applyCameraUI(sf::RenderWindow& window) {
	// Réinitialiser la vue pour les éléments de l'interface utilisateur (UI)
	window.setView(window.getDefaultView());

	// TODO : Présentement, elle applique le UI, mais il faudra que je la reconfigure.
	// Je pourrais même créer une classe pour le UI.
}
