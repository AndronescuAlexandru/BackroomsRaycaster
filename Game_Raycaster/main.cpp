// Version : 0.7.1

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <algorithm>
#include <SFML/System/Time.hpp>
#include <Windows.h>
#include <fstream>

#include "Maps.h"
#include "Player.h"
#include "Textures.h"
#include "Constants.h"
#include "Menus.h"
#include "FileOperations.h"

extern Player player;
extern CurrentLevel currentLevel;

extern struct Level_0 level_0;
extern struct Level_1 level_1;
extern struct Level_2 level_2;
extern struct Level_3 level_3;
extern struct Level_Run level_Run;
extern struct Level_5 level_5;
extern struct Level_0_0_1 level_0_0_1;

sf::SoundBuffer soundEffectsBuffer;
sf::Sound soundEffect;

bool keyboardPressed = false;
bool levelChanged = false;
bool playerIsMoving = false;
bool footstepsPlaying = false;
bool debugMode = true;
bool noSaveFile;
bool hasFocus = true;

float wallShading = 1.5;
float ceilingShading = 1.5;
float floorShading = 1.5;
int gameState = 0;
int RENDER_DISTANCE = 8;
float menuSpriteScaleX = 1.25, menuSpriteScaleY = 1;

// sets default screen resolution

int screenWidth = 800;
int screenHeight = 600;

float cameraHeight = 0.66; // height of player camera(1.0 is ceiling, 0.0 is floor)

const int texture_size = 512; // size(width and height) of texture that will hold all wall textures
const int texture_wall_size = 128; // size(width and height) of each wall type in the full texture

const float fps_refresh_time = 0.1; // time between FPS text refresh. FPS is smoothed out over this time

char GetTile(int x, int y, short level)
{
    return currentLevel.map[y * currentLevel.MAP_WIDTH + x];
}

float GetHeight(int x, int y, short level)
{
    return currentLevel.maxWallHeight;
}

void UpdateFPS(sf::Text& fpsText, float dt, int64_t frame_time_micro)
{
    static float dt_counter = 0.0; // delta time for multiple frames, for calculating FPS smoothly
    static int frame_counter = 0; // counts frames for FPS calculation
    char frameInfoString[sizeof("FPS: *****.*, Frame time: ******")];

    // Update FPS, smoothed over time
    if (dt_counter >= fps_refresh_time)
    {
        float fps = (float)frame_counter / dt_counter;
        frame_time_micro /= frame_counter;
        snprintf(frameInfoString, sizeof(frameInfoString), "FPS: %3.1f, Frame time: %6ld, X: %3.1f,Y: %3.1f", fps, frame_time_micro);
        fpsText.setString(frameInfoString);
        dt_counter = 0.0;
        frame_counter = 0;
        frame_time_micro = 0;
    }
    dt_counter += dt;
    frame_counter++;
}

float get_degrees(const float i_degrees)
{
    return static_cast<float>(fmod(360 + fmod(i_degrees, 360), 360));
}

float deg_to_rad(const float i_degrees)
{
    return PI * get_degrees(i_degrees) / 180;
}

void PlaySFX(short type, sf::Sound& soundEffect)
{

    switch (type)
    {
    case 0:
    {
        if (!soundEffectsBuffer.loadFromFile("Data/Audio/MetalDoorOpeningClosing.mp3"))
            std::cout << "Could not open sound file MetalDoorOpeningClosing.mp3!\n";

        soundEffect.setBuffer(soundEffectsBuffer);
        soundEffect.play();

        break;
    }
    case 1:
    {
        if (!soundEffectsBuffer.loadFromFile("Data/Audio/FallingSFX.mp3"))
            std::cout << "Could not open sound file NoclipSFX1.mp3!\n";

        soundEffect.setBuffer(soundEffectsBuffer);
        soundEffect.play();
        break;
    }
    default:
        break;
    }
}

