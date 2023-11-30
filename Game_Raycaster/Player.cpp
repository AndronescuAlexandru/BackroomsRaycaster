#include "Player.h"

Player::Player()
{
    position.x = 1.5;
    position.y = 2.0;
    direction.x = 0.0;
    direction.y = 1.0;
    plane.x = -0.66;
    plane.y = 0.0;
    playerSize = 0.375;
    playerMoveSpeed = 5.0;
    playerRotateSpeed = 3.0;
}

Player::~Player()
{

}

void Player::setPlayerNewPos(short x, short y)
{
    position.x = x;
    position.y = y;
}
