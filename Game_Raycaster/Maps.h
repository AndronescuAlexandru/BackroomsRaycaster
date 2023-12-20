#pragma once
#ifndef MAPS_H
#define MAPS_H

#include <SFML/Audio.hpp>
#include "Textures.h"
#include <fstream>

/*  map legend 

Levels 0,1,2:

# - Yellow wall 
^ - Yellow wall with red exit door
* - Dark Yellow Wall
% - concrete wall type 1
@ - concrete wall type 2
D - concrete wall type 1 with door
S - concrete wall type 2 with door
$ - Concrete wall type 1 with lamp
A - dirty concrete wall with door
W - dirty concrete wall
& - dirty concrete wall with pipes
. - empty space with yellow carpet texture ceiling and floor
; - empty space with concrete texture ceiling and floor

*/

class CurrentLevel
{
public:
    int ID; // saves the current level number where the player will be
    int ID_NextLevel;
    int MAP_WIDTH;
    int MAP_HEIGHT;
    int maxWallHeight;
    char* map;
    int* heightMap;
    int machineVolume;

    // colors used for floor and ceiling tiles in the current level
    sf::Color color;
    sf::Color color1;
    sf::Color color2;
    sf::Color floorColor;

    // sounds used in the current level
    sf::SoundBuffer soundBuffer;
    sf::SoundBuffer soundBuffer2;
    sf::SoundBuffer entitySoundBuffer;
    sf::SoundBuffer levelEntranceSoundBuffer;
    sf::SoundBuffer footstepsBuffer;
    sf::SoundBuffer machineSFXBuffer;

    sf::Sound AmbientSFX;
    sf::Sound AmbientSFX2;
    sf::Sound EntitySFX;
    sf::Sound levelEntranceSound;
    sf::Sound footsteps;
    sf::Sound machineSFX;

    sf::Texture Textures; // map textures

    CurrentLevel();
    void loadMapFile(const char* levelAdress);
    void loadLevel(sf::RenderWindow& window, sf::RenderStates& state);
    void loadHeightMapFile(const char* levelHeightMapAdress);
    ~CurrentLevel();

};

// level 0 map data

struct Level_0
{
    // map size
    const int MAP_WIDTH = 128;
    const int MAP_HEIGHT = 128;
    const short maxWallHeight = 3;

    const char* mapFileAdress = "Data/Maps/Level_0.map";
    const char* textureAdress = "Data/Textures/level_0_1_textures.png";
    const char* ambientSFXAdress = "Data/Audio/Level0LightAmbience.mp3";
    const char* footstepsSFXAdress = "Data/Audio/CarpetFootsteps.mp3";
    const char* entitySFXAdress = "Data/Audio/EntitySFX.mp3";

};

// level 1 map data

struct Level_1
{
    // map size
    const short MAP_WIDTH = 256;
    const short MAP_HEIGHT = 256;
    const short maxWallHeight = 3;

    const char* mapFileAdress = "Data/Maps/Level_1.map";
    const char* textureAdress = "Data/Textures/level_0_1_textures.png";
    const char* ambientSFXAdress = "Data/Audio/Level0LightAmbience.mp3";
    const char* footstepsSFXAdress = "Data/Audio/ConcreteFootsteps.mp3";
    const char* entitySFXAdress = "Data/Audio/EntitySFX.mp3";
    const char* levelEntranceSFXAdress = "Data/Audio/FallingSFX.mp3";

};

// level 2 map data

struct Level_2
{
    // map size
    const short MAP_WIDTH = 256;
    const short MAP_HEIGHT = 256;
    const short maxWallHeight = 2;
    const short machinePosX = 100;
    const short machinePosY = 100;
    short machineSFXVolume = 100;

