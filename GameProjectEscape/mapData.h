//====================================
// Fichier : mapData.cpp
// Description : Implémentation de la fonction pour charger un fichier CSV
// Auteur : Aboubacar Sanogo
// Version : 1.0
//====================================

#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

//===============================
// Fonction pour charger une carte de tuiles à partir d'un fichier CSV
//================================
bool loadCSV(const std::string& filename, std::vector<int>& map, int& width, int& height);

//===============================
// Fonction pour copier les données de la carte chargée dans un tableau 1D
//================================
void loadMap(int* map, const std::vector<int>& mapData, const int MAX_TILES);
