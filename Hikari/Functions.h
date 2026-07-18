//===============================
// Fichier : Functions.h
// Description : Déclaration de la classe MiniMap pour gérer la mini-carte
// Auteur : Aboubacar Sanogo
// Version : 1.0
//===============================

#pragma once

//===============================
// Inclusion des bibliothèques
//===============================
#include <SFML/Graphics.hpp>

//==============================
// Fonction pour calculer une taille en fonction de la taille de la fenêtre
// Paramètres:
// - baseFontSize : la taille de la police d'origine
// - baseWindowWidth : la largeur de la fenêtre d'origine
// - baseWindowHeight : la hauteur de la fenêtre d'origine
// - currentWindowWidth : la largeur de la fenêtre actuelle
// - currentWindowHeight : la hauteur de la fenêtre actuelle
// Description:
// La taille est basé sur une ancienne taille de fenêtre et une taille précédente
//==============================
static int calculateSize(int baseSize, int baseWindowWidth, 
	int baseWindowHeight, int currentWindowWidth, int currentWindowHeight);

//==============================
// Fonction pour calculer une taille X en fonction de la taille de la fenêtre
// Paramètres:
// - baseFontSize : la taille de la police d'origine
// - baseWindowWidth : la largeur de la fenêtre d'origine
// - currentWindowWidth : la largeur de la fenêtre actuelle
// Description:
// La taille est basé sur une ancienne taille X de fenêtre et une taille X précédente
//==============================
static float calculateSizeX(float baseSize, int baseWindowWidth, int currentWindowWidth);

//==============================
// Fonction pour calculer une taille Y en fonction de la taille de la fenêtre
// Paramètres:
// - baseFontSize : la taille de la police d'origine
// - baseWindowHeight : la hauteur de la fenêtre d'origine
// - currentWindowHeight : la hauteur de la fenêtre actuelle
// Description:
// La taille est basé sur une ancienne taille Y de fenêtre et une taille Y précédente
//==============================
static float calculateSizeY(float baseSize, int baseWindowHeight, int currentWindowHeight);

//==============================
// Fonction pour calculer une position X en fonction de la taille de la fenêtre
// Description:
// La position est basé sur une ancienne position X de l'élément pour une ancienne taille de fenêtre
//==============================
static float calculatePositionX(float positionX, int baseWindowWidth, int currentWindowWidth);

//==============================
// Fonction pour calculer une psotion X en fonction de la taille de la fenêtre
// Description:
// La position est basé sur une ancienne position Y de l'élément pour une ancienne taille de fenêtre
//==============================
static float calculatePositionY(float positionY, int baseWindowHeight, int currentWindowHeight);