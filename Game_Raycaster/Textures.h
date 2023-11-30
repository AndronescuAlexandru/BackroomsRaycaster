#pragma once
#include <unordered_map>


// list of wall texture types, in order as they appear in the full texture
enum class Textures {
    YellowWall,
    ConcreteWall1,
    ConcreteWall2,
    StoneBricks,
    Ice,
    DarkStoneBricks,
    Wood,
    Concrete,
    Truck,
    RedBricks,
    GarageDoor,
    CrackedConcrete,
    HotelWall,
    HotelWallDoor,
    HotelElevatorDoor,
    HotelWallPainting1,
};

// valid wall types and their texture for the world map
const std::unordered_map<char, Textures> wallTypes{
    {'#', Textures::StoneBricks},
    {'M', Textures::DarkStoneBricks},
    {'=', Textures::Wood},
    {'~', Textures::Ice},
    {'^', Textures::Concrete},
    {'&', Textures::CrackedConcrete },
    {'T', Textures::Truck },
    {'G', Textures::GarageDoor },
    {'H', Textures::HotelWall },
    {'D', Textures::HotelWallDoor },
    {'E', Textures::HotelElevatorDoor },
    {'P', Textures::HotelWallPainting1 },
};

const std::unordered_map<char, Textures> level0_wallTypes{
    {'#', Textures::YellowWall},
    {'@', Textures::ConcreteWall1},
    {'$', Textures::ConcreteWall2},
};