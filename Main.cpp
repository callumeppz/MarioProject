#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.hpp"
#include "enemy.cpp"
#include "mushroom.cpp"


// Add the AnimatedSprite class definition here
class AnimatedSprite {
public:
    AnimatedSprite(const std::string& texturePath, int frameWidth, int frameHeight, int numFrames)
        : m_frameWidth(frameWidth),
          m_frameHeight(frameHeight),
          m_numFrames(numFrames),
          m_currentFrame(0) {
        if (!m_texture.loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load texture");
        }

        m_sprite.setTexture(m_texture);
        m_sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    }

    void setFrame(int frame) {
        if (frame >= 0 && frame < m_numFrames) {
            m_currentFrame = frame;
            m_sprite.setTextureRect(sf::IntRect(m_frameWidth * m_currentFrame, 0, m_frameWidth, m_frameHeight));
        }
    }

    int getCurrentFrame() const {
        return m_currentFrame;
    }

    int getFrameWidth() const {
        return m_frameWidth;
    }

    int getFrameHeight() const {
        return m_frameHeight;
    }

    const sf::Sprite& getSprite() const {
        return m_sprite;
    }

    sf::Sprite& getSprite() {
        return m_sprite;
    }

private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    int m_frameWidth;
    int m_frameHeight;
    int m_numFrames;
    int m_currentFrame;
};

class Mario {
float jumpTimer;
float MoveTimer;
float MoveTimerY;
float jumpcount;
public:
    Mario(const std::string& texturePath) {
        if (!marioTexture.loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load Mario texture");
        }
        marioSprite.setTexture(marioTexture);
        marioSprite.setPosition(128 - marioTexture.getSize().x / 2, 240 - marioTexture.getSize().y);
        isJumping = false;
        isMovingLeft = false;
        MoveTimer = 0.0f;
        MoveTimerY = 0.0f;
        jumpTimer = 0.0f;
        jumpcount = 0;
        velocity = sf::Vector2f(0, 0);
    }

        void changeTexture(const std::string& texturePath) {
        if (!marioTexture.loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load Mario texture");
        }
        marioSprite.setTexture(marioTexture);
    }

