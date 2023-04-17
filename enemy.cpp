#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Map.hpp"

class Enemy {
    float moveSpeed;
    const float moveSpeedMagnitude = 50.0f;
public:
    Enemy(const std::string& texturePath, const Map& map) {
        if (!enemyTexture.loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load enemy texture");
        }
        enemySprite.setTexture(enemyTexture);
        srand(time(0)); // Seed the random number generator

        // Set a random initial position above ground level
        enemySprite.setPosition(0, 30);
        velocity = sf::Vector2f(0, 0);

        // Set initial move speed and direction
        moveSpeed = moveSpeedMagnitude;
    }

    void loadBackground(const std::string& backgroundPath) {
        if (!backgroundTexture.loadFromFile(backgroundPath)) {
            throw std::runtime_error("Failed to load background texture");
        }
        backgroundSprite.setTexture(backgroundTexture);
    }

    void setPosition(float x, float y) {
    enemySprite.setPosition(x, y);
}

public:
    float getRandomMoveVectorX() {
        const float moveSpeed = 50.0f;
        int direction = rand() % 3 - 1; // Generate a random number between -1, 0, and 1
        return direction * moveSpeed;
    }

  void update(sf::RenderWindow& window, float deltaTime, sf::View& view, const Map& map) {
        const float gravity = 500.0f;
        sf::Vector2f moveVector(0, 0);

        moveVector.x = moveSpeed * deltaTime;

        // Update vertical movement
        velocity.y += gravity * deltaTime;
        moveVector.y = velocity.y * deltaTime;

        // Check horizontal movement and reverse direction if collision occurs
        if (!map.checkCollision(enemySprite, sf::Vector2f(moveVector.x, 0))) {
            enemySprite.move(moveVector.x, 0);
        } else {
            moveSpeed = -moveSpeed;
        }

        // Check vertical movement
        if (!map.checkCollision(enemySprite, sf::Vector2f(0, moveVector.y))) {
            enemySprite.move(0, moveVector.y);
        } else {
            velocity.y = 0.0f;
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(backgroundSprite); // Draw the background first
        window.draw(enemySprite);
    }

private:
    sf::Texture enemyTexture;
    sf::Sprite enemySprite;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Vector2f velocity;
};
