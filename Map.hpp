#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Mushroom;  // forward declaration

class Map {
public:
    Map(const std::string& cloudTexturePath, const std::string& blockTexturePath, const std::string& block2TexturePath, size_t cloudCount);

    void update(float moveVectorX);
    void draw(sf::RenderWindow& window);
    bool isOnGround(const sf::Sprite& marioSprite) const;
    bool checkCollision(const sf::Sprite& sprite, const sf::Vector2f& moveVector) const;

private:
    sf::Texture cloudTexture;
    sf::Texture block2Texture;
    sf::Texture blockTexture;
    std::vector<sf::Sprite> cloudSprites;
    std::vector<sf::Sprite> luckysprites;
    std::vector<sf::Sprite> floorBlocks;

    friend class Mushroom;  // allow access to private members
};

#endif // MAP_HPP
