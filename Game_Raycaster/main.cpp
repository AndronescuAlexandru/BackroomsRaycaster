// Version : 0.6.2

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <algorithm>
#include <SFML/System/Time.hpp>
#include <Windows.h>

#include "Maps.h"
#include "Player.h"
#include "Textures.h"

#define VERSION_ID "Version 0.6.2"
#define PI 3.1459
#define GRAVITY_ACCELERATION 9.81
#define FOV 90
#define MAIN_MENU 0
#define IN_GAME 1
#define GAME_OVER 2

struct Level_2 level_2;
struct Level_3 level_3;
struct Level_Run level_Run;
struct Level_5 level_5;
struct Level_0_0_1 level_0_0_1;

Player player;
CurrentLevel currentLevel;

sf::SoundBuffer soundEffectsBuffer;
sf::Sound soundEffect;

bool keyboardPressed = false;
bool levelChanged = false;
bool playerIsMoving = false;
bool footstepsPlaying = false;
bool debugMode = false;

short menuChoice = 0;

float wallShading = 1.5;
float ceilingShading = 1.5;
float floorShading = 1.5;
short gameState = 0;
short RENDER_DISTANCE = 8;

// screen resolution

int screenWidth = 800;
int screenHeight = 600;

float cameraHeight = 0.66; // height of player camera(1.0 is ceiling, 0.0 is floor)

const int texture_size = 512; // size(width and height) of texture that will hold all wall textures
const int texture_wall_size = 128; // size(width and height) of each wall type in the full texture

const float fps_refresh_time = 0.1; // time between FPS text refresh. FPS is smoothed out over this time

char getTile(int x, int y, short level)
{
    return currentLevel.map[y * currentLevel.MAP_WIDTH + x];
}

float getHeight(int x, int y, short level)
{
    return currentLevel.maxWallHeight;
}

// check if a rectangular can move to given position without colliding with walls or
// being outside of the map
// position is considered the middle of the rectangle