void KeyboardInput(bool hasFocus, Player& player, sf::Vector2f size, float dt) // handles keyboard input
{
    bool coordinatesRecentlyChanged = false;
    if (hasFocus)
    {
        using kb = sf::Keyboard;

        // moving forward or backwards (1.0 or -1.0)
        float moveForward = 0.0;

        if (gameState == IN_GAME)
        {
            // get input
            if (kb::isKeyPressed(kb::Up))
            {
                moveForward = 1.0;
            }
            else
                if (kb::isKeyPressed(kb::Down))
                {
                    moveForward = -1.0;
                }
                else
                {
                    footstepsPlaying = false;
                    currentLevel.footsteps.stop();
                }

            // debug controls
            if (debugMode == true)
            {
                if (kb::isKeyPressed(kb::A)) // shortens the render distance and darkens the wall shading
                {
                    wallShading = 4;
                    ceilingShading = 4;
                    floorShading = 4;
                    RENDER_DISTANCE = 4;
                }

                if (kb::isKeyPressed(kb::D))
                {
                    wallShading = 1.5;
                    RENDER_DISTANCE = 16;
                }

                if (kb::isKeyPressed(kb::Q))
                {
                    std::cout << "Enter player's new X position\n";
                    std::cin >> player.position.x;
                    std::cout << "Enter player's new Y position\n";
                    std::cin >> player.position.y;
                }

                if (kb::isKeyPressed(kb::W) && cameraHeight < 0.95)
                {
                    cameraHeight += 0.1 * dt;
                    keyboardPressed = true;
                }
                else
                {
                    keyboardPressed = false;
                }

                if (kb::isKeyPressed(kb::S) && cameraHeight > 0.1)
                    cameraHeight -= 0.1 * dt;

                if (kb::isKeyPressed(kb::L))
                {
                    std::cout << "Enter level ID to load \n";
                    std::cin >> currentLevel.ID;

                    levelChanged = true;
                }
            }

            if (kb::isKeyPressed(kb::LShift))
            {
                player.playerMoveSpeed = 3.5;
                currentLevel.footsteps.setPitch(1.4);
            }
            else if (player.playerMoveSpeed = 4.0)
            {
                player.playerMoveSpeed = 2;
                currentLevel.footsteps.setPitch(0.9);
            }

            if (kb::isKeyPressed(kb::Space))
            {
                cameraHeight = 0.8;
                //keyboardPressed = true;
            }

            if (kb::isKeyPressed(kb::E))
            {
                if (currentLevel.ID == 0 && (int)player.position.x == 2 && (int)player.position.y == 1)
                {
                    currentLevel.ID = 6;
                    PlaySFX(0, soundEffect);

                    levelChanged = true;
                }

                if (currentLevel.ID == 0 && (((int)player.position.x == 1 && (int)player.position.y == 99)
                    || ((int)player.position.x == 126 && (int)player.position.y == 17)))
                {
                    currentLevel.ID = 1;
                    //playSFX(1, soundEffect);
                    levelChanged = true;
                }

                if (currentLevel.ID == 1 && (int)player.position.x == 124 && (int)player.position.y == 123 && coordinatesRecentlyChanged == false)
                {
                    PlaySFX(0, soundEffect);
                    player.position.y = 126.5;
                    coordinatesRecentlyChanged = true;
                    sf::sleep(sf::milliseconds(500));
                }
                if (currentLevel.ID == 1 && (int)player.position.x == 124 && (int)player.position.y == 126 && coordinatesRecentlyChanged == false)
                {
                    PlaySFX(0, soundEffect);
                    player.position.y = 123;
                    coordinatesRecentlyChanged = true;
                    sf::sleep(sf::milliseconds(500));
                }

                if (currentLevel.ID == 1 && (int)player.position.x == 1 && (int)player.position.y == 126)
                {
                    currentLevel.ID = 2;
                    PlaySFX(0, soundEffect);

                    levelChanged = true;
                }

            }

            if (moveForward != 0.0f)
            {
                if (footstepsPlaying == false)
                {
                    currentLevel.footsteps.play();
                    currentLevel.footsteps.setLoop(true);
                    footstepsPlaying = true;
                }


                sf::Vector2f moveVec = player.direction * player.playerMoveSpeed * moveForward * dt;

                if (player.CanMove(sf::Vector2f(player.position.x + moveVec.x, player.position.y), size)) {
                    player.position.x += moveVec.x;
                }
                if (player.CanMove(sf::Vector2f(player.position.x, player.position.y + moveVec.y), size)) {
                    player.position.y += moveVec.y;
                }
            }

            // rotating rightwards or leftwards(1.0 or -1.0)
            float rotateDirection = 0.0;

            // get input
            if (kb::isKeyPressed(kb::Left))
            {
                rotateDirection = -1.0;
            }
            else if (kb::isKeyPressed(kb::Right))
            {
                rotateDirection = 1.0;
            }

            if (kb::isKeyPressed(kb::Escape))
            {
                gameState = PAUSE;

            }

            // handle rotation
            if (rotateDirection != 0.0)
            {
                float rotation = player.playerRotateSpeed * rotateDirection * dt;
                player.direction = player.Rotate(player.direction, rotation);
                player.plane = player.Rotate(player.plane, rotation);
            }

            coordinatesRecentlyChanged = false; 
        }

    }
}

