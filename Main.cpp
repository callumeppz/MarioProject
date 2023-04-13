#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.hpp"

class Mario {
public:
    Mario(const std::string& texturePath) {
        if (!marioTexture.loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load Mario texture");
        }
        marioSprite.setTexture(marioTexture);
        marioSprite.setPosition(128 - marioTexture.getSize().x / 2, 240 - marioTexture.getSize().y);

        isJumping = false;
        isMovingLeft = false;
    }

    public:
    float getMoveVectorX() const {
        const float moveSpeed = 50.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            return -moveSpeed;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            return moveSpeed;
        }
        return 0.0f;
    }

    void update(sf::RenderWindow& window, float deltaTime) {
        const float moveSpeed = 50.0f;
        const float jumpForce = 250.0f;
        const float gravity = 500.0f;
        sf::Vector2f moveVector(0, 20);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            moveVector.x = -moveSpeed * deltaTime;
            isMovingLeft = true;
        } else {
            isMovingLeft = false;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            moveVector.x = moveSpeed * deltaTime;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isJumping) {
            isJumping = true;
            velocity.y = -jumpForce;
        }

        if (isJumping) {
            velocity.y += gravity * deltaTime;
            moveVector.y = velocity.y * deltaTime;
        }

        if (checkBounds(moveVector)) {
            marioSprite.move(0, moveVector.y);
            if (moveVector.x > 0) {
                backgroundSprite.move(-moveVector.x * 0.5f, 0);
            } else {
                backgroundSprite.move(-moveVector.x, 0);
            }
        } else {
            isJumping = false;
            velocity.y = 0.0f;
        }

        // Flip the sprite horizontally if moving left
        if (isMovingLeft) {
            marioSprite.setScale(-1.f, 1.f);
            marioSprite.setOrigin(marioTexture.getSize().x, 20);
        } else {
            marioSprite.setScale(1.f, 1.f);
            marioSprite.setOrigin(0, 20);
        }

        // Place the sprite on the "floor"
        if (marioSprite.getPosition().y + marioTexture.getSize().y >= 240) {
            marioSprite.setPosition(marioSprite.getPosition().x, 240 - marioTexture.getSize().y);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(backgroundSprite); // Draw the background first
        window.draw(marioSprite);
    }

private:
    bool checkBounds(const sf::Vector2f& moveVector) {
        sf::Vector2f newPosition = marioSprite.getPosition() + moveVector;
        sf::Vector2u textureSize = marioTexture.getSize();
        const unsigned int canvasHeight = 240;

        return newPosition.y + textureSize.y <= canvasHeight;
    }

    sf::Texture marioTexture;
    sf::Sprite marioSprite;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Vector2f velocity;
    bool isJumping;
    bool isMovingLeft;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(256, 240), "Mario Personal Project");
    sf::Clock clock;
    try {
        Mario mario("/Users/callumapps/Desktop/SuperMarioBros_Project 2/MarioIdle.png");
        Map map("/Users/callumapps/Desktop/SuperMarioBros_Project 2/cloud.png", "/Users/callumapps/Desktop/SuperMarioBros_Project 2/block.png", 5);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
            float deltaTime = clock.restart().asSeconds();
            window.clear(sf::Color(135, 206, 235));
            mario.update(window, deltaTime);
            map.update(mario.getMoveVectorX());
            map.draw(window);
            mario.draw(window);
            window.display();
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
