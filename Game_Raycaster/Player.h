#pragma once
#include<SFML/Graphics.hpp>

class Player
{
public:
    sf::Vector2f position; // coordinates in worldMap
    sf::Vector2f direction; // direction, relative to (0,0)
    sf::Vector2f plane; // 2d raycaster version of the camera plane,

    float playerSize; // dimensions of player collision box, in tiles
    float playerMoveSpeed; // player movement speed in tiles per second
    float playerRotateSpeed; // player rotation speed in radians per second

    Player();
    void setPlayerNewPos(short x, short y);
    ~Player();

private:

};