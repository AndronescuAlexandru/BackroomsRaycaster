// Version : 0.3.0

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <algorithm> 
#include <SFML/Audio.hpp>
#include <Windows.h>

#include "Maps.h" // includes data from all maps
#include "Player.h" // includes the player class and the SFML/Graphics.hpp header file
#include "Textures.h" // includes all texture files

//#include <SFML/Graphics.hpp>

#define PI 3.1459
#define FOV 90
#define MAIN_MENU 0
#define IN_GAME 1
#define GAME_OVER 2

float wallShading = 1.5;
float ceilingShading = 1.5;
float floorShading = 1.5;
short gameState = 0;
short RENDER_DISTANCE = 8;

// map size

const short MAP_WIDTH = 64;
const short MAP_HEIGHT = 64;

short currentLevel = 0; // saves the current level number where the player will be

// screen resolution

int screenWidth = 800;
int screenHeight = 600;

float cameraHeight = 0.66; // height of player camera(1.0 is ceiling, 0.0 is floor)

const int texture_size = 512; // size(width and height) of texture that will hold all wall textures
const int texture_wall_size = 128; // size(width and height) of each wall type in the full texture

const float fps_refresh_time = 0.1; // time between FPS text refresh. FPS is smoothed out over this time



char getTile(int x, int y, short level)
{
    switch (level)
    {
    case 0:
        return level0[y * MAP_WIDTH + x];
    case 1:
        return level1[y * MAP_WIDTH + x];
    case 2:
        return level2[y * MAP_WIDTH + x];
    case 3:
        return level3[y * MAP_WIDTH + x];
    case 4:
        return levelRun[y * MAP_WIDTH + x];
    case 5:
        return levelRun[y * MAP_WIDTH + x];
    default:
        return level0[y * MAP_WIDTH + x];
        break;
    }
}

float getHeight(int x, int y, short level)
{
    switch (level)
    {
    case 0:
        return level0HeightMap[y * MAP_WIDTH + x];
    case 1:
        return level1HeightMap[y * MAP_WIDTH + x];
    case 2:
        return level2HeightMap[y * MAP_WIDTH + x];
    case 4:
        return levelRunHeightMap[y * MAP_WIDTH + x];
    case 5:
        return level5HeightMap[y * MAP_WIDTH + x];
    default:
        return level0HeightMap[y * MAP_WIDTH + x];
        break;
    }
}

float getGroundHeight(int x, int y)
{
    return groundHeightMap[y * MAP_WIDTH + x];
}

// checks worldMap for errors
// returns: true on success, false on errors found
bool mapCheck() 
{
    // check size
    int mapSize = sizeof(level0) - 1; // - 1 because sizeof also counts the final NULL character

    if (mapSize != MAP_WIDTH * MAP_HEIGHT)
    {
        std::cout << "Map size " << mapSize << " is not mapWidth * mapHeight " << MAP_WIDTH * MAP_HEIGHT <<"\n";
        return false;
    }

    return true;
}

// check if a rectangular can move to given position without colliding with walls or
// being outside of the map
// position is considered the middle of the rectangle

bool canMove(sf::Vector2f position, sf::Vector2f size) 
{
    // create the corners of the rectangle
    sf::Vector2i upper_left(position - size / 2.0f);
    sf::Vector2i lower_right(position + size / 2.0f);

    if (upper_left.x < 0 || upper_left.y < 0 || lower_right.x >= MAP_WIDTH || lower_right.y >= MAP_HEIGHT)
    {
        return false; // out of map bounds
    }

    // loop through each map tile within the rectangle. The rectangle could be multiple tiles in size!
    for (int y = upper_left.y; y <= lower_right.y; y++) 
    {
        for (int x = upper_left.x; x <= lower_right.x; x++) 
        {
            if (getTile(x, y, currentLevel) != '.' && getTile(x, y, currentLevel) != ';' && getTile(x, y, currentLevel) != 'j')
            {
                return false;
            }
        }
    }
    return true;
}

// rotate a given vector with given float value in radians and return the result

sf::Vector2f rotateVec(sf::Vector2f vec, float value) 
{
    return sf::Vector2f(vec.x * std::cos(value) - vec.y * std::sin(value),vec.x * std::sin(value) + vec.y * std::cos(value));
}

