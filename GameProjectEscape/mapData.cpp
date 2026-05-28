//====================================
// Fichier : mapData.cpp
// Description : Implémentation de la fonction pour charger un fichier CSV
// Auteur : Aboubacar Sanogo
// Version : 1.0
//====================================

#include "mapData.h"

bool loadCSV(const std::string& filename, std::vector<int>& map, int& width, int& height)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    std::string line;
    width = 0;
    height = 0;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string value;

        int currentWidth = 0;

        while (std::getline(ss, value, ','))
        {
            if (!value.empty())
            {
                map.push_back(std::stoi(value));
                currentWidth++;
            }
        }

        if (width == 0)
            width = currentWidth;

        height++;
    }

    return true;
}

void loadMap(int* map, const std::vector<int>& mapData, const int MAX_TILES)
{
    for (int i = 0; i < MAX_TILES; ++i) {
        map[i] = mapData[i];
    }
}