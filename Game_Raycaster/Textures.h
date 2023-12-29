#pragma once
#ifndef TEXTURES_H
#define TEXTURES_H


#include <unordered_map>
#include <SFML/Graphics.hpp>

// list of wall texture types, in order as they appear in the full texture

enum class TexturesLevel0 {
    YellowWall,
    YellowCarpet,
    YellowWallRedDoor,
    NoClipYellowWall,  
};

enum class TexturesLevel1 {  
    ConcreteWallWithLamp,
    CrackedConcrete,
    ConcreteTexture1,
    ConcreteTexture2,
    ConcreteTexture1Door,
    ConcreteTexture2Door,
    ConcreteFloor,
    ConcreteTexture3,
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

enum class TexturesLevel5 {
    HotelWall,
    HotelWallWithDoor,
    HotelWallElevator,
    HotelWallPainting,
    ElevatorInsideFront,
    ElevatorInsideBack,
    ElevatorInsideSides,
    ElevatorInsideFloor,
    HotelFloor,
    HotelCeiling,
    RoomWall,
    RoomFloor,
    RoomCeiling,
    RoomWallWithDoor,
    HotelWall2Painting,
    HotelWall2,
};

enum class TexturesLevel6 {
    HotelWall,
    HotelWallWithDoor,
    HotelWallElevator,
    HotelWallPainting,
    ElevatorInsideFront,
    ElevatorInsideBack,
    ElevatorInsideSides,
    ElevatorInsideFloor,
    HotelFloor,
    HotelCeiling,
    RoomWall,
    RoomFloor,
    RoomCeiling,
    RoomWallWithDoor,
    HotelWall2Painting,
    HotelWall2,
};


enum class TexturesLeveRun {
    RedWallWithDoor,
    RedCeilingFloor,
    RedWall,
};

// valid wall types and their texture for the world map

const std::unordered_map<char, TexturesLevel0> level0_wallTypes
{
    {'#', TexturesLevel0::YellowWall},
    {'@', TexturesLevel0::YellowWallRedDoor},
    {'*', TexturesLevel0::NoClipYellowWall},
    {'.', TexturesLevel0::YellowCarpet},
    
};

const std::unordered_map<char, TexturesLevel1> level1_wallTypes
{   
    {'@', TexturesLevel1::ConcreteTexture2},
    {'%', TexturesLevel1::ConcreteTexture1},
    {'C', TexturesLevel1::CrackedConcrete},
    {'S', TexturesLevel1::ConcreteTexture2Door},
    {'D', TexturesLevel1::ConcreteTexture1Door},
    {'$', TexturesLevel1::ConcreteWallWithLamp},
    {';', TexturesLevel1::ConcreteFloor},
    {',', TexturesLevel1::ConcreteFloor}, // used for simulating light on ceilings and floors from light sources

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
    {'.', TexturesLevel2::ConcreteTexture},
    {',', TexturesLevel2::ConcreteTexture}, // used for simulating light on ceilings and floors from light sources
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
    {',', TexturesLevel3::ElevatorInsideFloor},
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

const std::unordered_map<char, TexturesLevel5> level5_wallTypes
{
    {'#', TexturesLevel5::HotelWall},
    {'@', TexturesLevel5::HotelWallWithDoor},
    {'$', TexturesLevel5::HotelWallElevator},
    {'%', TexturesLevel5::HotelWallPainting},
    {'^', TexturesLevel5::ElevatorInsideFront},
    {'&', TexturesLevel5::ElevatorInsideBack},
    {'!', TexturesLevel5::ElevatorInsideSides},
    {'*', TexturesLevel5::RoomWall},
    {']', TexturesLevel5::RoomWallWithDoor},
    {'W', TexturesLevel5::HotelWall2},
    {'P', TexturesLevel5::HotelWall2Painting},
    {',', TexturesLevel5::RoomFloor},
    {':', TexturesLevel5::RoomCeiling},
    {'.', TexturesLevel5::HotelFloor},
    {';', TexturesLevel5::RoomCeiling},
    {'_', TexturesLevel5::ElevatorInsideFloor},

};

const std::unordered_map<char, TexturesLeveRun> levelRun_wallTypes
{
    {'#', TexturesLeveRun::RedWall},
    {'@', TexturesLeveRun::RedWallWithDoor},
    {'.', TexturesLeveRun::RedCeilingFloor},
};

#endif // !TEXTURES_H