    void loadBackground(const std::string& backgroundPath) {
        if (!backgroundTexture.loadFromFile(backgroundPath)) {
            throw std::runtime_error("Failed to load background texture");
        }
        backgroundSprite.setTexture(backgroundTexture);
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

void update(sf::RenderWindow& window, float deltaTime, sf::View& view, const Map& map) {
    float moveSpeed = 50.0f;
    const float maxMoveTimer = 5.0f;  // Maximum time the jump button can be held
    float jumpForce = 250.0f;
    const float gravity = 500.0f;
    float maxJumpTime = 0.25f;  // Maximum time the jump button can be held
    sf::Vector2f moveVector(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        changeTexture("/Users/callumapps/Desktop/SuperMarioBros_Project 2/runmario.png");
        MoveTimerY += deltaTime;
        moveVector.x = -moveSpeed * deltaTime;
        isMovingLeft = true;
        if (MoveTimerY > 0.0f && MoveTimerY < 2.0f) {
            changeTexture("/Users/callumapps/Desktop/SuperMarioBros_Project 2/runmario.png");
            isMovingLeft = true;
            moveSpeed = 50.0f;
            moveVector.x = -moveSpeed * deltaTime;
        } else if (MoveTimerY >= 2.0f && MoveTimerY < 5.0f) {
            isMovingLeft = true;
            moveSpeed = 100.0f;
            moveVector.x = -moveSpeed * deltaTime;
        } else if (MoveTimerY >= 5.0f) {
            isMovingLeft = true;
            moveSpeed = 150.0f;
            moveVector.x = -moveSpeed * deltaTime;
        }
    } else {
        changeTexture("/Users/callumapps/Desktop/SuperMarioBros_Project 2/MarioIdle.png");
        MoveTimerY = 0.0f;
        isMovingLeft = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        changeTexture("/Users/callumapps/Desktop/SuperMarioBros_Project 2/runmario.png");
        MoveTimer += deltaTime;
        moveVector.x = moveSpeed * deltaTime;
        if (MoveTimer > 2.0f) {
            moveSpeed = 100.0f;
            moveVector.x = moveSpeed * deltaTime;
        } else if (MoveTimer > 5.0f) {
            moveSpeed = 150.0f;
            moveVector.x = moveSpeed * deltaTime;
        } else {
            moveSpeed = 50.0f;
            moveVector.x = moveSpeed * deltaTime;
        }
    } else {
        changeTexture("/Users/callumapps/Desktop/SuperMarioBros_Project 2/MarioIdle.png");
        MoveTimer = 0.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && map.isOnGround(marioSprite)) {
        isJumping = true;
        jumpTimer = 0.0f;
        velocity.y = -jumpForce;
    }

    if (isJumping && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && jumpTimer < maxJumpTime) {
        changeTexture("/Users/callumapps/Desktop/SuperMarioBros_Project 2/MarioJump.png");
        jumpTimer += deltaTime;
        jumpcount = 1;
        velocity.y += -jumpForce * deltaTime;
        if (jumpcount == 1){
            jumpForce = 100000.0f;
            maxJumpTime = 1.0f;
            jumpcount = 0;
        }
    } else {
        isJumping = false;
        jumpTimer = maxJumpTime;
    }

    // Check horizontal movement
    if (!map.checkCollision(marioSprite, sf::Vector2f(moveVector.x, 0))) {
        marioSprite.move(moveVector.x, 0);
    }

    // Update vertical movement
    velocity.y += gravity * deltaTime;
    moveVector.y = velocity.y * deltaTime;
    
    // Check vertical movement
    if (!map.checkCollision(marioSprite, sf::Vector2f(0, moveVector.y))) {
        marioSprite.move(0, moveVector.y);
    } else {
        velocity.y = 0.0f;
        if (map.isOnGround(marioSprite)) {
            isJumping = false;
        }
    }

    // Update the view center
    view.setCenter(marioSprite.getPosition().x + marioTexture.getSize().x / 2, view.getCenter().y);
    window.setView(view);

    // Move the background in the opposite direction of Mario's movement
    backgroundSprite.move(-moveVector.x, 0);

    // Flip the sprite horizontally if moving left
    if (isMovingLeft) {
        marioSprite.setScale(-1.f, 1.f);
        marioSprite.setOrigin(marioTexture.getSize().x, 20);
    } else {
        marioSprite.setScale(1.f, 1.f);
        marioSprite.setOrigin(0, 20);
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
    sf::RenderWindow window(sf::VideoMode(1080, 960), "Mario Personal Project");
    sf::Clock clock;
    sf::View view(sf::FloatRect(0, 0, 256, 240));

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return -1;
    }

    sf::Text timerText("", font, 9);
    timerText.setFillColor(sf::Color::Black); // set the text color to black
    timerText.setPosition(view.getCenter().x - view.getSize().x / 2 + 10, view.getCenter().y - view.getSize().y / 2 + 10);

    sf::Text MarioLifeText("", font, 9);
    MarioLifeText.setFillColor(sf::Color::Black); // set the text color to black
    MarioLifeText.setPosition(view.getCenter().x + view.getSize().x / 2 - 50, view.getCenter().y - view.getSize().y / 2 + 10);

    try {
        Mario mario("/Users/callumapps/Desktop/SuperMarioBros_Project 2/MarioIdle.png");
        Map map("/Users/callumapps/Desktop/SuperMarioBros_Project 2/cloud.png", "/Users/callumapps/Desktop/SuperMarioBros_Project 2/block.png", "/Users/callumapps/Desktop/SuperMarioBros_Project 2/block2.png", 5);
        Mushroom mushroom("/Users/callumapps/Desktop/SuperMarioBros_Project 2/Mushroom.png", map);
       
    int numEnemies = 10; // Change this value to spawn exactly 10 enemies
    std::vector<Enemy> enemies;
    for (int i = 0; i < numEnemies; ++i) {
        enemies.emplace_back("/Users/callumapps/Desktop/SuperMarioBros_Project 2/goomba.png", map);
        enemies.back().setPosition(1 + rand() % 100, 30);

    }
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
            float deltaTime = clock.restart().asSeconds();
            int mariolife = 3;
            static int seconds = 0; // make seconds a static variable to preserve its value between function calls
            seconds += static_cast<int>(deltaTime); // add the elapsed time to seconds
            timerText.setString("Time: " + std::to_string(seconds)); // concatenate the time value to the timer text
            MarioLifeText.setString("Life: " + std::to_string(mariolife)); // concatenate the time value to the timer text

            window.clear(sf::Color(135, 206, 235));
            mario.update(window, deltaTime, view, map);

            for (Enemy& enemy : enemies) {
                enemy.update(window, deltaTime, view, map); // Update the enemy
            }

            mushroom.update(window, deltaTime, view, map); // Update the enemy
            mushroom.draw(window); // Draw the enemy
            map.update(mario.getMoveVectorX());
            map.draw(window);
            mario.draw(window);

            for (Enemy& enemy : enemies) {
                enemy.draw(window); // Draw the enemy
            }

            // Move timer text along with the view
            timerText.setPosition(view.getCenter().x - view.getSize().x / 2 + 10, view.getCenter().y - view.getSize().y / 2 + 10);
            MarioLifeText.setPosition(view.getCenter().x - view.getSize().x / 2 + 60, view.getCenter().y - view.getSize().y / 2 + 10);

            window.draw(timerText);
            window.draw(MarioLifeText);

            window.display();
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}

