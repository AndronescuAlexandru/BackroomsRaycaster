#pragma once
#include <unordered_map>
#include<SFML/Graphics.hpp>

// list of wall texture types, in order as they appear in the full texture

enum class TexturesLevel0_1_2 {
    YellowWall,
    YellowCarpet,
    ConcreteTexture1,
    ConcreteTexture2,
    ConcreteWallWithLamp,
    DirtyConcreteWallWithPipes,
    YellowWallRedDoor,
    NoClipYellowWall,
    ConcreteTexture1Door,
    ConcreteTexture2Door,
    DirtyConcreteWithDoor,
    DirtyConcreteTexture,
};

enum class TexturesLeveRun {
    RedWallWithDoor,
    RedCeilingFloor,
    RedWall,
};

enum class Textures {
    YellowWall,
    ConcreteTexture1,
    ConcreteTexture2,
    YellowCeiling,
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

const std::unordered_map<char, TexturesLevel0_1_2> level0_wallTypes{
    {'#', TexturesLevel0_1_2::YellowWall},
    {'@', TexturesLevel0_1_2::ConcreteTexture2},
    {'%', TexturesLevel0_1_2::ConcreteTexture1},
    {'A', TexturesLevel0_1_2::DirtyConcreteWithDoor},
    {'W', TexturesLevel0_1_2::DirtyConcreteTexture},
    {'S', TexturesLevel0_1_2::ConcreteTexture2Door},
    {'D', TexturesLevel0_1_2::ConcreteTexture1Door},
    {'$', TexturesLevel0_1_2::ConcreteWallWithLamp},
    {'&', TexturesLevel0_1_2::DirtyConcreteWallWithPipes},
    {'^', TexturesLevel0_1_2::YellowWallRedDoor},
    {'*', TexturesLevel0_1_2::NoClipYellowWall},
    {'.', TexturesLevel0_1_2::YellowCarpet},
    {';', TexturesLevel0_1_2::ConcreteTexture1},
    
};

const std::unordered_map<char, TexturesLeveRun> levelRun_wallTypes{
    {'#', TexturesLeveRun::RedWall},
    {'@', TexturesLeveRun::RedWallWithDoor},
    {'.', TexturesLeveRun::RedCeilingFloor},
};