float square_root(float x)
{
    int i = *(int*)&x; // get bits for floating value
    i = (i >> 1); // gives initial guess y0
    x = *(float*)&i; // convert bits back to float

    return x;
}


void Raycasting(sf::RenderWindow& window, sf::RenderStates state, sf::VertexArray& lines)
{
    window.clear();

    // loop through vertical screen lines, draw a line of wall for each
    for (int x = 0; x < screenWidth; x++)
    {

        char tile = '.'; // tile type that got hit
        bool horizontal; // did we hit a horizontal side? Otherwise it's vertical

        float perpWallDist = 0.0; // wall distance, projected on camera direction

        int heightTile = 1;
        int groundHeight = 1;
        int wallHeight; // height of wall to draw on the screen at each distance
        int shading = 1.5;
        int lightSourcePosX, lightSourcePosY;
        int distance;

        int ceilingPixel = 0; // position of ceiling pixel on the screen
        int groundPixel = screenHeight; // position of ground pixel on the screen
        int wallTextureNum; // number of texture from the texture file

        sf::Vector2i step; // what direction to step in (+1 or -1 for each dimension)
        sf::Vector2f sideDist; // distance from current position to next gridline, for x and y separately

        // ray to emit
        float cameraX = 2 * x / (float)screenWidth - 1.0; // x in camera space (between -1 and +1)
        sf::Vector2f rayPos = player.position;
        sf::Vector2f rayDir = player.direction + player.plane * cameraX;
        sf::Vector2i mapPos(rayPos); // which box of the map we're in

        // calculate distance traversed between each grid line for x and y based on direction
        sf::Vector2f deltaDist(
            sqrt(1.0 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x)),
            sqrt(1.0 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y))
        );

        // calculate step and initial sideDist
        if (rayDir.x < 0.0) {
            step.x = -1;
            sideDist.x = (rayPos.x - mapPos.x) * deltaDist.x;
        }
        else {
            step.x = 1;
            sideDist.x = (mapPos.x + 1.0f - rayPos.x) * deltaDist.x;
        }
        if (rayDir.y < 0.0f) {
            step.y = -1;
            sideDist.y = (rayPos.y - mapPos.y) * deltaDist.y;
        }
        else {
            step.y = 1;
            sideDist.y = (mapPos.y + 1.0f - rayPos.y) * deltaDist.y;
        }

        // cast the ray until we hit a wall, meanwhile draw floors
        while (tile == '.' || tile == ';' || tile == ',')
        {
            float wall_x;

            currentLevel.color = ((mapPos.x % 2 == 0 && mapPos.y % 2 == 0) || (mapPos.x % 2 == 1 && mapPos.y % 2 == 1)) ? currentLevel.color1 : currentLevel.color2;

            /*if (tile == ',')
            {
                currentLevel.color = sf::Color(140, 140, 140);
                currentLevel.floorColor = sf::Color(140, 140, 140);
            }
            else
                currentLevel.floorColor = currentLevel.color1;*/

            if (sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                mapPos.x += step.x;
                horizontal = true;
                perpWallDist = (mapPos.x - rayPos.x + (1 - step.x) / 2) / rayDir.x;
                wall_x = rayPos.y + perpWallDist * rayDir.y;
            }
            else
            {
                sideDist.y += deltaDist.y;
                mapPos.y += step.y;
                horizontal = false;
                perpWallDist = (mapPos.y - rayPos.y + (1 - step.y) / 2) / rayDir.y;
                wall_x = rayPos.x + perpWallDist * rayDir.x;
            }

            // Check if the wall is beyond the maximum rendering distance
            if (perpWallDist > RENDER_DISTANCE)
                break;


            wallTextureNum = (int)level0_wallTypes.find(tile)->second;

            sf::Vector2i ceilingTextureCoords(
                wallTextureNum * texture_wall_size % texture_size,
                wallTextureNum * texture_wall_size / texture_size * texture_wall_size
            );

            wallHeight = (screenHeight / perpWallDist);

            int tex_x = int(wall_x * float(texture_wall_size));
            // ceilingTextureCoords.x += tex_x;

            float ceilingTextureX = (wall_x - floor(wall_x)) * texture_wall_size;

            // add floor

           // lines.append(sf::Vertex(sf::Vector2f((float)x, (float)groundPixel), floorColor, sf::Vector2f(385, 129)));
            lines.append(sf::Vertex(
                sf::Vector2f((float)x, (float)groundPixel),
                currentLevel.floorColor,
                sf::Vector2f((float)(ceilingTextureCoords.x + ceilingTextureX), (float)ceilingTextureCoords.y)));

            groundPixel = int(wallHeight * cameraHeight + screenHeight * 0.5);


            lines.append(sf::Vertex(
                sf::Vector2f((float)x, (float)groundPixel),
                currentLevel.floorColor,
                sf::Vector2f((float)(ceilingTextureCoords.x + ceilingTextureX), (float)(ceilingTextureCoords.y + texture_wall_size - 1))));

            //lines.append(sf::Vertex(sf::Vector2f((float)x, (float)groundPixel), floorColor, sf::Vector2f(385, 129)));

            // add ceiling         

            //lines.append(sf::Vertex(sf::Vector2f((float)x, (float)ceilingPixel), color_c, sf::Vector2f((float)ceilingTextureCoords.x, (float)(ceilingTextureCoords.y + texture_wall_size - 1))));
            
            sf::Color color_c = sf::Color::Transparent;

            if (tile == '$' && currentLevel.ID == 1)
            {
                ceilingPixel = int(-heightTile * wallHeight * (1.0 - cameraHeight) + screenHeight * 0.5);


                lines.append(sf::Vertex(sf::Vector2f((float)x, (float)groundPixel * 1.2), color_c, sf::Vector2f(385, 129))); // also adds floor reflexion
            }
            else {
                lines.append(sf::Vertex(
                    sf::Vector2f((float)x, (float)ceilingPixel),
                    currentLevel.color,
                    sf::Vector2f((float)(ceilingTextureCoords.x + ceilingTextureX), (float)ceilingTextureCoords.y)));
            }

            tile = GetTile(mapPos.x, mapPos.y, currentLevel.ID);
            heightTile = GetHeight(mapPos.x, mapPos.y, currentLevel.ID);
            ceilingPixel = int((-wallHeight * heightTile) * (1.0 - cameraHeight) + screenHeight * 0.5);

            //lines.append(sf::Vertex(sf::Vector2f((float)x, (float)ceilingPixel), color_c, sf::Vector2f(0, 0)));

            lines.append(sf::Vertex(
                sf::Vector2f((float)x, (float)ceilingPixel),
                currentLevel.color,
                sf::Vector2f((float)(ceilingTextureCoords.x + ceilingTextureX), (float)(ceilingTextureCoords.y + texture_wall_size - 1))));

            // change color and find tile type

            currentLevel.color = (currentLevel.color == currentLevel.color1) ? currentLevel.color2 : currentLevel.color1;

        }

        if (perpWallDist < RENDER_DISTANCE)
        {
            // calculate lowest and highest pixel to fill in current line                  
            int drawStart = ceilingPixel;
            int drawEnd = groundPixel;

            // get position of the wall texture in the full texture
            switch (currentLevel.ID)
            {
            case 0:
                wallTextureNum = (int)level0_wallTypes.find(tile)->second;
                break;
            case 1:
                wallTextureNum = (int)level0_wallTypes.find(tile)->second;
                if (tile == '$')
                {
                    wallShading = 1;
                    lightSourcePosX = mapPos.x;
                    lightSourcePosY = mapPos.y;
                }
                else
                {
                    wallShading = 1.5;
                         distance = (mapPos.x + mapPos.y) - (lightSourcePosX + lightSourcePosY);

                        if (distance >= 1 && distance <= 2)
                        {
                            wallShading = 1.2;
                            shading = 1.2;
                        }
                        else if (distance > 2 && distance <= 3)
                        {
                            wallShading = 1.3;
                            shading = 1.3;
                        }
                        else if (distance > 3 && distance <= 4)
                        {
                            wallShading = 1.4;
                            shading = 1.4;
                        }
                        else
                        {
                            wallShading = 1.5;
                            shading = 1.5;
                        }
                }
                break;
            case 2:
                wallTextureNum = (int)level0_wallTypes.find(tile)->second;
                break;
            case 3:
                wallTextureNum = (int)wallTypes.find(tile)->second;
                break;
            case 4:
                wallTextureNum = (int)levelRun_wallTypes.find(tile)->second;
                break;
            case 5:
                wallTextureNum = (int)wallTypes.find(tile)->second;
                break;
            case 6:
                wallTextureNum = (int)level0_wallTypes.find(tile)->second;
                break;
            default:
                break;
            }

            sf::Vector2i texture_coords(
                wallTextureNum * texture_wall_size % texture_size,
                wallTextureNum * texture_wall_size / texture_size * texture_wall_size
            );

            // calculate where the wall was hit
            float wall_x;
            if (horizontal)
            {
                wall_x = rayPos.y + perpWallDist * rayDir.y;

                // currentLevel.color.r /= 4;
                // currentLevel.color.g /= 4;
                // currentLevel.color.b /= 4;
            }
            else
            {
                wall_x = rayPos.x + perpWallDist * rayDir.x;
            }
            wall_x -= floor(wall_x);
            // get x coordinate on the wall texture
            int tex_x = int(wall_x * float(texture_wall_size));

            // flip texture if we see it on the other side of us, this prevents a mirrored effect for the texture
            if ((horizontal && rayDir.x <= 0) || (!horizontal && rayDir.y >= 0))
            {
                tex_x = texture_wall_size - tex_x - 1;
            }

            texture_coords.x += tex_x;

            // illusion of shading by making horizontal walls darker
            currentLevel.color = sf::Color::White;

            currentLevel.color.r /= wallShading;
            currentLevel.color.g /= wallShading;
            currentLevel.color.b /= wallShading;

            // add line to vertex buffer
            lines.append(sf::Vertex(
                sf::Vector2f((float)x, (float)drawStart),
                currentLevel.color,
                sf::Vector2f((float)texture_coords.x, (float)texture_coords.y + 1)
            ));
            lines.append(sf::Vertex(
                sf::Vector2f((float)x, (float)drawEnd),
                currentLevel.color,
                sf::Vector2f((float)texture_coords.x, (float)(texture_coords.y + texture_wall_size - 1))
            ));

        }
    }

    window.draw(lines, state);
}

