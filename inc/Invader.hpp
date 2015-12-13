#ifndef INVADER_H
#define INVADER_H

#include <SFML/Graphics.hpp>

/* An individual Space Invader. */

class Invader
{
// This is up here because I like private members first,
// but this needs to be public and is used by a private member.
// The different types of Invaders
public: enum InvaderType { LARGE, MEDIUM, SMALL };

private:
    static const unsigned TXTR_HEIGHT = 35; // Same for all Invader types

    static const unsigned DEATH_TICK_MAX = 10; // How long death sprite should show
    static const int SPEED = 10; // How far the invader moves each step

    // Invaders have 2 frames in their animation, and 1 for death
    sf::Image &spritesheet;
    sf::Texture frame1_txtr;
    sf::Texture frame2_txtr;
    sf::Texture frame3_txtr;
    sf::Sprite sprite;

    InvaderType type;
    unsigned score_value; // How many points the invader is worth when killed
    int move_dir; // 1 is right, -1 is left, 0 is not moving
    bool is_dead;
    bool is_exploding;
    int frame_on; // Either 1 or 2

    unsigned death_tick; // Keep track of how long death sprite has shown

    // Load textures for each frame given a rectangle containing location of texture on spritesheet
    void setTextures(sf::IntRect frame1, sf::IntRect frame2, sf::IntRect frame3);

    // Cycle through animation
    void flipFrame();

public:
    Invader(sf::Image &spritesheet, InvaderType type);
    sf::Sprite& getSprite(){ return this->sprite; }
    unsigned getScoreValue(){ return this->score_value; }
    bool isDead(){ return this->is_dead; }
    bool isExploding(){ return this->is_exploding; }
    
    // Called when Invader is hit by bullet
    void die();

    // Called every movement tick
    void move();

    // The invader shifts down one row
    void dropDown();

    // The invader reverses its direction of movement
    void reverseDir();

    // Checks if Invader hit edge of screen
    bool checkHitEdge(int screenw);
    
    // Increment death tick and hide once hit max
    void incDeathTick();

};

#endif