bool canMove(sf::Vector2f position, sf::Vector2f size)
{
    // create the corners of the rectangle
    sf::Vector2i upper_left(position - size / 2.0f);
    sf::Vector2i lower_right(position + size / 2.0f);

    if (upper_left.x < 0 || upper_left.y < 0 || lower_right.x >= currentLevel.MAP_WIDTH || lower_right.y >= currentLevel.MAP_HEIGHT)
    {
        return false; // out of map bounds
    }

    // loop through each map tile within the rectangle. The rectangle could be multiple tiles in size!
    for (int y = upper_left.y; y <= lower_right.y; y++)
    {
        for (int x = upper_left.x; x <= lower_right.x; x++)
        {
            if (getTile(x, y, currentLevel.ID) != '.' && getTile(x, y, currentLevel.ID) != ';' && getTile(x, y, currentLevel.ID) != 'j')
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
    return sf::Vector2f(vec.x * std::cos(value) - vec.y * std::sin(value), vec.x * std::sin(value) + vec.y * std::cos(value));
}

void updateFPS(sf::Text& fpsText, float dt, int64_t frame_time_micro)
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

void playSFX(short type, sf::Sound& soundEffect)
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

void keyboardInput(bool hasFocus, Player& player, sf::Vector2f size, float dt) // handles keyboard input
{
    bool coordinatesRecentlyChanged = false;
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
                playSFX(0, soundEffect);

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
                playSFX(0, soundEffect);
                player.position.y = 126.5;
                coordinatesRecentlyChanged = true;
                sf::sleep(sf::milliseconds(500));
            }
            if (currentLevel.ID == 1 && (int)player.position.x == 124 && (int)player.position.y == 126 && coordinatesRecentlyChanged == false)
            {
                playSFX(0, soundEffect);
                player.position.y = 123;
                coordinatesRecentlyChanged = true;
                sf::sleep(sf::milliseconds(500));
            }

            if (currentLevel.ID == 1 && (int)player.position.x == 1 && (int)player.position.y == 126)
            {
                currentLevel.ID = 2;
                playSFX(0, soundEffect);

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

        if (kb::isKeyPressed(kb::Escape))
        {
            gameState = MAIN_MENU;
        }

        // handle rotation
        if (rotateDirection != 0.0)
        {
            float rotation = player.playerRotateSpeed * rotateDirection * dt;
            player.direction = rotateVec(player.direction, rotation);
            player.plane = rotateVec(player.plane, rotation);
        }

        coordinatesRecentlyChanged = false;
    }
}

void optionsMenu(sf::RenderWindow& window, sf::Font font, sf::Sprite& menuBackgroundSprite)
{
    short choice = 0;

    sf::RectangleShape setRes_640_480_Button(sf::Vector2f(120, 30));
    sf::RectangleShape setRes_800_600_Button(sf::Vector2f(120, 30));
    sf::RectangleShape setRes_1024_600_Button(sf::Vector2f(120, 30));
    sf::RectangleShape setRes_1280_720_Button(sf::Vector2f(120, 30));
    sf::RectangleShape setRes_1600_900_Button(sf::Vector2f(120, 30));
    sf::RectangleShape setRes_1920_1080_Button(sf::Vector2f(120, 30));
    sf::RectangleShape decreaseDD(sf::Vector2f(30, 30));
    sf::RectangleShape increaseDD(sf::Vector2f(30, 30));

    sf::Text gameResSectionText;
    sf::Text ResText_640_480;
    sf::Text ResText_800_600;
    sf::Text ResText_1024_600;
    sf::Text ResText_1280_720;
    sf::Text ResText_1600_900;
    sf::Text ResText_1920_1080;
    sf::Text drawDistanceValue;
    sf::Text plus;
    sf::Text minus;

    // text properties

    // setting text font
    ResText_640_480.setFont(font);
    ResText_800_600.setFont(font);
    ResText_1024_600.setFont(font);
    ResText_1280_720.setFont(font);
    ResText_1600_900.setFont(font);
    ResText_1920_1080.setFont(font);
    gameResSectionText.setFont(font);
    drawDistanceValue.setFont(font);

    //setting text character size
    ResText_640_480.setCharacterSize(20);
    ResText_800_600.setCharacterSize(20);
    ResText_1024_600.setCharacterSize(20);
    ResText_1280_720.setCharacterSize(20);
    ResText_1600_900.setCharacterSize(20);
    ResText_1920_1080.setCharacterSize(20);
    gameResSectionText.setCharacterSize(20);
    drawDistanceValue.setCharacterSize(20);

    //setting text string
    ResText_640_480.setString("640x480");
    ResText_800_600.setString("800x600");
    ResText_1024_600.setString("1024x600");
    ResText_1280_720.setString("1280x720");
    ResText_1600_900.setString("1600x900");
    ResText_1920_1080.setString("1920x1080");
    gameResSectionText.setString("Screen resolution (Cureent screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
    drawDistanceValue.setString("Draw distance " + std::to_string(RENDER_DISTANCE));

    //setting text color
    ResText_640_480.setFillColor(sf::Color::White);
    ResText_800_600.setFillColor(sf::Color::White);
    ResText_1024_600.setFillColor(sf::Color::White);
    ResText_1280_720.setFillColor(sf::Color::White);
    ResText_1600_900.setFillColor(sf::Color::White);
    ResText_1920_1080.setFillColor(sf::Color::White);
    gameResSectionText.setFillColor(sf::Color::White);
    drawDistanceValue.setFillColor(sf::Color::White);

    // setting text position
    gameResSectionText.setPosition(5, 5);
    ResText_640_480.setPosition(5, 30);
    ResText_800_600.setPosition(5, 65);
    ResText_1024_600.setPosition(5, 100);
    ResText_1280_720.setPosition(5, 135);
    ResText_1600_900.setPosition(5, 170);
    ResText_1920_1080.setPosition(5, 205);
    drawDistanceValue.setPosition(5, 240);

    // button properties

    // setting button color
    setRes_640_480_Button.setFillColor(sf::Color::Black);
    setRes_800_600_Button.setFillColor(sf::Color::Black);
    setRes_1024_600_Button.setFillColor(sf::Color::Black);
    setRes_1280_720_Button.setFillColor(sf::Color::Black);
    setRes_1600_900_Button.setFillColor(sf::Color::Black);
    setRes_1920_1080_Button.setFillColor(sf::Color::Black);
    decreaseDD.setFillColor(sf::Color::Black);
    increaseDD.setFillColor(sf::Color::Black);

    // setting button position
    setRes_640_480_Button.setPosition(sf::Vector2f(5, 30));
    setRes_800_600_Button.setPosition(sf::Vector2f(5,65));
    setRes_1024_600_Button.setPosition(sf::Vector2f(5,100));
    setRes_1280_720_Button.setPosition(sf::Vector2f(5,135));
    setRes_1600_900_Button.setPosition(sf::Vector2f(5,170));
    setRes_1920_1080_Button.setPosition(sf::Vector2f(5, 205));
    decreaseDD.setPosition(sf::Vector2f(5, 270));
    increaseDD.setPosition(sf::Vector2f(50, 270));

    while (true)
    {
        window.clear();

        window.draw(menuBackgroundSprite);

        window.draw(gameResSectionText);

        window.draw(setRes_640_480_Button);
        window.draw(setRes_800_600_Button);
        window.draw(setRes_1024_600_Button);
        window.draw(setRes_1280_720_Button);
        window.draw(setRes_1600_900_Button);
        window.draw(setRes_1920_1080_Button);

        window.draw(ResText_640_480);
        window.draw(ResText_800_600);
        window.draw(ResText_1024_600);
        window.draw(ResText_1280_720);
        window.draw(ResText_1600_900);
        window.draw(ResText_1920_1080);

        window.draw(drawDistanceValue);
        window.draw(decreaseDD);
        window.draw(increaseDD);

        window.display();

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && choice > 0)
            choice--;

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && choice < 6)
            choice++;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            break;

        setRes_640_480_Button.setFillColor(sf::Color::Black);
        setRes_800_600_Button.setFillColor(sf::Color::Black);
        setRes_1024_600_Button.setFillColor(sf::Color::Black);
        setRes_1280_720_Button.setFillColor(sf::Color::Black);
        setRes_1600_900_Button.setFillColor(sf::Color::Black);
        setRes_1920_1080_Button.setFillColor(sf::Color::Black);

        ResText_640_480.setFillColor(sf::Color::White);
        ResText_800_600.setFillColor(sf::Color::White);
        ResText_1024_600.setFillColor(sf::Color::White);
        ResText_1280_720.setFillColor(sf::Color::White);
        ResText_1600_900.setFillColor(sf::Color::White);
        ResText_1920_1080.setFillColor(sf::Color::White);

        decreaseDD.setFillColor(sf::Color::Black);
        increaseDD.setFillColor(sf::Color::Black);

        sf::sleep(sf::milliseconds(100));

        switch (choice)
        {
        case 0:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && screenWidth != 640 && screenHeight != 480)
            {
                screenWidth = 640;
                screenHeight = 480;
                window.close();
                window.create(sf::VideoMode(640, 480), "Backrooms");

                menuBackgroundSprite.setScale(sf::Vector2f(1.0, 0.75));
                gameResSectionText.setString("Screen resolution (Cureent screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
            }
            
            setRes_640_480_Button.setFillColor(sf::Color::White);
            ResText_640_480.setFillColor(sf::Color::Black);

            break;
        case 1:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && screenWidth != 800 && screenHeight != 600)
            {
                screenWidth = 800;
                screenHeight = 600;
                window.close();
                window.create(sf::VideoMode(800, 600), "Backrooms");

                menuBackgroundSprite.setScale(sf::Vector2f(1.25, 1));
                gameResSectionText.setString("Screen resolution (Cureent screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
            }
            
            setRes_800_600_Button.setFillColor(sf::Color::White);
            ResText_800_600.setFillColor(sf::Color::Black);

            break;
        case 2:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && screenWidth != 1024 && screenHeight != 600)
            {
                screenWidth = 1024;
                screenHeight = 600;
                window.close();
                window.create(sf::VideoMode(1024, 600), "Backrooms");

                menuBackgroundSprite.setScale(sf::Vector2f(1.75, 1));
                gameResSectionText.setString("Screen resolution (Cureent screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
            }
               
            setRes_1024_600_Button.setFillColor(sf::Color::White);
            ResText_1024_600.setFillColor(sf::Color::Black);

            break;
        case 3:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && screenWidth != 1280 && screenHeight != 720)
            {
                screenWidth = 1280;
                screenHeight = 720;
                window.close();
                window.create(sf::VideoMode(1280, 720), "Backrooms");

                menuBackgroundSprite.setScale(sf::Vector2f(1.95, 1.15));
                gameResSectionText.setString("Screen resolution (Cureent screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
            }

            setRes_1280_720_Button.setFillColor(sf::Color::White);
            ResText_1280_720.setFillColor(sf::Color::Black);

            break;
        case 4:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && screenWidth != 1600 && screenHeight != 900)
            {
                screenWidth = 1600;
                screenHeight = 900;
                window.close();
                window.create(sf::VideoMode(1600, 900), "Backrooms");

                menuBackgroundSprite.setScale(sf::Vector2f(2.45, 1.50));
                gameResSectionText.setString("Screen resolution (Cureent screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
            }

            setRes_1600_900_Button.setFillColor(sf::Color::White);
            ResText_1600_900.setFillColor(sf::Color::Black);

            break;
        case 5:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && screenWidth != 1920 && screenHeight != 1080)
            {
                screenWidth = 1920;
                screenHeight = 1080;
                window.close();
                window.create(sf::VideoMode(1920, 1080), "Backrooms");

                menuBackgroundSprite.setScale(sf::Vector2f(2.85, 1.65));
                gameResSectionText.setString("Screen resolution (Cureent screen resolution " + std::to_string(screenWidth) + 'x' + std::to_string(screenHeight) + ')');
            }
            
            setRes_1920_1080_Button.setFillColor(sf::Color::White);
            ResText_1920_1080.setFillColor(sf::Color::Black);

            break;
        case 6: 
            decreaseDD.setFillColor(sf::Color::White);
            increaseDD.setFillColor(sf::Color::White);
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && RENDER_DISTANCE > 8)
            {
                decreaseDD.setFillColor(sf::Color::Black);
                increaseDD.setFillColor(sf::Color::White);
                RENDER_DISTANCE--;

                drawDistanceValue.setString("Draw distance " + std::to_string(RENDER_DISTANCE));
            }

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && RENDER_DISTANCE < 128)
            {
                decreaseDD.setFillColor(sf::Color::White);
                increaseDD.setFillColor(sf::Color::Black);
                RENDER_DISTANCE++;

                drawDistanceValue.setString("Draw distance " + std::to_string(RENDER_DISTANCE));
            }
            break;
        case 7:

            break;
        default:
            break;
        }
    }

}

