#include "Maps.h"
#include "Player.h"

struct Level_0 level_0;
struct Level_1 level_1;

extern float wallShading;
extern float ceilingShading;
extern float floorShading;

extern Player player;
extern bool noSaveFile;

CurrentLevel::CurrentLevel()
{
    ID = 0;
    MAP_WIDTH = 256;
    MAP_HEIGHT = 256;
    maxWallHeight = 1;
}

CurrentLevel::~CurrentLevel()
{
}

void CurrentLevel::loadMapFile(const char* levelAdress)
{
    std::ifstream levelFile(levelAdress);

    map = (char*)malloc((MAP_HEIGHT * MAP_WIDTH) * sizeof(char));

    if (map == nullptr)
    {
        printf("Could not allocate memory for pointer level_0.map!");
        return;
    }

    if (!levelFile.is_open())
    {
        printf("Could not open data file Level_0.txt or file doesn't exist; Exiting program");
        free(map);
        return;
    }

    for (int i = 0; i <= MAP_HEIGHT * MAP_WIDTH; i++)
    {
        levelFile >> map[i];
        if (levelFile.eof())
            break;
    }

    levelFile.close();
}

void CurrentLevel::loadLevel(sf::RenderWindow& window, sf::RenderStates& state)
{
    // loads files specific for each level and set render state that uses the texture
    switch (ID)
    {
    case 0:
    {
        loadMapFile(level_0.mapFileAdress);
        if (!Textures.loadFromFile(level_0.textureAdress))
        {
            printf("Cannot open sound file %c\n", level_0.textureAdress);
        }

        if (!soundBuffer.loadFromFile(level_0.ambientSFXAdress))
        {
            printf("Cannot open sound file %c\n", level_0.ambientSFXAdress);
        }

        if (!footstepsBuffer.loadFromFile(level_0.footstepsSFXAdress))
            printf("Cannot open sound file %c\n", level_0.footstepsSFXAdress);

        footsteps.setBuffer(footstepsBuffer);

        MAP_HEIGHT = level_0.MAP_HEIGHT;
        MAP_WIDTH = level_0.MAP_WIDTH;
        maxWallHeight = level_0.maxWallHeight;

        state.texture = &Textures;

        AmbientSFX.setBuffer(soundBuffer);
        AmbientSFX.setVolume(50);
        AmbientSFX.play();
        AmbientSFX.setLoop(true);

        wallShading = 1.1;

        color1 = sf::Color(182, 179, 102);
        color2 = sf::Color(255, 255, 255);

        floorColor = sf::Color(178, 163, 106);

        if(noSaveFile)
            player.setPlayerNewPos(2, 2);

        break;
    }
    case 1:
        window.clear(sf::Color::Black);
        window.display();
        loadMapFile(level_1.mapFileAdress);

        if (!levelEntranceSoundBuffer.loadFromFile(level_1.levelEntranceSFXAdress))
        {
            //std::cout << "Cannot open sound file Level0_01EntranceSFX.mp3!\n";
        }

        levelEntranceSound.setBuffer(levelEntranceSoundBuffer);
        levelEntranceSound.play();

        sf::sleep(sf::seconds(2));

        if (!Textures.loadFromFile(level_1.textureAdress))
        {
            printf("Cannot open sound file %c\n", level_1.textureAdress);
        }

        if (!soundBuffer.loadFromFile(level_1.ambientSFXAdress))
        {
            printf("Cannot open sound file %c\n", level_1.ambientSFXAdress);
        }

        if (!footstepsBuffer.loadFromFile(level_1.footstepsSFXAdress))
            printf("Cannot open sound file %c\n", level_1.footstepsSFXAdress);

        footsteps.setBuffer(footstepsBuffer);

        MAP_HEIGHT = level_1.MAP_HEIGHT;
        MAP_WIDTH = level_1.MAP_WIDTH;
        maxWallHeight = level_1.maxWallHeight;

        state.texture = &Textures;

        AmbientSFX.setBuffer(soundBuffer);
        AmbientSFX.setVolume(10);
        AmbientSFX.play();
        AmbientSFX.setLoop(true);

        wallShading = 1.5;
        ceilingShading = 1.5;
        floorShading = 1.5;

        color1 = sf::Color(100, 100, 100);
        color2 = sf::Color(100, 100, 100); //sf::Color(129, 124, 121);
        //color.r /= ceilingShading;
        //color.g /= ceilingShading;
        //color.b /= ceilingShading;

        floorColor = sf::Color(154, 153, 149);
        floorColor.r /= floorShading;
        floorColor.g /= floorShading;
        floorColor.b /= floorShading;

        break;
    case 2:
        loadMapFile(level_2.mapFileAdress);
        if (!Textures.loadFromFile(level_2.textureAdress))
        {
            printf("Cannot open file %c\n", level_2.textureAdress);
        }

        if (!soundBuffer.loadFromFile(level_2.ambientSFXAdress))
        {
            printf("Cannot open sound file %c\n", level_2.ambientSFXAdress);
        }

        if (!soundBuffer2.loadFromFile(level_2.ambientSFXAdress2))
        {
            printf("Cannot open sound file %c\n", level_2.ambientSFXAdress2);
        }

        if (!footstepsBuffer.loadFromFile(level_2.footstepsSFXAdress))
            printf("Cannot open sound file %c\n", level_2.footstepsSFXAdress);

        if (!machineSFXBuffer.loadFromFile(level_2.machineSFXAdress))
        {
            printf("Cannot open sound file %c\n", level_2.ambientSFXAdress);
        }

        machineVolume = level_2.machineSFXVolume;

        AmbientSFX.setBuffer(soundBuffer);
        AmbientSFX.setVolume(10);
        AmbientSFX.play();
        AmbientSFX.setLoop(true);

        AmbientSFX2.setBuffer(soundBuffer2);
        AmbientSFX2.setVolume(20);
        //AmbientSFX2.play();
        AmbientSFX2.setLoop(true);

        machineSFX.setBuffer(machineSFXBuffer);
        machineSFX.setVolume(100);
        machineSFX.play();
        machineSFX.setLoop(true);

        footsteps.setBuffer(footstepsBuffer);

        MAP_HEIGHT = level_2.MAP_HEIGHT;
        MAP_WIDTH = level_2.MAP_WIDTH;
        maxWallHeight = level_2.maxWallHeight;

        state.texture = &Textures;

        wallShading = 2.5;
        ceilingShading = 2.5;
        floorShading = 2.5;

        color1 = sf::Color(154, 153, 149);
        color2 = sf::Color(154, 153, 149); //sf::Color(129, 124, 121);
        color.r /= ceilingShading;
        color.g /= ceilingShading;
        color.b /= ceilingShading;

        floorColor = sf::Color(154, 153, 149);
        floorColor.r /= floorShading;
        floorColor.g /= floorShading;
        floorColor.b /= floorShading;

        player.setPlayerNewPos(1.5, 1.5);

        break;
    case 3:
        loadMapFile(level_3.mapFileAdress);
        if (!Textures.loadFromFile(level_3.textureAdress))
        {
            printf("Cannot open sound file %c\n", level_3.textureAdress);
        }

        if (!soundBuffer.loadFromFile(level_3.ambientSFXAdress))
        {
            printf("Cannot open sound file %c\n", level_3.ambientSFXAdress);
        }

        if (!footstepsBuffer.loadFromFile(level_3.footstepsSFXAdress))
            printf("Cannot open sound file %c\n", level_3.footstepsSFXAdress);

        footsteps.setBuffer(footstepsBuffer);

        MAP_HEIGHT = level_3.MAP_HEIGHT;
        MAP_WIDTH = level_3.MAP_WIDTH;
        maxWallHeight = level_3.maxWallHeight;

        state.texture = &Textures;

        color1 = sf::Color(43, 162, 198);
        color2 = sf::Color(96, 96, 96);
        floorColor = sf::Color(96, 96, 96);

        break;
    case 4:
        loadMapFile(level_Run.mapFileAdress);
        if (!Textures.loadFromFile(level_Run.textureAdress))
        {
            printf("Cannot open sound file %c\n", level_Run.textureAdress);
        }

        if (!soundBuffer.loadFromFile(level_Run.ambientSFXAdress))
        {
            printf("Cannot open sound file %c\n", level_Run.ambientSFXAdress);
        }

        if (!footstepsBuffer.loadFromFile(level_Run.footstepsSFXAdress))
            printf("Cannot open sound file %c\n", level_Run.footstepsSFXAdress);

        footsteps.setBuffer(footstepsBuffer);

        MAP_HEIGHT = level_Run.MAP_HEIGHT;
        MAP_WIDTH = level_Run.MAP_WIDTH;
        maxWallHeight = level_Run.maxWallHeight;

        state.texture = &Textures;

        color1 = sf::Color(154, 153, 149);
        color2 = sf::Color(154, 153, 149);

        floorColor = sf::Color(154, 153, 149);
        floorColor.r /= floorShading;
        floorColor.g /= floorShading;
        floorColor.b /= floorShading;

        AmbientSFX.setBuffer(soundBuffer);
        AmbientSFX.play();
        AmbientSFX.setLoop(true);

        break;
    case 5:
        loadMapFile(level_5.mapFileAdress);
        if (!Textures.loadFromFile(level_5.textureAdress))
        {
            printf("Cannot open sound file %c\n", level_5.textureAdress);
        }

        if (!soundBuffer.loadFromFile(level_5.ambientSFXAdress))
        {
            printf("Cannot open sound file %c\n", level_5.ambientSFXAdress);
        }

        if (!footstepsBuffer.loadFromFile(level_5.footstepsSFXAdress))
            printf("Cannot open sound file %c\n", level_5.footstepsSFXAdress);

        footsteps.setBuffer(footstepsBuffer);

        MAP_HEIGHT = level_5.MAP_HEIGHT;
        MAP_WIDTH = level_5.MAP_WIDTH;
        maxWallHeight = level_5.maxWallHeight;

        state.texture = &Textures;

        color1 = sf::Color(154, 153, 149);
        color2 = sf::Color(154, 153, 149);

        floorColor = sf::Color(154, 153, 149);
        floorColor.r /= floorShading;
        floorColor.g /= floorShading;
        floorColor.b /= floorShading;
        break;
    case 6:

        loadMapFile(level_0_0_1.mapFileAdress);
        if (!Textures.loadFromFile(level_0_0_1.textureAdress))
        {
            printf("Cannot open sound file %c\n", level_0_0_1.textureAdress);
        }

        if (!soundBuffer.loadFromFile(level_0_0_1.ambientSFXAdress))
        {
            printf("Cannot open sound file %c\n", level_0_0_1.ambientSFXAdress);
        }

        if (!footstepsBuffer.loadFromFile(level_0_0_1.footstepsSFXAdress))
            printf("Cannot open sound file %c\n", level_0_0_1.footstepsSFXAdress);

        if (!levelEntranceSoundBuffer.loadFromFile(level_0_0_1.levelEntranceSFXAdress))
        {
            printf("Cannot open sound file %c\n", level_0_0_1.levelEntranceSFXAdress);
        }

        footsteps.setBuffer(footstepsBuffer);

        player.position.x = 1.6;
        player.position.y = 2.5;

        MAP_HEIGHT = level_0_0_1.MAP_HEIGHT;
        MAP_WIDTH = level_0_0_1.MAP_WIDTH;
        maxWallHeight = level_0_0_1.maxWallHeight;

        state.texture = &Textures;

        levelEntranceSound.setBuffer(levelEntranceSoundBuffer);
        levelEntranceSound.play();

        AmbientSFX.setBuffer(soundBuffer);
        AmbientSFX.play();
        AmbientSFX.setLoop(true);

        wallShading = 1.1;

        color1 = sf::Color(182, 179, 102);
        color2 = sf::Color(255, 255, 255);

        //color1.r /= wallShading;
        //color1.g /= wallShading;
        //color1.b /= wallShading;

        floorColor = sf::Color(178, 163, 106);
        //floorColor.r /= wallShading;
        //floorColor.g /= wallShading;
        //floorColor.b /= wallShading;

        break;
    default:
        if (!footstepsBuffer.loadFromFile("Data/Audio/CarpetFootsteps.mp3"))
            //std::cout << "Could not open sound file CarpetFootsteps2.mp3!\n";

        footsteps.setBuffer(footstepsBuffer);

        printf("Could not load any levels!");

        break;
    }
}