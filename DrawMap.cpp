#include "Map.hpp"
#include <cstdlib>
#include <ctime>

Map::Map(const std::string& cloudTexturePath, const std::string& blockTexturePath, const std::string& block2TexturePath, size_t cloudCount) {
    if (!cloudTexture.loadFromFile(cloudTexturePath)) {
        throw std::runtime_error("Failed to load cloud texture");
    }

    if (!blockTexture.loadFromFile(blockTexturePath)) {
        throw std::runtime_error("Failed to load block texture");
    }

    if (!block2Texture.loadFromFile(block2TexturePath)) {
        throw std::runtime_error("Failed to load block texture");
    }


    // Create and position clouds
    srand(time(nullptr));
    for (int i = -1000; i < 5000; i += cloudTexture.getSize().x) {
        sf::Sprite cloudSprite;
        cloudSprite.setTexture(cloudTexture);
        cloudSprite.setPosition(
            static_cast<float>(rand() % 10000),  // random x position
            static_cast<float>(rand() % 150)   // random y position between 0 and 150
        );
        cloudSprites.push_back(cloudSprite);
    }

        // Create and position block2
    for (int i = -1000; i < 1000; i += block2Texture.getSize().x) {
        sf::Sprite block2Sprite;
        block2Sprite.setTexture(block2Texture);
        block2Sprite.setPosition(
            static_cast<float>(rand() % 10000),  // random x position
            static_cast<float>(rand() % 20 + 150)   // random y position between 150 and 170
        );
        luckysprites.push_back(block2Sprite);
    }

    // Create and position floor blocks
    for (int i = -1000; i < 100000; i += blockTexture.getSize().x) {
        sf::Sprite blockSprite;
        blockSprite.setTexture(blockTexture);
        blockSprite.setPosition(static_cast<float>(i), 240 - blockTexture.getSize().y);
        floorBlocks.push_back(blockSprite);
    }
}

void Map::update(float moveVectorX) {

        }
    

void Map::draw(sf::RenderWindow& window) {
    for (std::vector<sf::Sprite>::const_iterator it = cloudSprites.begin(); it != cloudSprites.end(); ++it) {
        window.draw(*it);
    }
    for (std::vector<sf::Sprite>::const_iterator it = luckysprites.begin(); it != luckysprites.end(); ++it) {
        window.draw(*it);
    }
    for (std::vector<sf::Sprite>::const_iterator it = floorBlocks.begin(); it != floorBlocks.end(); ++it) {
        window.draw(*it);
    }
}

bool Map::isOnGround(const sf::Sprite& marioSprite) const {
    sf::FloatRect marioBounds = marioSprite.getGlobalBounds();
    marioBounds.top += 1.0f;  // Check a small distance below Mario's feet

    for (std::vector<sf::Sprite>::const_iterator it = floorBlocks.begin(); it != floorBlocks.end(); ++it) {
        if (marioBounds.intersects(it->getGlobalBounds())) {
            return true;
        }
    }

    for (std::vector<sf::Sprite>::const_iterator it = luckysprites.begin(); it != luckysprites.end(); ++it) {
        if (marioBounds.intersects(it->getGlobalBounds())) {
            return true;
        }
    }

    return false;
}


bool Map::checkCollision(const sf::Sprite& sprite, const sf::Vector2f& moveVector) const {
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    spriteBounds.left += moveVector.x;
    spriteBounds.top += moveVector.y;

    for (std::vector<sf::Sprite>::const_iterator it = floorBlocks.begin(); it != floorBlocks.end(); ++it) {
        if (spriteBounds.intersects(it->getGlobalBounds())) {
            return true;
        }
    }

    for (std::vector<sf::Sprite>::const_iterator it = luckysprites.begin(); it != luckysprites.end(); ++it) {
        if (spriteBounds.intersects(it->getGlobalBounds())) {
            // Remove the lucky block sprite from the vector
            return true;
        }
    }


    return false;
}




