# SFML Code Snippets

This reference provides common C++ / SFML patterns used in this project.

## Entity Structure
Recommended base class for game entities.

```cpp
class Entity {
public:
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    void setPosition(sf::Vector2f pos) { sprite.setPosition(pos); }

protected:
    sf::Sprite sprite;
    sf::Texture texture;
};
```

## Collision Detection
Standard AABB collision logic for tilemaps.

```cpp
bool checkCollision(const sf::FloatRect& a, const sf::FloatRect& b) {
    return a.intersects(b);
}

// In the update loop:
void moveWithCollision(sf::Sprite& sprite, sf::Vector2f velocity, const std::vector<sf::RectangleShape>& walls) {
    // X Movement
    sprite.move(velocity.x, 0.f);
    for (auto& wall : walls) {
        if (sprite.getGlobalBounds().intersects(wall.getGlobalBounds())) {
            sprite.move(-velocity.x, 0.f);
            break;
        }
    }

    // Y Movement
    sprite.move(0.f, velocity.y);
    for (auto& wall : walls) {
        if (sprite.getGlobalBounds().intersects(wall.getGlobalBounds())) {
            sprite.move(0.f, -velocity.y);
            break;
        }
    }
}
```

## Animation Loop
Frame-rate independent animation logic.

```cpp
struct Animation {
    int frameCount;
    float frameTime;
    float elapsedTime;
    int currentFrame;
    int row;

    void update(float dt, sf::IntRect& rect, int tileSize) {
        elapsedTime += dt;
        if (elapsedTime >= frameTime) {
            elapsedTime = 0;
            currentFrame = (currentFrame + 1) % frameCount;
            rect.left = currentFrame * tileSize;
            rect.top = row * tileSize;
        }
    }
};
```

## Asset Loading
Safe texture loading pattern.

```cpp
sf::Texture& getTexture(const std::string& path) {
    static std::map<std::string, sf::Texture> textures;
    if (textures.find(path) == textures.end()) {
        if (!textures[path].loadFromFile(path)) {
            // Handle error
        }
    }
    return textures[path];
}
```
