//===============================
// Fichier : Animations.cpp
// Description : Implémentation de la classe Animations pour gérer les animations du joueur
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

// TODO : Ajouter la gestion des animations pour les ennemis et les objets du jeu

//===============================
// Inclusion des bibliothèques
//===============================
#include "Animations.h"

Animations::Animations(int frameHeight, int frameWidth) : fH(frameHeight), fL(frameWidth) {

}

void Animations::resetAnimation(sf::IntRect& rectSource, int row) const {
	rectSource.position.x = 0;
	rectSource.position.y = row * fH;
}