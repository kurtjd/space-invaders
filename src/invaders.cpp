#include <iostream>
#include <sstream>
#include <cmath>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "PlayerShip.hpp"
#include "Shield.hpp"
#include "PlayerBullet.hpp"

/* Display Constants */
const std::string SCREEN_TITLE = "Space Invaders!";
const unsigned SCREEN_WIDTH = 800;
const unsigned SCREEN_HEIGHT = 730;
const unsigned FRAME_RATE = 60;
const sf::Color BG_COLOR = sf::Color::Black;

// Calculates the current FPS and updates the window title with it.
void updateFPS(sf::Window &window, sf::Clock &fps_clock, float &fps_timer)
{
    float fps = 1.0f / (fps_clock.getElapsedTime().asSeconds() - fps_timer);
    std::ostringstream title;
    title << "Space Invaders! - " << round(fps) << "fps";
    window.setTitle(title.str());
    fps_timer = fps_clock.getElapsedTime().asSeconds();
}

// Creates an Image object to be used as a texture for sprites.
sf::Image load_sprites(std::string img)
{
    sf::Image spritesheet;
    spritesheet.loadFromFile(img);
    return spritesheet;
}

//Creates shields across the screen
void init_shields(sf::Image &spritesheet, std::vector<Shield*> &shields)
{
    // Create shields on the heap so they don't go out of scope.
    // This loop creates 4 shields across the screen.
    // 97 is start point, -5 is to stop part of 5th shield from drawing at edge
    for (unsigned i = 97; i < SCREEN_WIDTH - 5; i += 175)
        shields.push_back(new Shield(spritesheet, i));
}

//Remove shields from memory
void del_shields(std::vector<Shield*> &shields)
{
    for (unsigned i = 0; i < shields.size(); ++i)
        delete shields[i];

    shields.clear();
}

// Draws the shields to the screen
void draw_shields(sf::RenderWindow &window, std::vector<Shield*> &shields)
{
    for (unsigned i = 0; i < shields.size(); ++i)
        window.draw(shields[i]->getSprite());
}

int main()
{
    sf::Clock fps_clock;
    float fps_timer = fps_clock.getElapsedTime().asSeconds();

    // Create and render game window.
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), SCREEN_TITLE);
    window.setFramerateLimit(FRAME_RATE);

    // Load spritesheet
    sf::Image spritesheet = load_sprites("../sprites/invader_sprites.png");

    // Create screenline
    sf::RectangleShape screenline(sf::Vector2f(SCREEN_WIDTH, 2));
    screenline.setPosition(0, 680);
    screenline.setFillColor(sf::Color::Green);

    // Create bullet
    PlayerBullet player_bul;

    // Create shields vector
    std::vector<Shield*> shields;
    
    // Initialize shields
    init_shields(spritesheet, shields);

    // Create player ship
    PlayerShip playership = PlayerShip(spritesheet, SCREEN_WIDTH / 2);


    // Begin game loop
    while (window.isOpen())
    {
        // Check for input
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                del_shields(shields);
                window.close();
                break;

            default:
                break;
            }
        }

        // Handle keyboard input (this is realtime keyboard input, as opposed to 'event-based'
        // Move playership
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && playership.getX() < (SCREEN_WIDTH - (playership.getWidth() / 2) - 10))
            playership.move(1);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && playership.getX() > (0 + (playership.getWidth() / 2) + 10))
            playership.move(-1);

        // Shoot bullet
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !player_bul.isShooting())
        {
            sf::Vector2f playerpos = playership.getAliveSprite().getPosition();
            player_bul.shoot(playerpos.x, playerpos.y);
        }

        // Update objects
        if (player_bul.isShooting())
            player_bul.move();

        // Display window and draw objects
        window.clear(BG_COLOR);
        window.draw(playership.getAliveSprite());
        window.draw(player_bul.getShape());
        draw_shields(window, shields);
        window.draw(screenline);
        window.display();

        updateFPS(window, fps_clock, fps_timer);
    }
}