    const char* mapFileAdress = "Data/Maps/Level_2.map";
    const char* textureAdress = "Data/Textures/level_2_textures.png";
    const char* ambientSFXAdress = "Data/Audio/Level2_AmbientSFX.mp3";
    const char* ambientSFXAdress2 = "Data/Audio/Level2_AmbientSFX2.mp3";
    const char* machineSFXAdress = "Data/Audio/Level2_MachineSFX.mp3";
    const char* footstepsSFXAdress = "Data/Audio/ConcreteFootsteps.mp3";
    const char* entitySFXAdress = "Data/Audio/EntitySFX.mp3";
    const char* levelEntranceSFXAdress = "Data/Audio/FallingSFX.mp3";

};

// level 3 map data

struct Level_3
{
    // map size
    const short MAP_WIDTH = 64;
    const short MAP_HEIGHT = 64;
    const short maxWallHeight = 1;

    const char* mapFileAdress = "Data/Maps/Level_3.map";
    const char* heightMapFileAdress = "Data/Maps/Level_3_Height.map";
    const char* textureAdress = "Data/Textures/level_3_textures.png";
    const char* ambientSFXAdress = "Data/Audio/Level0LightAmbience.mp3";
    const char* footstepsSFXAdress = "Data/Audio/ConcreteFootsteps.mp3";
    const char* entitySFXAdress = "Data/Audio/EntitySFX.mp3";
    const char* levelEntranceSFXAdress = "Data/Audio/FallingSFX.mp3";

};

// level 4 map data

struct Level_4
{
    // map size
    const short MAP_WIDTH = 128;
    const short MAP_HEIGHT = 128;
    const short maxWallHeight = 3;

    const char* mapFileAdress = "Data/Maps/Level_4.map";
    const char* heightMapFileAdress = "Data/Maps/Level_4_Height.map";
    const char* textureAdress = "Data/Textures/level_4_textures.png";
    const char* ambientSFXAdress = "Data/Audio/Level0LightAmbience.mp3";
    const char* footstepsSFXAdress = "Data/Audio/CarpetFootsteps.mp3";
    const char* entitySFXAdress = "Data/Audio/EntitySFX.mp3";
    const char* levelEntranceSFXAdress = "Data/Audio/ElevatorFunctioning.mp3";

};

// level 5 map data

struct Level_5
{
    // map size
    const short MAP_WIDTH = 64;
    const short MAP_HEIGHT = 64;
    const short maxWallHeight = 1;

    const char* mapFileAdress = "Data/Maps/Level_5.map";
    const char* textureAdress = "Data/Textures/textures.png";
    const char* ambientSFXAdress = "Data/Audio/Level0LightAmbience.mp3";
    const char* footstepsSFXAdress = "Data/Audio/ConcreteFootsteps.mp3";
    const char* entitySFXAdress = "Data/Audio/EntitySFX.mp3";
    const char* levelEntranceSFXAdress = "Data/Audio/FallingSFX.mp3";

};

// level Run map data

struct Level_Run
{
    // map size
    const short MAP_WIDTH = 64;
    const short MAP_HEIGHT = 64;
    const short maxWallHeight = 1;

    const char* mapFileAdress = "Data/Maps/Level_Run.map";
    const char* textureAdress = "Data/Textures/level_Run_textures.png";
    const char* ambientSFXAdress = "Data/Audio/LevelRunAmbientSFX.mp3";
    const char* footstepsSFXAdress = "Data/Audio/ConcreteFootsteps.mp3";
    const char* entitySFXAdress = "Data/Audio/EntitySFX.mp3";
    const char* levelEntranceSFXAdress = "Data/Audio/FallingSFX.mp3";

};

// level 0.01

struct Level_0_0_1
{
    // map size
    const short MAP_WIDTH = 64;
    const short MAP_HEIGHT = 385;

    const short maxWallHeight = 3;

    const char* mapFileAdress = "Data/Maps/Level_0_0_1.map";
    const char* textureAdress = "Data/Textures/level_0_textures.png";
    const char* ambientSFXAdress = "Data/Audio/Level0LightAmbience.mp3";
    const char* footstepsSFXAdress = "Data/Audio/CarpetFootsteps.mp3";
    const char* entitySFXAdress = "Data/Audio/EntitySFX.mp3";
    const char* levelEntranceSFXAdress = "Data/Audio/Level0_01EntranceSFX.mp3";


};

#endif
