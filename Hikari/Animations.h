//===============================
// Fichier : Animations.h
// Description : Déclaration de la classe Animations pour gérer les animations
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include "GameHeaders.h"
#include "PlayerData.h"

//=========================================================
// Classe Animations pour gérer les animations du joueur.
// Cette classe est responsable de la mise à jour de l'animation du joueur en fonction de son état et de sa direction,
// ainsi que de la gestion des frames d'animation et du temps entre les frames.
//=========================================================
class Animations {
public:
	    Animations(int frameHeight, int frameWidth);

		//===============================
		// Fonction pour réinitialiser l'animation
		//===============================
		void resetAnimation(sf::IntRect& rectSource, int row) const;

private:
	// FrameHauteur : Hauteur de la partie de la texture à afficher
	int fH;
	// FrameLargeur : Largeur de la partie de la texture à afficher
	int fL;
};
