// map.hpp
#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Map {
public:
    Map(const std::string& cloudTexturePath, const std::string& blockTexturePath, size_t cloudCount = 20);

    void update(float moveVectorX);
    void draw(sf::RenderWindow& window);

private:
    sf::Texture cloudTexture;
    std::vector<sf::Sprite> cloudSprites;
    sf::Texture blockTexture;
    std::vector<sf::Sprite> floorBlocks;
};

#endif // MAP_HPP
