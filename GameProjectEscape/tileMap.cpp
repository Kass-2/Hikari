//====================================
// Fichier : tileMap.cpp
// Description : Implémentation de la classe TileMap pour gérer les cartes de tuiles
// Auteur : Google AI
// Version : 1.0
//====================================

#include "tileMap.h"

TileMap::TileMap(unsigned int mapWidth, unsigned int mapHeight)
    : mapWidth(mapWidth), mapHeight(mapHeight)
{

}

bool TileMap::load(const std::string& tilesetPath, sf::Vector2u tileSize, const int* tiles)
{
    unsigned int width = mapWidth;
	unsigned int height = mapHeight;

    if (!m_tileset.loadFromFile(tilesetPath))
        return false;

    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(width * height * 6);

    for (unsigned int i = 0; i < width; ++i)
    {
        for (unsigned int j = 0; j < height; ++j)
        {
            int tileNumber = tiles[i + j * width];

            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

            sf::Vector2f p0(i * tileSize.x, j * tileSize.y);
            sf::Vector2f p1((i + 1) * tileSize.x, j * tileSize.y);
            sf::Vector2f p2((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            sf::Vector2f p3(i * tileSize.x, (j + 1) * tileSize.y);

            sf::Vector2f t0(tu * tileSize.x, tv * tileSize.y);
            sf::Vector2f t1((tu + 1) * tileSize.x, tv * tileSize.y);
            sf::Vector2f t2((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            sf::Vector2f t3(tu * tileSize.x, (tv + 1) * tileSize.y);

            // Triangle 1
            triangles[0].position = p0; triangles[0].texCoords = t0;
            triangles[1].position = p1; triangles[1].texCoords = t1;
            triangles[2].position = p2; triangles[2].texCoords = t2;

            // Triangle 2
            triangles[3].position = p0; triangles[3].texCoords = t0;
            triangles[4].position = p2; triangles[4].texCoords = t2;
            triangles[5].position = p3; triangles[5].texCoords = t3;
        }
    }
    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_tileset;
    target.draw(m_vertices, states);
}