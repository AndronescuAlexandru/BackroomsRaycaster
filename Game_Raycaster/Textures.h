#pragma once
#ifndef TEXTURES_H
#define TEXTURES_H


#include <unordered_map>
#include <SFML/Graphics.hpp>

// list of wall texture types, in order as they appear in the full texture

enum class TexturesLevel0_1_2 {
    YellowWall,
    YellowCarpet,
    YellowWallRedDoor,
    NoClipYellowWall,
    ConcreteWallWithLamp,
    CrackedConcrete,
    ConcreteTexture1,
    ConcreteTexture2,
    ConcreteTexture1Door,
    ConcreteTexture2Door,
    Concrete3WithDoor,
    Concrete3Texture,
    Concrete3WallWithPipes,
    Concrete3WallWithPipes2,
    ConcreteFloor,
    Concrete3WallWithPipes3,
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
const std::unordered_map<char, Textures> wallTypes
{
    {'M', Textures::DarkStoneBricks},
    {'=', Textures::Wood},
    {'~', Textures::Ice},
    {'^', Textures::Concrete},
    {'&', Textures::CrackedConcrete},
    {'T', Textures::Truck },
    {'G', Textures::GarageDoor },
    {'H', Textures::HotelWall },
    {'D', Textures::HotelWallDoor },
    {'E', Textures::HotelElevatorDoor },
    {'P', Textures::HotelWallPainting1 },
    {'S', Textures::HotelWallPainting1 },
};

const std::unordered_map<char, TexturesLevel0_1_2> level0_wallTypes
{
    {'#', TexturesLevel0_1_2::YellowWall},
    {'^', TexturesLevel0_1_2::YellowWallRedDoor},
    {'*', TexturesLevel0_1_2::NoClipYellowWall},
    {'.', TexturesLevel0_1_2::YellowCarpet},
    {'@', TexturesLevel0_1_2::ConcreteTexture2},
    {'%', TexturesLevel0_1_2::ConcreteTexture1},
    {'A', TexturesLevel0_1_2::Concrete3WithDoor},
    {'W', TexturesLevel0_1_2::Concrete3Texture},
    {'C', TexturesLevel0_1_2::CrackedConcrete},
    {'S', TexturesLevel0_1_2::ConcreteTexture2Door},
    {'D', TexturesLevel0_1_2::ConcreteTexture1Door},
    {'$', TexturesLevel0_1_2::ConcreteWallWithLamp},
    {'&', TexturesLevel0_1_2::Concrete3WallWithPipes},
    {'(', TexturesLevel0_1_2::Concrete3WallWithPipes2},
    {')', TexturesLevel0_1_2::Concrete3WallWithPipes3},
    {';', TexturesLevel0_1_2::ConcreteFloor},
    {',', TexturesLevel0_1_2::ConcreteFloor}, // used for simulating light on ceilings and floors from light sources
    
};

const std::unordered_map<char, TexturesLeveRun> levelRun_wallTypes
{
    {'#', TexturesLeveRun::RedWall},
    {'@', TexturesLeveRun::RedWallWithDoor},
    {'.', TexturesLeveRun::RedCeilingFloor},
};

#endif // !TEXTURES_H