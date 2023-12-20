#pragma once
#ifndef TEXTURES_H
#define TEXTURES_H


#include <unordered_map>
#include <SFML/Graphics.hpp>

// list of wall texture types, in order as they appear in the full texture

enum class TexturesLevel0_1 {
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
    ConcreteFloor,
};

enum class TexturesLevel2 {
    ConcreteWithDoor,
    ConcreteTexture,
    ConcreteElevatorDoor,
    ConcreteFireDoor,
    ConcreteWallWithPipes,
    ConcreteWallWithPipes2,
    ConcreteWallWithPipes3,
    ConcreteFloor,
    MachineTexture,
};

enum class TexturesLevel3 {
    YellowTileWall,
    Floor,
    YellowConcreteWall,
    YellowTileWallElevator,
    ElevatorInsideFront,
    ElevatorInsideBack,
    ElevatorInsideSides,
    ElevatorInsideFloor,
    MachineTexture,
    Wall,
};

enum class TexturesLevel4 {
    OfficeWall,
    OfficeCarpet,
    OfficeWallWithDoor,
    OfficeWallElevatorDoor,
    ElevatorInsideFront,
    ElevatorInsideBack,
    ElevatorInsideSides,
    ElevatorInsideFloor,
    OfficeWallWithWindow,
    OfficeWallWithRadiator,
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

const std::unordered_map<char, TexturesLevel0_1> level0_wallTypes
{
    {'#', TexturesLevel0_1::YellowWall},
    {'@', TexturesLevel0_1::YellowWallRedDoor},
    {'*', TexturesLevel0_1::NoClipYellowWall},
    {'.', TexturesLevel0_1::YellowCarpet},
    {'@', TexturesLevel0_1::ConcreteTexture2},
    {'%', TexturesLevel0_1::ConcreteTexture1},
    {'C', TexturesLevel0_1::CrackedConcrete},
    {'S', TexturesLevel0_1::ConcreteTexture2Door},
    {'D', TexturesLevel0_1::ConcreteTexture1Door},
    {'$', TexturesLevel0_1::ConcreteWallWithLamp},
    {';', TexturesLevel0_1::ConcreteFloor},
    {',', TexturesLevel0_1::ConcreteFloor}, // used for simulating light on ceilings and floors from light sources
    
};

const std::unordered_map<char, TexturesLevel2> level2_wallTypes
{
    {'A', TexturesLevel2::ConcreteWithDoor},
    {'W', TexturesLevel2::ConcreteTexture},
    {'&', TexturesLevel2::ConcreteElevatorDoor},
    {'@', TexturesLevel2::ConcreteFireDoor},
    {'#', TexturesLevel2::ConcreteWallWithPipes},
    {'$', TexturesLevel2::MachineTexture},
    {'(', TexturesLevel2::ConcreteWallWithPipes2},
    {')', TexturesLevel2::ConcreteWallWithPipes3},
    {'.', TexturesLevel2::ConcreteFloor},
    {',', TexturesLevel2::ConcreteFloor}, // used for simulating light on ceilings and floors from light sources
};

const std::unordered_map<char, TexturesLevel3> level3_wallTypes
{
    {'.', TexturesLevel3::Floor},
    {'#', TexturesLevel3::YellowTileWall},
    {'W', TexturesLevel3::YellowConcreteWall},
    {'&', TexturesLevel3::YellowTileWallElevator},
    {'(', TexturesLevel3::ElevatorInsideFront},
    {')', TexturesLevel3::ElevatorInsideBack},
    {'!', TexturesLevel3::ElevatorInsideSides},
    {',', TexturesLevel3::MachineTexture},
    {'$', TexturesLevel3::MachineTexture},  
};

const std::unordered_map<char, TexturesLevel4> level4_wallTypes
{
    {'#', TexturesLevel4::OfficeWall},
    {'.', TexturesLevel4::OfficeCarpet},
    {'@', TexturesLevel4::OfficeWallWithDoor},
    {'$', TexturesLevel4::OfficeWallElevatorDoor},
    {'%', TexturesLevel4::OfficeWallWithWindow},
    {'&', TexturesLevel4::OfficeWallWithRadiator},
    {'(', TexturesLevel4::ElevatorInsideFront},
    {')', TexturesLevel4::ElevatorInsideBack},
    {'!', TexturesLevel4::ElevatorInsideSides},
    {',', TexturesLevel4::ElevatorInsideFloor},

};


const std::unordered_map<char, TexturesLeveRun> levelRun_wallTypes
{
    {'#', TexturesLeveRun::RedWall},
    {'@', TexturesLeveRun::RedWallWithDoor},
    {'.', TexturesLeveRun::RedCeilingFloor},
};

#endif // !TEXTURES_H