void updateFPS(sf::Text &fpsText, float dt, int64_t frame_time_micro)
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

void keyboardInput(bool hasFocus, Player &player, sf::Vector2f size, float dt) // handles keyboard input
{
    if (hasFocus)
    {
        using kb = sf::Keyboard;

        // moving forward or backwards (1.0 or -1.0)
        float moveForward = 0.0;

        // get input
        if (kb::isKeyPressed(kb::Up)) 
        {
            moveForward = 1.0;
        }
        else if (kb::isKeyPressed(kb::Down)) 
        {
            moveForward = -1.0;
        }

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

        if (kb::isKeyPressed(kb::W) && cameraHeight < 0.95)
            cameraHeight += 0.1 * dt;

        if (kb::isKeyPressed(kb::S) && cameraHeight > 0.1)
            cameraHeight -= 0.1 * dt;

        if (kb::isKeyPressed(kb::LShift))
            player.playerMoveSpeed = 5.0;
        else
            player.playerMoveSpeed = 2.5;

        // handle movement
        if (moveForward != 0.0f) 
        {
            sf::Vector2f moveVec = player.direction * player.playerMoveSpeed * moveForward * dt;

            if (canMove(sf::Vector2f(player.position.x + moveVec.x, player.position.y), size)) {
                player.position.x += moveVec.x;
            }
            if (canMove(sf::Vector2f(player.position.x, player.position.y + moveVec.y), size)) {
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

        // handle rotation
        if (rotateDirection != 0.0) 
        {
            float rotation = player.playerRotateSpeed * rotateDirection * dt;
            player.direction = rotateVec(player.direction, rotation);
            player.plane = rotateVec(player.plane, rotation);
        }
    }
}

int main() 
{
    bool hasFocus = true;

    Player player;

    sf::Font font;

    sf::Texture texture;
    sf::Texture level0_Textures;
    sf::Texture ceiling;
    sf::Texture levelRun_Textures;

    sf::SoundBuffer soundBuffer;
    sf::Sound level0_AmbientSFX;

    sf::Vector2f size(player.playerSize, player.playerSize); // player collision box width and height

    sf::RenderStates state;

    // lines used to draw walls and floors on the screen
    sf::VertexArray lines(sf::Lines, 18 * screenWidth);

    sf::Text fpsText("", font, 50); // text object for FPS counter
    sf::Clock clock; // timer

    int64_t frame_time_micro = 0; // time needed to draw frames in microseconds

    std::cout << "Enter level choice\n";
    std::cin >> currentLevel;

    std::cout << "Enter draw distance\n";
    std::cin >> RENDER_DISTANCE;

    std::cout << "Enter screen width\n";
    std::cin >> screenWidth;

    std::cout << "Enter screen height\n";
    std::cin >> screenHeight;
    system("cls");
  
    if (!mapCheck()) // checks if the map is correct
    {
        std::cout << "Map is invalid\n";
        return 1;
    }

    if (!font.loadFromFile("Fonts/arial.ttf")) //loads font
    {
        std::cout << "Cannot open font arial.ttf!\n";
        return 1;
    }

    // loads files specific for each level and set render state that uses the texture
    switch (currentLevel)
    {
    case 0:

        if (!level0_Textures.loadFromFile("Textures/level_0_textures.png"))
        {
            std::cout << "Cannot open texture level_0_textures.png!\n";
            return 1;
        }

        if (!soundBuffer.loadFromFile("Data/Audio/Level0LightAmbience.mp3"))
        {
            std::cout << "Cannot open sound file Level0LightAmbience.mp3!\n";
            return 1;
        }

        state.texture = &level0_Textures;
        level0_AmbientSFX.setBuffer(soundBuffer);
        level0_AmbientSFX.play();
        level0_AmbientSFX.setLoop(true);

        wallShading = 1.1;
        //ceilingShading = 1.5;
        //floorShading = 1.5;
        break;
    case 1:
        if (!level0_Textures.loadFromFile("Textures/level_0_textures.png"))
        {
            std::cout << "Cannot open texture level_0_textures.png!\n";
            return 1;
        }

        if (!soundBuffer.loadFromFile("Data/Audio/Level0LightAmbience.mp3"))
        {
            std::cout << "Cannot open sound file Level0LightAmbience.mp3!\n";
            return 1;
        }

        state.texture = &level0_Textures;
        level0_AmbientSFX.setBuffer(soundBuffer);
        level0_AmbientSFX.setVolume(10);
        level0_AmbientSFX.play();
        level0_AmbientSFX.setLoop(true);

        wallShading = 1.5;
        ceilingShading = 1.5;
        floorShading = 1.5;
        break;
    case 2:
        if (!level0_Textures.loadFromFile("Textures/level_0_textures.png"))
        {
            std::cout << "Cannot open texture level_0_textures.png!\n";
            return 1;
        }
        state.texture = &level0_Textures;

        wallShading = 1.7;
        ceilingShading = 1.7;
        floorShading = 1.7;
        break;
    case 3:
        if (!texture.loadFromFile("Textures/textures.png"))
        {
            std::cout << "Cannot open texture textures.png!\n";
            return 1;
        }
        state.texture = &texture;
        break;
    case 4:
        if (!levelRun_Textures.loadFromFile("Textures/level_Run_textures.png"))
        {
            std::cout << "Cannot open texture level_Run_textures.png!\n";
            return 1;
        }
    case 5:
        if (!texture.loadFromFile("Textures/textures.png"))
        {
            std::cout << "Cannot open texture textures.png!\n";
            return 1;
        }
        state.texture = &texture;
        break;
    default:
        
        break;
    }  

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Backrooms"); // create window after loading everything up

    //window.setSize(sf::Vector2u(screenWidth, screenHeight));

    //window.setFramerateLimit(60);

    while (window.isOpen()) 
    {
        float dt = clock.restart().asSeconds();
        sf::Event event;

        sf::View view = window.getDefaultView();
        
        updateFPS(fpsText, dt, frame_time_micro);

        // handle SFML events

        while (window.pollEvent(event)) 
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

        keyboardInput(hasFocus, player, size, dt);

        lines.resize(0);

        // loop through vertical screen lines, draw a line of wall for each
        for (int x = 0; x < screenWidth; x++) 
        {
            char tile = '.'; // tile type that got hit
            bool horizontal; // did we hit a horizontal side? Otherwise it's vertical

            float perpWallDist = 0.0; // wall distance, projected on camera direction

            int heightTile = 1;
            int groundHeight = 1;
            int wallHeight; // height of wall to draw on the screen at each distance

            int ceilingPixel = 0; // position of ceiling pixel on the screen
            int groundPixel = screenHeight; // position of ground pixel on the screen
            int wallTextureNum; // number of texture from the texture file

            // colors for floor and ceiling tiles
            sf::Color color;
            sf::Color color1;
            sf::Color color2;

            sf::Color floorColor;

            sf::Vector2i step; // what direction to step in (+1 or -1 for each dimension)
            sf::Vector2f sideDist; // distance from current position to next gridline, for x and y separately

            // ray to emit
            float cameraX = 2 * x / (float)screenWidth - 1.0; // x in camera space (between -1 and +1)
            sf::Vector2f rayPos = player.position;
            sf::Vector2f rayDir = player.direction + player.plane * cameraX;
            sf::Vector2i mapPos(rayPos); // which box of the map we're in

            // calculate distance traversed between each grid line for x and y based on direction
            sf::Vector2f deltaDist(
                sqrt(1.0f + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x)),
                sqrt(1.0f + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y))
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
            while (tile == '.' || tile == ';')
            {
                float wall_x;

                switch (currentLevel)
                {
                case 0:
                    color1 = sf::Color(182, 179, 102);
                    color2 = sf::Color(255, 255, 255);

                    floorColor = sf::Color(178, 163, 106);
                    // ceiling color shading
                    color = ((mapPos.x % 2 == 0 && mapPos.y % 2 == 0) ||
                        (mapPos.x % 2 == 1 && mapPos.y % 2 == 1)) ? color1 : color2;
                    break;
                case 1:
                    color1 = sf::Color(154, 153, 149);
                    color2 = sf::Color(200, 200, 200); //sf::Color(129, 124, 121);
                    color.r /= ceilingShading;
                    color.g /= ceilingShading;
                    color.b /= ceilingShading;

                    floorColor = sf::Color(154, 153, 149);
                    floorColor.r /= floorShading;
                    floorColor.g /= floorShading;
                    floorColor.b /= floorShading;
                    break;
                case 2:
                    color1 = sf::Color(154, 153, 149);
                    color2 = sf::Color(200, 200, 200); //sf::Color(129, 124, 121);
                    color.r /= ceilingShading;
                    color.g /= ceilingShading;
                    color.b /= ceilingShading;

                    floorColor = sf::Color(154, 153, 149);
                    floorColor.r /= floorShading;
                    floorColor.g /= floorShading;
                    floorColor.b /= floorShading;
                    break;
                case 3:
                    color1 = sf::Color(43, 162, 198);
                    color2 = sf::Color(96, 96, 96);
                    floorColor = sf::Color(96, 96, 96);
                    break;
                case 4:
                    color1 = sf::Color(154, 153, 149);
                    color2 = sf::Color(154, 153, 149);
                    
                    floorColor = sf::Color(154, 153, 149);
                    floorColor.r /= floorShading;
                    floorColor.g /= floorShading;
                    floorColor.b /= floorShading;

                    break;
                case 5:
                    color1 = sf::Color(154, 153, 149);
                    color2 = sf::Color(154, 153, 149);

                    floorColor = sf::Color(154, 153, 149);
                    floorColor.r /= floorShading;
                    floorColor.g /= floorShading;
                    floorColor.b /= floorShading;

                    break;
                default:
                    color1 = sf::Color(43, 162, 198);
                    color2 = sf::Color(96, 96, 96);

                    floorColor = sf::Color(96, 96, 96);
                    break;
                }

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

                if (player.position.x < 1.5 && player.position.y < 2.0)
                    cameraHeight = 0.8;
                else if(cameraHeight == 0.8)
                    cameraHeight = 0.66;

                wallHeight = (screenHeight / perpWallDist);

                int tex_x = int(wall_x * float(texture_wall_size));
               // ceilingTextureCoords.x += tex_x;

                float ceilingTextureX = (wall_x - floor(wall_x)) * texture_wall_size;

                // add floor

                groundHeight = getGroundHeight(mapPos.x, mapPos.y);
                //floorColor = sf::Color::White;

               // lines.append(sf::Vertex(sf::Vector2f((float)x, (float)groundPixel), floorColor, sf::Vector2f(385, 129)));
                lines.append(sf::Vertex(
                    sf::Vector2f((float)x, (float)groundPixel),
                    floorColor,
                    sf::Vector2f((float)(ceilingTextureCoords.x + ceilingTextureX), (float)ceilingTextureCoords.y)));

                groundPixel = int(wallHeight * groundHeight * cameraHeight + screenHeight * 0.5);


                lines.append(sf::Vertex(
                    sf::Vector2f((float)x, (float)groundPixel),
                    floorColor,
                    sf::Vector2f((float)(ceilingTextureCoords.x + ceilingTextureX), (float)(ceilingTextureCoords.y + texture_wall_size - 1))));

                //lines.append(sf::Vertex(sf::Vector2f((float)x, (float)groundPixel), floorColor, sf::Vector2f(385, 129)));

                // add ceiling         
                
                //lines.append(sf::Vertex(sf::Vector2f((float)x, (float)ceilingPixel), color_c, sf::Vector2f((float)ceilingTextureCoords.x, (float)(ceilingTextureCoords.y + texture_wall_size - 1))));
                lines.append(sf::Vertex(
                    sf::Vector2f((float)x, (float)ceilingPixel),
                    color,
                    sf::Vector2f((float)(ceilingTextureCoords.x + ceilingTextureX), (float)ceilingTextureCoords.y)));
               
                tile = getTile(mapPos.x, mapPos.y, currentLevel);
                heightTile = getHeight(mapPos.x, mapPos.y, currentLevel);
                ceilingPixel = int((-wallHeight * heightTile) * (1.0 - cameraHeight) + screenHeight * 0.5);

                //lines.append(sf::Vertex(sf::Vector2f((float)x, (float)ceilingPixel), color_c, sf::Vector2f(0, 0)));

                lines.append(sf::Vertex(
                    sf::Vector2f((float)x, (float)ceilingPixel),
                    color,
                    sf::Vector2f((float)(ceilingTextureCoords.x + ceilingTextureX), (float)(ceilingTextureCoords.y + texture_wall_size - 1))));

                /*if (tile == 'N')
                {
                    ceilingPixel = int(-wallHeight2 * (1.0 - cameraHeight) + screenHeight * 0.5);
                    color_c = sf::Color::Transparent;

                    lines.append(sf::Vertex(sf::Vector2f((float)x, (float)groundPixel * 1.2), color_c,sf::Vector2f(385, 129))); // also adds floor reflexion
                }
                else
                    if (tile == '!')
                    {
                        ceilingPixel = int(-wallHeight * (1.0 - cameraHeight) + screenHeight * 0.5);

                        lines.append(sf::Vertex(sf::Vector2f((float)x, (float)ceilingPixel), color_c,sf::Vector2f(385, 129))); // also adds floor reflexion
                    }
                    else
                        if (tile == 'M')
                        {
                            ceilingPixel = int(-wallHeight3 * (1.0 - cameraHeight) + screenHeight * 0.5);
                            lines.append(sf::Vertex(sf::Vector2f((float)x, (float)ceilingPixel), color_c, sf::Vector2f(385, 129)));
                        }
                        else
                        {
                            ceilingPixel = int(-wallHeight * (1.0 - cameraHeight) + screenHeight * 0.5);
                            lines.append(sf::Vertex(sf::Vector2f((float)x, (float)ceilingPixel), color_c,sf::Vector2f(385, 129)));
                        }*/

                // change color and find tile type

                color = (color == color1) ? color2 : color1;

            }


            if (perpWallDist < RENDER_DISTANCE)
            {
                // calculate lowest and highest pixel to fill in current line

                int drawStart = ceilingPixel;
                int drawEnd = groundPixel;
                
                // get position of the wall texture in the full texture
                switch (currentLevel)
                {
                case 0:
                    wallTextureNum = (int)level0_wallTypes.find(tile)->second;
                    break;
                case 1:
                    wallTextureNum = (int)level0_wallTypes.find(tile)->second;
                    if (tile == '$')
                        wallShading = 1;
                    else
                        wallShading = 1.5;
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
                default:
                    break;
                }

                sf::Vector2i texture_coords(
                    wallTextureNum* texture_wall_size% texture_size,
                    wallTextureNum* texture_wall_size / texture_size * texture_wall_size
                );

                // calculate where the wall was hit
                float wall_x;
                if (horizontal) 
                {
                    wall_x = rayPos.y + perpWallDist * rayDir.y;
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
                color = sf::Color::White;

                switch (currentLevel)
                {
                case 0:
                    color.r /= 1;
                    color.g /= 1;
                    color.b /= 1;
                    break;
                case 1:
                    color.r /= wallShading;
                    color.g /= wallShading;
                    color.b /= wallShading;
                    break;
                case 2:
                    color.r /= wallShading;
                    color.g /= wallShading;
                    color.b /= wallShading;
                    break;
                default:
                    break;
                }

                /*if (horizontal)
                {
                    color.r /= 2;
                    color.g /= 2;
                    color.b /= 2;
                }*/


                // add line to vertex buffer
                    lines.append(sf::Vertex(
                        sf::Vector2f((float)x, (float)drawStart),
                        color,
                        sf::Vector2f((float)texture_coords.x, (float)texture_coords.y + 1)
                    ));
                    lines.append(sf::Vertex(
                        sf::Vector2f((float)x, (float)drawEnd),
                        color,
                        sf::Vector2f((float)texture_coords.x, (float)(texture_coords.y + texture_wall_size - 1))
                    ));
            }
        }

        window.clear();
        window.draw(lines, state);
        window.draw(fpsText);
        frame_time_micro += clock.getElapsedTime().asMicroseconds();
        window.display();
        
    }

    return EXIT_SUCCESS;
}