int main()
{
    sf::Font font;
    sf::Vector2f size(player.playerSize, player.playerSize); // player collision box width and height
    sf::RenderStates state;

    // lines used to draw walls and floors on the screen
    sf::VertexArray lines(sf::Lines, 18 * screenWidth);

    sf::Text fpsText("", font, 50); // text object for FPS counter
    sf::Clock clock; // timer
    int64_t frame_time_micro = 0; // time needed to draw frames in microseconds

    if (!font.loadFromFile("Data/Fonts/arial.ttf")) //loads font
    {
        std::cout << "Cannot open font arial.ttf!\n";
        return 1;
    }

    LoadSaveFile();
    LoadUserSettingsData(); 

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Backrooms"); // create window after loading everything up
    window.setSize(sf::Vector2u(screenWidth, screenHeight));

    //window.setFramerateLimit(60);
    sf::View view = window.getDefaultView();

    UIConstruct(font);

    while (window.isOpen())
    {
        // handle SFML events
        sf::Event event;

        if (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::LostFocus:
                hasFocus = false;
                break;
            case sf::Event::GainedFocus:
                hasFocus = true;
                break;
            case sf::Event::Resized:
                // update the view to the new size of the window
                view.setSize({
                                        static_cast<float>(event.size.width),
                                        static_cast<float>(event.size.height)
                    });
                window.setView(view);
                break;
            default:
                break;
            }
        }

        switch (gameState)
        {
        case MAIN_MENU:
        {
            MainMenu(window, state, font);

            break;
        }
        case IN_GAME:
        {
            float dt = clock.restart().asSeconds();
            sf::Sprite sfSprite;

            UpdateFPS(fpsText, dt, frame_time_micro);

            if (levelChanged == true)
            {
                currentLevel.AmbientSFX.stop();
                currentLevel.AmbientSFX2.stop();
                currentLevel.machineSFX.stop();
                free(currentLevel.map);
                currentLevel.loadLevel(window, state);
                levelChanged = false;
            }

            KeyboardInput(hasFocus, player, size, dt);
            //printf("X: %f Y: %f player dir : %f", player.position.x, player.position.y, player.direction);
            //system("cls");

            lines.resize(0);

            if (cameraHeight > 0.67 && !keyboardPressed)
                cameraHeight -= 0.1 * GRAVITY_ACCELERATION * dt;

            if ((int)player.position.y == 245 && currentLevel.ID == 6)
                player.position.y = 200;

            if (currentLevel.ID == 2)
            {
                int distanceX = level_2.machinePosX - player.position.x;
                int distanceY = level_2.machinePosY - player.position.y;
                int volume;

                if(distanceX < 5 || distanceY < 5)
                    volume = (distanceX + distanceY) * (- 1);
                else if (distanceX + distanceY > 100)
                    volume = 100;
                else
                    volume = distanceX + distanceY;

                currentLevel.machineSFX.setVolume(100 - volume);
            }

            Raycasting(window, state, lines);

            //window.draw(sfSprite);
            window.draw(fpsText);
            frame_time_micro += clock.getElapsedTime().asMicroseconds();

            break;
        }
        case PAUSE:
        {
            PauseMenu(window,font, event, view);

            break;
        }
        case GAME_OVER:
        {
            break;
        }
        default:
            break;
        }

        window.display();
    }


    return 0;
}