int main()
{
    bool hasFocus = true;

    sf::Font font;
    sf::Text startButtonText;
    sf::Text optionsButtonText;
    sf::Text exitButtonText;
    sf::Text versionID;

    sf::Texture mainMenuBackgroundTexture;
    sf::Sprite mainMenuBackgroundSprite;

    sf::RectangleShape startGameButton(sf::Vector2f(120, 30));
    sf::RectangleShape optionsButton(sf::Vector2f(120, 30));
    sf::RectangleShape exitGameButton(sf::Vector2f(120, 30));

    sf::Vector2f size(player.playerSize, player.playerSize); // player collision box width and height

    sf::RenderStates state;

    // lines used to draw walls and floors on the screen
    sf::VertexArray lines(sf::Lines, 18 * screenWidth);

    sf::Text fpsText("", font, 50); // text object for FPS counter
    sf::Clock clock; // timer

    int64_t frame_time_micro = 0; // time needed to draw frames in microseconds

    if (!mainMenuBackgroundTexture.loadFromFile("Data/Textures/menu_background.jpg"))
    {
        std::cout << "Cannot open image file menu_background.jpg";
        return 1;
    }

    if (!font.loadFromFile("Data/Fonts/arial.ttf")) //loads font
    {
        std::cout << "Cannot open font arial.ttf!\n";
        return 1;
    }

    if (!currentLevel.entitySoundBuffer.loadFromFile("Data/Audio/EntitySFX.mp3"))
    {
        std::cout << "Cannot open sound file EntitySFX.mp3!\n";
        return 1;
    }

    mainMenuBackgroundSprite.setTexture(mainMenuBackgroundTexture);
    mainMenuBackgroundSprite.setScale(sf::Vector2f(1.25, 1));

    startGameButton.setFillColor(sf::Color::Black);
    optionsButton.setFillColor(sf::Color::Black);
    exitGameButton.setFillColor(sf::Color::Black);
    versionID.setFillColor(sf::Color::White);

    startButtonText.setFont(font);
    optionsButtonText.setFont(font);
    exitButtonText.setFont(font);
    versionID.setFont(font);

    startButtonText.setString("Start Game");
    optionsButtonText.setString("Options");
    exitButtonText.setString("Exit Game");
    versionID.setString(VERSION_ID);

    startButtonText.setCharacterSize(20);
    optionsButtonText.setCharacterSize(20);
    exitButtonText.setCharacterSize(20);
    versionID.setCharacterSize(20);

    startButtonText.setPosition(screenWidth / 2 - 40, screenHeight / 2);
    optionsButtonText.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 35);
    exitButtonText.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 70);
    versionID.setPosition(5, screenHeight - 40);

    startGameButton.setPosition(screenWidth / 2 - 40, screenHeight / 2);
    optionsButton.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 35);
    exitGameButton.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 70);

    currentLevel.EntitySFX.setBuffer(currentLevel.entitySoundBuffer);

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Backrooms"); // create window after loading everything up

    window.setSize(sf::Vector2u(screenWidth, screenHeight));

    //window.setFramerateLimit(60);

    while (window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && menuChoice < 2)
        {
            menuChoice++;
            sf::sleep(sf::milliseconds(100));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && menuChoice > 0)
        {
            menuChoice--;
            sf::sleep(sf::milliseconds(100));
        }
        window.clear();

        // handle SFML events
        sf::Event event;
        sf::View view = window.getDefaultView();

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

        switch (gameState)
        {
        case MAIN_MENU:
        {
            currentLevel.AmbientSFX.stop();

            window.draw(mainMenuBackgroundSprite);
            window.draw(startGameButton);
            window.draw(optionsButton);
            window.draw(exitGameButton);
            window.draw(startButtonText);
            window.draw(optionsButtonText);
            window.draw(exitButtonText);
            window.draw(versionID);

            startGameButton.setFillColor(sf::Color::Black);
            optionsButton.setFillColor(sf::Color::Black);
            exitGameButton.setFillColor(sf::Color::Black);

            startButtonText.setFillColor(sf::Color::White);
            optionsButtonText.setFillColor(sf::Color::White);
            exitButtonText.setFillColor(sf::Color::White);

            switch (menuChoice)
            {
            case 0:
                startGameButton.setFillColor(sf::Color::White);
                startButtonText.setFillColor(sf::Color::Black);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    if (debugMode == true)
                    {
                        std::cout << "Enter level choice\n";
                        std::cin >> currentLevel.ID;
                    }

                    gameState = IN_GAME;
                    currentLevel.loadLevel(window, state);
                }

                break;
            case 1:
                optionsButton.setFillColor(sf::Color::White);
                optionsButtonText.setFillColor(sf::Color::Black);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    optionsMenu(window, font, mainMenuBackgroundSprite);

                    startButtonText.setPosition(screenWidth / 2 - 40, screenHeight / 2);
                    optionsButtonText.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 35);
                    exitButtonText.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 70);
                    versionID.setPosition(5, screenHeight - 40);

                    startGameButton.setPosition(screenWidth / 2 - 40, screenHeight / 2);
                    optionsButton.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 35);
                    exitGameButton.setPosition(screenWidth / 2 - 40, screenHeight / 2 + 70);
                }

                break;
            case 2:
                exitGameButton.setFillColor(sf::Color::White);
                exitButtonText.setFillColor(sf::Color::Black);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                    exit(1);

                break;
            default:
                break;
            }
            sf::sleep(sf::milliseconds(50));

            break;
        }
        case IN_GAME:
        {
            float dt = clock.restart().asSeconds();

            updateFPS(fpsText, dt, frame_time_micro);

            if (levelChanged == true)
            {
                currentLevel.AmbientSFX.stop();
                free(currentLevel.map);
                currentLevel.loadLevel(window, state);
                levelChanged = false;
            }

            keyboardInput(hasFocus, player, size, dt);


            //printf("X: %f Y: %f player dir : %f", player.position.x, player.position.y, player.direction);
            //system("cls");

            lines.resize(0);

            if (cameraHeight > 0.67 && !keyboardPressed)
                cameraHeight -= 0.1 * GRAVITY_ACCELERATION * dt;

            if ((int)player.position.y == 245 && currentLevel.ID == 6)
                player.position.y = 200;

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

                    currentLevel.color = ((mapPos.x % 2 == 0 && mapPos.y % 2 == 0) || (mapPos.x % 2 == 1 && mapPos.y % 2 == 1)) ? currentLevel.color1 : currentLevel.color2;

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

                    //groundHeight = getGroundHeight(mapPos.x, mapPos.y);
                    //floorColor = sf::Color::White;

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
                    lines.append(sf::Vertex(
                        sf::Vector2f((float)x, (float)ceilingPixel),
                        currentLevel.color,
                        sf::Vector2f((float)(ceilingTextureCoords.x + ceilingTextureX), (float)ceilingTextureCoords.y)));

                    tile = getTile(mapPos.x, mapPos.y, currentLevel.ID);
                    heightTile = getHeight(mapPos.x, mapPos.y, currentLevel.ID);
                    ceilingPixel = int((-wallHeight * heightTile) * (1.0 - cameraHeight) + screenHeight * 0.5);

                    //lines.append(sf::Vertex(sf::Vector2f((float)x, (float)ceilingPixel), color_c, sf::Vector2f(0, 0)));

                    lines.append(sf::Vertex(
                        sf::Vector2f((float)x, (float)ceilingPixel),
                        currentLevel.color,
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

                    /*if (horizontal)
                    {
                        color.r /= 2;
                        color.g /= 2;
                        color.b /= 2;
                    }*/


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
            window.draw(fpsText);
            frame_time_micro += clock.getElapsedTime().asMicroseconds();

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
