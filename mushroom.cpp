#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Map.hpp"

class Mushroom {
    float moveSpeed;
    const float moveSpeedMagnitude = 50.0f;
public:
    Mushroom(const std::string& texturePath, const Map& map) {
        if (!mushroomTexture.loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load mushroom texture");
        }
        mushroomSprite.setTexture(mushroomTexture);
        srand(time(0)); // Seed the random number generator

        // Set a random initial position above ground level
        mushroomSprite.setPosition(0, 30);
        velocity = sf::Vector2f(0, 0);

        // Set initial move speed and direction
        moveSpeed = moveSpeedMagnitude;
    }

    void setPosition(float x, float y) {
        mushroomSprite.setPosition(x, y);
    }

    void update(sf::RenderWindow& window, float deltaTime, sf::View& view, const Map& map) {
        const float gravity = 500.0f;
        sf::Vector2f moveVector(0, 0);

        moveVector.x = moveSpeed * deltaTime;

        // Update vertical movement
        velocity.y += gravity * deltaTime;
        moveVector.y = velocity.y * deltaTime;

        // Check horizontal movement and reverse direction if collision occurs
        if (!map.checkCollision(mushroomSprite, sf::Vector2f(moveVector.x, 0))) {
            mushroomSprite.move(moveVector.x, 0);
        } else {
            moveSpeed = -moveSpeed;
        }

        // Check vertical movement
        if (!map.checkCollision(mushroomSprite, sf::Vector2f(0, moveVector.y))) {
            mushroomSprite.move(0, moveVector.y);
        } else {
            velocity.y = 0.0f;
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(mushroomSprite);
    }

private:
    sf::Texture mushroomTexture;
    sf::Sprite mushroomSprite; // Declare mushroomSprite here
    sf::Vector2f velocity;
};
