#include "Map.hpp"
#include <cstdlib>
#include <ctime>

Map::Map(const std::string& cloudTexturePath, const std::string& blockTexturePath, size_t cloudCount) {
    if (!cloudTexture.loadFromFile(cloudTexturePath)) {
        throw std::runtime_error("Failed to load cloud texture");
    }

    if (!blockTexture.loadFromFile(blockTexturePath)) {
        throw std::runtime_error("Failed to load block texture");
    }

    // Create and position clouds
    srand(time(nullptr));
    for (size_t i = 0; i < cloudCount; ++i) {
        sf::Sprite cloudSprite;
        cloudSprite.setTexture(cloudTexture);
        cloudSprite.setPosition(
            static_cast<float>(rand() % 512),  // random x position
            static_cast<float>(rand() % 150)   // random y position between 0 and 150
        );
        cloudSprites.push_back(cloudSprite);
    }

    // Create and position floor blocks
    for (int i = 0; i < 256; i += blockTexture.getSize().x) {
        sf::Sprite blockSprite;
        blockSprite.setTexture(blockTexture);
        blockSprite.setPosition(static_cast<float>(i), 240 - blockTexture.getSize().y);
        floorBlocks.push_back(blockSprite);
    }
}

void Map::update(float moveVectorX) {
    for (auto& cloud : cloudSprites) {
        cloud.move(moveVectorX, 0);
        if (cloud.getPosition().x + cloudTexture.getSize().x < 0) {
            cloud.setPosition(256, static_cast<float>(rand() % 150));
        }
    }
}

void Map::draw(sf::RenderWindow& window) {
    for (const auto& cloud : cloudSprites) {
        window.draw(cloud);
    }
    for (const auto& block : floorBlocks) {
        window.draw(block);
    }
}


