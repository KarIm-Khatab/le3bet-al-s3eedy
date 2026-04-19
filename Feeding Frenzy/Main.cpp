#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace sf;

struct Score{
int score = 0, bouns = 1, smallFscore = 10, medFscore = 20, bigFscore = 50,
taileBscore = 100, starscore = 60, prealscore = 50,
scale = 0, lives = 3, trials = 3, speedP = 800, speedE = 100, livelnum = 1,
currentFrenzy = 0, i = 0; //for counting frenzy word

int FRENZYi[7]{ 0,0,0,0,0,0,0 };
float dt, timer = 0;

bool lessThanSec, medFishCanBeAten = 0, bigFishCanBeAten = 0;

Clock clock1;

Text FRENZYiText[7], scoretext;
Font font;
string word = "FRENZY!";
};
Score score;
struct fishes {
    bool aten = 0, ate = 0, playerIntersectWFish = 0, draw = 1, dead = 0;
    int size;
    int score;
    Sprite sprite;
    Texture texture;
    Vector2f Position;
    
    int frameW, frameH, mouthX, mouthY;
    int frames, countframe, currentframe;

    float anitimer;
    float speed;
    fishes() {
        frameW = 0, frameH = 0, frames = 0, anitimer = 0, countframe = currentframe = 0, speed = 100;
    }
    fishes(Texture& t, int f, int W, int H) {
        texture = t;
        countframe = f;
        currentframe = 0;
        anitimer = 0;
        frameW = W;
        frameH = H;
        sprite.setTexture(texture);
        sprite.setOrigin(frameW / 2, frameH / 2);
        mouthX = sprite.getPosition().x + sprite.getGlobalBounds().width / 2;
        mouthY = sprite.getPosition().y;
    }

};

struct Shark{
    Texture signalTexture, sharkTexture;
    Sprite signal, shark;

    float sharkAnimTimer = 0, sharkTimer = 0, turnTimer = 0, eatTimer = 0;
    int flashCount = 0, sharkFrame = 0, eatFrame = 0;
    bool fromRight = true, isTurning = false, sharkReady = false, showSignal = true,eating = false;
    bool ate, tailBite;
    float dt;

    Vector2f startPos;

    void start();
    void update(float dt);
    void draw(RenderWindow& window);
    void sharkMovement(float speed, bool fromRight, float deltaTime);
    void sharkEntrance(float& timer, int& flashCount, float deltaTime, Vector2f startPosition, bool& finished);
    void sharkAnimation(int frame, int row, int frameCountInEveryRow);
};

void intersection(RectangleShape playerHitbox, RectangleShape fishHitbox, Vector2f playerPosition, Vector2f fishPosition, int playerSize, int fishSize, int score, int lives, bool drawPlayer, bool canMove);

void window_mode(RenderWindow& window, bool full_screen, Vector2f res);

void addfishs(Texture& t, int f, int w, int h, const int n);

void setback(Sprite& sp, Texture& t);

Vector2f Mouthposition(fishes& player);

int start();

int draw(RenderWindow& window);

int scoring(Shark& shark, float dt);

void intersection();

void playermovement(float dt, fishes& fish, Sprite& sprite, Texture& t,RenderWindow& window);

void dead();

void Sorry(RenderWindow& window, float dt);

void ContinueY_N(RenderWindow& window);

int mouthintersect(fishes& a, fishes& b);

struct GameSettings{
    short sound_volume = 100, music_volume = 100;
    bool controls[4] ={1,1,1,0}; // 0 -> sfx, 1-> music, 2->full screen, 3 -> 0
    //bool sfx = 1, music = 1, full_screen = 1, font = 0;
    Vector2f res = {1920,1080};

    Font game_font;

    void load_essentials()
    {
        if (!game_font.loadFromFile("Fonts\\hint.ttf")) cout << "Menu font is not found" << endl;
        else controls[3] = 1;
    }
};

struct help_and_options{

    struct menuButtons{

        Texture checkbox_texture[2][2];
        Sprite checkbox_sprite[2][2];

        void load_buttons()
        {
            if(!checkbox_texture[0][0].loadFromFile("Sprites\\menu\\option menu check box.png")) cout << "option menu check box texture is not found" << endl;
            if(!checkbox_texture[0][1].loadFromFile("Sprites\\menu\\option menu check box-hoovered.png")) cout << "option menu check box texture is not found" << endl;
            if(!checkbox_texture[1][0].loadFromFile("Sprites\\menu\\marked check box.png")) cout << "option menu marked check box texture is not found" << endl;
            if(!checkbox_texture[1][1].loadFromFile("Sprites\\menu\\marked check box-hoovered.png")) cout << "option menu hoovered marked check box texture is not found" << endl;

            for(int i =0; i < 2; i++)
            {
                for(int j = 0; j < 2; j++)
                {
                checkbox_sprite[i][j].setTexture(checkbox_texture[i][j]);
                checkbox_sprite[i][j].setOrigin(checkbox_sprite[i][j].getGlobalBounds().width/2,checkbox_sprite[i][j].getGlobalBounds().height/2);
                checkbox_sprite[i][j].setScale(1.1f,1.1f);
                }
            }
        }
    };


    menuButtons buttons[4];
    Texture buttons_texture[6][2], menu_shell_texture[6],Rarrow_texture[1][2], Larrow_texture[1][2], mouse_texture, reft;
    Sprite buttons_sprite[6][2],menu_shell_sprite[6],Rarrow_sprite[1][2], Larrow_sprite[1][2],mouse_sprite, refs;
    string settings_string[4] = {"Sound:", "Music:", "Full Screen:", "Mouse Speed:"}, 
    how_to_play_string[2] {"Welcome to the beautiful Frenzy Coast.\nEnjoy the sights, but don't get too comfortable...\nA fish still needs his lunch, and it's a fish-eat-fish\nworld out there!",
        "- Use your mouse to control Boris.\n- Eat fish that are smaller than you.\n- Avoid anything that's larger than you.\n- Eat enough fish and you'll grow bigger!\n"};
    Text settings_text[4],how_to_play_text[2], sound_volume, music_volume;

    void load_assets(Vector2f res, Font& game_font)
    {
        reft.loadFromFile("Sprites\\menu\\2.png");
        refs.setTexture(reft);
        for(int i =0; i< 4; i++)
            buttons[i].load_buttons();
        
        if(!mouse_texture.loadFromFile("Sprites\\menu\\mouse.png")) cout << "mouse texture is not found" << endl;
        
        if(!Rarrow_texture[0][0].loadFromFile("Sprites\\menu\\yellow_arrow_small.png")) cout << "right arrow texture is not found" << endl;
        if(!Rarrow_texture[0][1].loadFromFile("Sprites\\menu\\yellow_arrow_small_glow.png")) cout << "glowing right arrow texture is not found" << endl;
        if(!Larrow_texture[0][0].loadFromFile("Sprites\\menu\\yellow_arrow_small_left.png")) cout << "left arrow texture is not found" << endl;
        if(!Larrow_texture[0][1].loadFromFile("Sprites\\menu\\yellow_arrow_small_left_glow.png")) cout << "glowing left arrow texture is not found" << endl;
        
        if(!menu_shell_texture[0].loadFromFile("Sprites\\menu\\shell_stageBack.png")) cout << "main shell's not found" << endl;
        if(!menu_shell_texture[1].loadFromFile("Sprites\\menu\\help-and-options-shell1.png")) cout << "help-and-optionsshell's not found" << endl;
        if(!menu_shell_texture[2].loadFromFile("Sprites\\menu\\settings-shell.png")) cout << "settings shell is not found" << endl;
        if(!menu_shell_texture[3].loadFromFile("Sprites\\menu\\controls-shell.png")) cout << "controls shell is not found" << endl;
        if(!menu_shell_texture[4].loadFromFile("Sprites\\menu\\how-to-play-shell.png")) cout << "controls shell is not found" << endl;
        
        if(!buttons_texture[0][0].loadFromFile("Sprites\\menu\\controls.png")) cout << "not found" << endl;
        if(!buttons_texture[0][1].loadFromFile("Sprites\\menu\\controls-glow.png")) cout << "not found" << endl;
        if(!buttons_texture[1][0].loadFromFile("Sprites\\menu\\how-to-play.png")) cout << "not found" << endl;
        if(!buttons_texture[1][1].loadFromFile("Sprites\\menu\\how-to-play-glow.png")) cout << "not found" << endl;
        if(!buttons_texture[2][0].loadFromFile("Sprites\\menu\\settings.png")) cout << "not found" << endl;
        if(!buttons_texture[2][1].loadFromFile("Sprites\\menu\\settings-glow.png")) cout << "not found" << endl;
        if(!buttons_texture[3][0].loadFromFile("Sprites\\menu\\credits.png")) cout << "not found" << endl;
        if(!buttons_texture[3][1].loadFromFile("Sprites\\menu\\credits-glow.png")) cout << "not found" << endl;
        if(!buttons_texture[4][0].loadFromFile("Sprites\\menu\\done.png")) cout << "not found" << endl;
        if(!buttons_texture[4][1].loadFromFile("Sprites\\menu\\done-glow.png")) cout << "not found" << endl;
        if(!buttons_texture[5][0].loadFromFile("Sprites\\menu\\lil-done.png")) cout << "not found" << endl;
        if(!buttons_texture[5][1].loadFromFile("Sprites\\menu\\lil-done-glow.png")) cout << "not found" << endl;

        for(int i = 0; i < 6; i++)
        {
            for(int j =0; j < 2; j++)
            {
            buttons_sprite[i][j].setTexture(buttons_texture[i][j]);
            buttons_sprite[i][j].setOrigin(buttons_sprite[i][j].getGlobalBounds().width/2,buttons_sprite[i][j].getGlobalBounds().height/2);
            if(!j)
                buttons_sprite[i][j].setScale(0.245f, 0.245f);
            else
                buttons_sprite[i][j].setScale(0.255f, 0.255f);

            switch (i)
            {
                case 0: buttons_sprite[i][j].setPosition(res.x /2.f, res.y * 0.265f); break;
                case 1: buttons_sprite[i][j].setPosition(res.x /2.f, res.y * 0.38f); break;
                case 2: buttons_sprite[i][j].setPosition(res.x /2.f, res.y * 0.495f); break;
                case 3: buttons_sprite[i][j].setPosition(res.x /2.f, res.y * 0.61f); break;
                case 4: buttons_sprite[i][j].setPosition(res.x /2.f, res.y * 0.79f); break;
                case 5: buttons_sprite[i][j].setPosition(res.x /2.f, res.y * 0.79f);
                    if(!j)
                        buttons_sprite[i][j].setScale(0.2,0.2);
                    else
                        buttons_sprite[i][j].setScale(0.22,0.22);
                break;
            }

            if(i == 1 || i == 2)
            {
                if(!j)
                    buttons_sprite[i][j].setScale(0.245, 0.238);
                else
                    buttons_sprite[i][j].setScale(0.255, 0.239);
            }
            }
        }

        for(int i =0; i < 5;i++)
        {
            menu_shell_sprite[i].setTexture(menu_shell_texture[i]);
            menu_shell_sprite[i].setOrigin(menu_shell_sprite[i].getGlobalBounds().width/2, menu_shell_sprite[i].getGlobalBounds().height/2);
            menu_shell_sprite[i].setPosition(res.x /2, res.y * 0.51f);
            menu_shell_sprite[i].setScale(1.76,1.75);
        }

        mouse_sprite.setTexture(mouse_texture);
        mouse_sprite.setOrigin(mouse_sprite.getGlobalBounds().width/2, mouse_sprite.getGlobalBounds().height/2);
        mouse_sprite.setPosition(res.x * 0.43f, res.y * 0.5f);
        mouse_sprite.setScale(0.9f,0.9f);

        for(int i =0; i < 4; i++)
        {
            settings_text[i].setString(settings_string[i]);
            settings_text[i].setFont(game_font);
            settings_text[i].setFillColor(Color::White);
            settings_text[i].setOutlineColor(Color::Black);
            settings_text[i].setCharacterSize(48);
            settings_text[i].setOutlineThickness(2);
            FloatRect bounds = settings_text[i].getLocalBounds();
            settings_text[i].setOrigin(bounds.left + bounds.width, bounds.top);
            
            settings_text[i].setPosition(0.477f * res.x, 0.272f * res.y + i * 100);
        }
        
        for(int i =0; i < 2; i++)
        {
            how_to_play_text[i].setString(how_to_play_string[i]);
            how_to_play_text[i].setFont(game_font);
            how_to_play_text[i].setFillColor(Color::White);
            how_to_play_text[i].setOutlineColor(Color::Black);
            how_to_play_text[i].setCharacterSize(34);
            how_to_play_text[i].setOutlineThickness(2);
            how_to_play_text[i].setOrigin(how_to_play_text[i].getGlobalBounds().width/2,how_to_play_text[i].getGlobalBounds().height/2);
        }

        how_to_play_text[0].setPosition((0.44f) * res.x,(0.335f) * res.y);
        how_to_play_text[1].setPosition((0.435f) * res.x,(0.585) * res.y);

        for(int i =0; i<4; i++)
            for(int j =0; j < 2;j++)
                for(int h =0; h < 2; h++)
                    buttons[i].checkbox_sprite[j][h].setPosition(0.495f * res.x, 0.295f * res.y + i * 100);


        
        sound_volume.setPosition(0.495f * res.x, 0.272f * res.y);
        sound_volume.setFont(game_font);
        sound_volume.setFillColor(Color::White);
        sound_volume.setOutlineColor(Color::Black);
        sound_volume.setCharacterSize(48);
        sound_volume.setOutlineThickness(2);

    }

    void handle_movements(Event& event, short &selected, short &menu_scene, RenderWindow& window, GameSettings &settings)
    {

        if (event.type == Event::KeyPressed)
        {
            if(menu_scene == 6)
            {
                if (event.key.code == Keyboard::Escape)
                    menu_scene = 1;
                if (event.key.code == Keyboard::Down)
                    selected++, selected %= 5;
                if (event.key.code == Keyboard::Up)
                    selected = (selected - 1) + 5, selected %= 5;
            }
            else if(menu_scene == 13)
            {
                if (event.key.code == Keyboard::Escape)
                    menu_scene = 6;
            }
        }

        if (event.type == Event::MouseMoved)
        {

            Vector2i mousePixel = Mouse::getPosition(window);
            Vector2f mouse_position = window.mapPixelToCoords(mousePixel);

            if(menu_scene == 6)
            {
                for(int i =0; i < 5; i++)
                {
                    if(buttons_sprite[i][0].getGlobalBounds().contains(mouse_position))
                    {
                        selected = i;
                        break;
                    }
                    else
                    selected = -1;
                }
            }

            else if (menu_scene == 11)
            {
                for(int i = 0; i < 4; i++)
                {
                    if(buttons_sprite[5][0].getGlobalBounds().contains(mouse_position))
                    {
                        selected = 0;
                        break;
                    }
                    if(buttons[i].checkbox_sprite[0][0].getGlobalBounds().contains(mouse_position))
                    {
                        selected = i + 1;
                        break;
                    }
                    else 
                        selected = -1;
                }

            }
                else if(menu_scene == 12 || menu_scene == 13 || menu_scene == 14)
                {
                    selected = buttons_sprite[4][0].getGlobalBounds().contains(mouse_position) ? 0 : -1;
                }
        }

        if (event.type == Event::MouseButtonPressed)
        {
            Vector2i mousePixel = Mouse::getPosition(window);
            Vector2f mouse_position = window.mapPixelToCoords(mousePixel);
            if(event.mouseButton.button == Mouse::Left)
            {
                if(menu_scene == 6)
                {
                    if(selected == 0)
                        menu_scene = 12, selected = -1;
                    else if(selected == 1)
                        menu_scene = 13;
                    else if(selected == 2)
                        menu_scene = 11, selected = -1;
                    else if(selected == 3)
                        menu_scene = 14, selected = -1;
                    else if(selected == 4)
                        menu_scene = 1, selected = -1;
                }
                else if(menu_scene == 11)
                {
                    if(selected == 0)
                        menu_scene = 6, selected = -1;
                    else if(selected == 1)
                        settings.controls[0] = !settings.controls[0];
                    else if(selected == 2)
                        settings.controls[1] = !settings.controls[1];
                    else if(selected == 3)
                        settings.controls[2] = !settings.controls[2], window_mode(window, settings.controls[2], settings.res);
                }
                else if(menu_scene == 12 || menu_scene == 13 || menu_scene == 14)
                {
                    if(selected == 0) menu_scene = 6, selected = -1;
                }
            }
        }
    }

    void draw(RenderWindow& window, short selected, Sprite main_menu_background_sprite,Sprite menu_rocks_sprite)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
        window.draw(menu_rocks_sprite);
        //window.draw(refs);
        for(int i = 0; i < 2; i++)
            window.draw(menu_shell_sprite[i]);
        for(int i = 0; i < 5; i++)
            window.draw(buttons_sprite[i][selected == i ? 1 : 0]);
        window.display();
    }

    void update_settings(GameSettings& settings)
    {
        sound_volume.setString(to_string(settings.sound_volume));
        music_volume.setString(to_string(settings.music_volume));
        FloatRect bounds = sound_volume.getLocalBounds();
        sound_volume.setOrigin(bounds.left + bounds.width, bounds.top);
    }

    void draw_settings(RenderWindow& window, short selected, Sprite main_menu_background_sprite,Sprite menu_rocks_sprite, GameSettings& settings)
    {
        update_settings(settings);
        window.clear();
        window.draw(main_menu_background_sprite);
        window.draw(menu_rocks_sprite);
        //window.draw(refs);
        for(int i = 0; i < 3; i=i+2)
            window.draw(menu_shell_sprite[i]);
        
        for(int i =0; i < 4; i++)
        {
            window.draw(settings_text[i]);
        }
        for(int i = 0; i < 4; i++)
            window.draw(buttons[i].checkbox_sprite[settings.controls[i] ? 1 : 0][selected == (i + 1) ? 1 : 0]);
        //window.draw(sound_volume);
        window.draw(buttons_sprite[5][selected == 0 ? 1 : 0]);
        window.display();
    }

    void draw_controls(RenderWindow& window, short selected, Sprite main_menu_background_sprite,Sprite menu_rocks_sprite)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
        window.draw(menu_rocks_sprite);

        for(int i = 0; i < 4; i=i+3)
        {
            window.draw(menu_shell_sprite[i]);
            if(i == 3)
                continue;
            window.draw(mouse_sprite);
        }

        window.draw(buttons_sprite[5][selected == 0 ? 1 : 0]);
        window.display();
    }

    void draw_how_to_play(RenderWindow& window, short selected, Sprite main_menu_background_sprite,Sprite menu_rocks_sprite)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
        window.draw(menu_rocks_sprite);
        //window.draw(refs);
        for(int i = 0; i < 5; i=i+4)
            window.draw(menu_shell_sprite[i]);
        for(int i =0; i < 2; i++)
            window.draw(how_to_play_text[i]);
        window.draw(buttons_sprite[4][selected == 0 ? 1 : 0]);
        window.display();
    }

    void draw_credits(RenderWindow& window, short selected, Sprite main_menu_background_sprite,Sprite menu_rocks_sprite)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
        window.draw(menu_rocks_sprite);
        //window.draw(refs);
        for(int i = 0; i < 3; i=i+2)
            window.draw(menu_shell_sprite[i]);
        window.draw(buttons_sprite[4][selected == 0 ? 1 : 0]);
        window.display();
    }
};

struct Ingame_windows{
    void load()
    {

    }

    void draw_first_tip(RenderWindow& window, Sprite& background, Sprite& rock)
    {

    }
};

struct mainMenu{
    help_and_options* help = nullptr;
    short selected =-1, menu_scene =0;
    // scenes 0 -> click to start, 1 -> main menu, 2-> single player, 3-> multiplayer, 4-> leaderboards
    // 5 -> achievements, 6->help, 7-> quit 
    // 8 -> story mode, 9 - > story lite, 10 -> time attack, 
    // settings = 11, controls = 12, how to play = 13, credits = 14
    Texture main_menu_background_text, logo_texture,main_menu_buttons_texture[6][2], reft, txt, menu_rocks_text,
    tool_tip_texture,tube_texture, ment, rr,
    single_player_texture[4][2], quads_texture[6], the_sign_texture;
    Sprite main_menu_background_sprite,main_menu_buttons_sprite[6][2], logo_sprite, menu_rocks_sprite,
    tool_tip_sprite, tube_sprite, mens, refs, txts, rrs, single_player_sprite[4][2], quads_sprite[6], the_sign_sprite;

    Vector2f Res;
    SoundBuffer click_buffer, main_theme_buffer;
    Sound click_sound, main_theme_sound;
    string menu_strings[6] = {"Single Player Game Modes","Multiplayer Game Modes","View Leaderboards", "See Your Achievements", "Adjust Sound and Music Settings Or Learn How tp Play", "Return to Desktop"};
    Text menu_text[6];
    Font menu_font;
    void load_assets(Vector2f res, GameSettings &settings)
    {

        if(!settings.controls[3])
            settings.load_essentials();

        Res = res;
        Vector2f buttons_scale;
        if (!main_menu_background_text.loadFromFile("Sprites\\2\\Menu assets\\mainMenu.jpg")) cout << "main menu background is not found" << endl;
        main_menu_background_sprite.setTexture(main_menu_background_text);
        main_menu_background_sprite.setScale(res.x/main_menu_background_text.getSize().x, res.y/main_menu_background_text.getSize().y);
        
        if (!rr.loadFromFile("Sprites\\menu\\ref.png")) cout << "rr's texture is not found" << endl;
        rrs.setTexture(rr);
        // Loading buttons!
        if (!main_menu_buttons_texture[0][0].loadFromFile("Sprites\\menu\\single-player.png")) cout << "button 1's texture is not found" << endl;
        if (!main_menu_buttons_texture[0][1].loadFromFile("Sprites\\menu\\single-player-glow.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!main_menu_buttons_texture[1][0].loadFromFile("Sprites\\menu\\multiplayer.png")) cout << "button 2's texture is not found" << endl;
        if (!main_menu_buttons_texture[1][1].loadFromFile("Sprites\\menu\\multiplayer-glow.png")) cout << "glowing button 2 texture is not found" << endl;
        if (!main_menu_buttons_texture[2][0].loadFromFile("Sprites\\menu\\leaderboards.png")) cout << "button 3's texture is not found" << endl;
        if (!main_menu_buttons_texture[2][1].loadFromFile("Sprites\\menu\\leaderboards-glow.png")) cout << "glowing button 3 texture is not found" << endl;
        if (!main_menu_buttons_texture[3][0].loadFromFile("Sprites\\menu\\achievements.png")) cout << "button 4's texture is not found" << endl;
        if (!main_menu_buttons_texture[3][1].loadFromFile("Sprites\\menu\\achievements-glow.png")) cout << "glowing button 4 texture is not found" << endl;
        if (!main_menu_buttons_texture[4][0].loadFromFile("Sprites\\menu\\help-and-options.png")) cout << "button 5's texture is not found" << endl;
        if (!main_menu_buttons_texture[4][1].loadFromFile("Sprites\\menu\\help-and-options-glow.png")) cout << "glowing button 5 texture is not found" << endl;
        if (!main_menu_buttons_texture[5][0].loadFromFile("Sprites\\menu\\quit.png")) cout << "button 6's texture is not found" << endl;
        if (!main_menu_buttons_texture[5][1].loadFromFile("Sprites\\menu\\quit-glow.png")) cout << "glowing button 6 texture is not found" << endl;

        if (!single_player_texture[0][0].loadFromFile("Sprites\\menu\\story-mode.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[0][1].loadFromFile("Sprites\\menu\\story-mode-glow.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[1][0].loadFromFile("Sprites\\menu\\story-title.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[1][1].loadFromFile("Sprites\\menu\\story-title-glow.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[2][0].loadFromFile("Sprites\\menu\\time-attack.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[2][1].loadFromFile("Sprites\\menu\\time-attack-glow.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[3][0].loadFromFile("Sprites\\menu\\back.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[3][1].loadFromFile("Sprites\\menu\\back-glow.png")) cout << "glowing button 1's texture is not found" << endl;
        
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            main_menu_buttons_sprite[i][j].setTexture(main_menu_buttons_texture[i][j]);
            main_menu_buttons_sprite[i][j].setOrigin(main_menu_buttons_sprite[i][j].getGlobalBounds().width/2,main_menu_buttons_sprite[i][j].getGlobalBounds().height/2);

            if(!j)
                main_menu_buttons_sprite[i][j].setScale(0.24f, 0.24f);
            else
                main_menu_buttons_sprite[i][j].setScale(0.255f, 0.255f);

            switch(i)
            {
                case 0:
                    main_menu_buttons_sprite[i][j].setPosition(res.x * 0.7f, res.y * 0.12f);
                    if(j){ main_menu_buttons_sprite[i][j].setScale(0.3f, 0.3f); break;}
                    main_menu_buttons_sprite[i][j].setScale(0.285f, 0.285f);
                    break;
                case 1:
                    main_menu_buttons_sprite[i][j].setPosition(res.x * 0.7f, res.y * 0.25f);
                    main_menu_buttons_sprite[i][j].setRotation(-1.3f);
                    if(j) {main_menu_buttons_sprite[i][j].setScale(0.28f, 0.28f); break;}
                    main_menu_buttons_sprite[i][j].setScale(0.265f,0.265f);
                    break;
                case 2:
                    main_menu_buttons_sprite[i][j].setPosition(res.x * 0.7f, res.y * 0.36f);
                    break;
                case 3:
                    main_menu_buttons_sprite[i][j].setPosition(res.x * 0.7f, res.y * 0.46f);
                    if(j) {main_menu_buttons_sprite[i][j].setScale(0.26f, 0.26f); break;}
                    main_menu_buttons_sprite[i][j].setScale(0.48f,0.48f);
                    break;
                case 4:
                    main_menu_buttons_sprite[i][j].setPosition(res.x * 0.7f, res.y * 0.565f);
                    main_menu_buttons_sprite[i][j].setRotation(-1.05f);
                    break;
                case 5:
                    main_menu_buttons_sprite[i][j].setPosition(res.x * 0.7, res.y * 0.66f);
                    main_menu_buttons_sprite[i][j].setRotation(-1.45f);
                    if(j) {main_menu_buttons_sprite[i][j].setScale(0.19f, 0.19f); break;}
                    main_menu_buttons_sprite[i][j].setScale(0.18f,0.18f);
                    break;
            }
        }
    }

        for(int i = 0; i < 4; i++)
        {
            for(int j =0; j < 2; j++)
            {
                single_player_sprite[i][j].setTexture(single_player_texture[i][j]);
                buttons_scale= {(res.x * 0.25f) / single_player_texture[i][j].getSize().x,(res.y * 0.115f) / single_player_texture[i][j].getSize().y};
                single_player_sprite[i][j].setScale(buttons_scale.x, buttons_scale.y);
                if(i == 0)
                {
                    single_player_sprite[i][j].setPosition(res.x * 0.56501f, res.y * 0.262f);
                    single_player_sprite[i][j].rotate(1.8);
                }
                else if (i == 1)
                {
                    single_player_sprite[i][j].setPosition(res.x * 0.56501f, res.y * 0.386f);
                    single_player_sprite[i][j].rotate(-1.3);
                }
                else if (i == 2)
                {
                    single_player_sprite[i][j].setPosition(res.x * 0.56501f, res.y * 0.486f);
                    single_player_sprite[i][j].rotate(1.3);
                }
                else
                {
                    single_player_sprite[i][j].setPosition(res.x * 0.56501f, res.y * 0.596f);
                    single_player_sprite[i][j].rotate(1.3);
                }
            }
        }

        // al original menu "for refrence"
        ment.loadFromFile("men.jpg");
        mens.setScale(res.x/ment.getSize().x, res.y/ment.getSize().y);
        mens.setTexture(ment);
        
        if (!tool_tip_texture.loadFromFile("Sprites\\2\\Menu assets\\tooltip.png")) cout << "tooltip's texture is not found" << endl;
        tool_tip_sprite.setTexture(tool_tip_texture);
        tool_tip_sprite.setOrigin(tool_tip_sprite.getGlobalBounds().width / 2, 0);
        tool_tip_sprite.setPosition(res.x / 2, res.y * 0.735f);
        tool_tip_sprite.setScale((res.x * 0.5f) / tool_tip_texture.getSize().x, (res.y * 0.19f) / tool_tip_texture.getSize().y);
        
        if (!logo_texture.loadFromFile("Sprites\\2\\Menu assets\\logo.png")) cout << "logo's texture is not found" << endl;
        logo_sprite.setTexture(logo_texture);
        logo_sprite.setScale((res.x * 0.396f) / logo_texture.getSize().x, (res.y * 0.39f) / logo_texture.getSize().y);
        logo_sprite.setPosition(res.x * 0.296f, res.y * 0.033f);
        
        if (!tube_texture.loadFromFile("Sprites\\2\\Menu assets\\tubes_01.png")) cout << "tube's texture is not found" << endl;
        tube_sprite.setTexture(tube_texture);
        tube_sprite.setOrigin(tube_texture.getSize().x / 2,tube_texture.getSize().y);
        tube_sprite.setScale((res.x * 0.135f) / tube_texture.getSize().x, (res.y * 0.315f) / tube_texture.getSize().y);
        tube_sprite.setPosition(res.x * 0.715f, res.y * 1.f);
        
        if (!main_theme_buffer.loadFromFile("Sounds\\music\\menu_theme.wav")) cout << "tube's texture is not found" << endl;
        main_theme_sound.setBuffer(main_theme_buffer);
        main_theme_sound.setVolume(settings.music_volume);
        main_theme_sound.setLoop(true);
        
        if (!txt.loadFromFile("Fonts\\press_any_button.png")) cout << "button is not found" << endl;
        txts.setTexture(txt);
        txts.setScale(res.x/txt.getSize().x, res.y/txt.getSize().y);
        //txts.setOrigin(txt.getSize().x / 2,txt.getSize().y);
        //txts.setPosition(res.x , res.y );
        //txts.setScale(res.x / txt.getSize().x, res.y / txt.getSize().y);
        
        if (!menu_font.loadFromFile("Fonts\\hint.ttf")) cout << "Menu font is not found" << endl;
        for (int i = 0; i < 1; i++)
        {
            menu_text[i].setString(menu_strings[i]);
            menu_text[i].setCharacterSize(50);
            menu_text[i].setFillColor(Color::White);
            menu_text[i].setOutlineColor(Color::Black);
            menu_text[i].setOutlineThickness(2);
            FloatRect textBounds = menu_text[i].getLocalBounds();
            menu_text[i].setOrigin(textBounds.left + textBounds.width / 2.f,
            textBounds.top  + textBounds.height / 2.f);
            FloatRect tipBounds = tool_tip_sprite.getGlobalBounds();
            menu_text[i].setPosition(tipBounds.left + tipBounds.width  / 2.f,
            tipBounds.top  + tipBounds.height / 2.f);
            menu_text[i].setFont(menu_font);
            
        }
        
        if (!menu_rocks_text.loadFromFile("Sprites\\2\\Menu assets\\rocks_g.png")) cout << "Rocks are not found" << endl;
        menu_rocks_sprite.setTexture(menu_rocks_text);
        menu_rocks_sprite.setScale((res.x/menu_rocks_text.getSize().x), res.y/menu_rocks_text.getSize().y);
        menu_rocks_sprite.setPosition(res.x * 0.0645, res.y * 0.314f);
        // click to start refrence "hab2a ashelha ba3deen"
        if (!reft.loadFromFile("ref1.png")) cout << "ref's texture is not found" << endl;
        refs.setTexture(reft);
        refs.setScale(res.x/reft.getSize().x, res.y/reft.getSize().y);
        if (!click_buffer.loadFromFile("Sounds\\sfx\\pop.mp3")) cout << "quad q1 is not found" << endl;
        click_sound.setBuffer(click_buffer);
        
        
        if (!the_sign_texture.loadFromFile("Sprites\\menu\\sprite_clean.png")) cout << "quad back is not found" << endl;
        the_sign_sprite.setTexture(the_sign_texture);
        the_sign_sprite.setPosition(res.x * 0.355, res.y * 0.785f);
        the_sign_sprite.setScale((res.x * 0.3f) / the_sign_texture.getSize().x, (res.y * 0.1f) / the_sign_texture.getSize().y);


        if (!quads_texture[0].loadFromFile("Sprites\\menu\\quad_back.png")) cout << "quad back is not found" << endl;
        if (!quads_texture[1].loadFromFile("Sprites\\menu\\quad_ring.png")) cout << "quad ring is not found" << endl;
        if (!quads_texture[2].loadFromFile("Sprites\\menu\\quad_q2.png")) cout << "quad q2 is not found" << endl;
        if (!quads_texture[3].loadFromFile("Sprites\\menu\\quad_q3.png")) cout << "quad q3 is not found" << endl;
        if (!quads_texture[4].loadFromFile("Sprites\\menu\\quad_q4.png")) cout << "quad q4 is not found" << endl;
        if (!quads_texture[5].loadFromFile("Sprites\\menu\\quad_q1.png")) cout << "quad q1 is not found" << endl;
        
        for(int i = 0; i < 6; i++)
        {
            quads_sprite[i].setTexture(quads_texture[i]);
            quads_sprite[i].setOrigin(quads_sprite[i].getGlobalBounds().width/2, quads_sprite[i].getGlobalBounds().height/2);
            quads_sprite[i].setPosition(res.x * 0.69201f, res.y * 0.165f);
            buttons_scale= {(res.x * 0.171f) / quads_texture[i].getSize().x,(res.y * 0.28f) / quads_texture[i].getSize().y};
            quads_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
        }
    };

    void logo_transformation(char a)
    {
        if (a == 'f')
        {
        logo_sprite.setScale((Res.x * 0.365f) / logo_texture.getSize().x, (Res.y * 0.37f) / logo_texture.getSize().y);
        logo_sprite.setPosition(Res.x * 0.1665f, Res.y * 0.05f);
        }
        else
        {
            logo_sprite.setScale((Res.x * 0.396f) / logo_texture.getSize().x, (Res.y * 0.39f) / logo_texture.getSize().y);
            logo_sprite.setPosition(Res.x * 0.296f, Res.y * 0.033f);
        }
    };

    void Click_sound(GameSettings& settings)
    {
        if(settings.controls[0])
            click_sound.setVolume(70);
        else
            click_sound.setVolume(0);
        click_sound.play();
    }

    void handle_movements(Event& event, short &scene, RenderWindow& window, GameSettings &settings)
    {
        if (event.type == Event::KeyPressed)
        {
            Click_sound(settings);
            if(menu_scene == 0 && event.key.code != Keyboard::Escape)
            {
                logo_transformation('f');
                menu_scene++;
            }
            else if(menu_scene == 1)
            {
                if (event.key.code == Keyboard::Down)
                    selected ++, selected %= 6;
                if (event.key.code == Keyboard::Up)
                    selected = (selected - 1) + 6 , selected%=6;
                if (event.key.code == Keyboard::Escape)
                {
                    logo_transformation('b');
                    menu_scene--; selected = -1;
                    cout << menu_scene << endl;
                }
                if(event.key.code == Keyboard::Enter)
                    if(menu_scene == 1)
                    {
                        if(selected == 0)
                        {
                            menu_scene = 2;
                        }
                        else if(selected == 1)
                        {
                            menu_scene = 3;
                        }
                        else if(selected == 2)
                            menu_scene = 4;
                        else if(selected == 3)
                            menu_scene = 5;
                        else if(selected == 4)
                            menu_scene = 6;
                        else if(selected == 5)
                        {
                            window.close();
                        }
                        selected = -1;
                    }
                    else if(menu_scene == 2)
                    {
                        if(selected == 0)
                        {
                            menu_scene = 2;
                        }
                        else if(selected == 1);
                        
                            //menu_scene = 3;
                        
                        else if(selected == 2);
                            //menu_scene = 4;
                        else if(selected == 3)
                            menu_scene = 1;
                    }
            }
            else if(menu_scene == 2)
            {
                if (event.key.code == Keyboard::Escape)
                    menu_scene = 1;
                if (event.key.code == Keyboard::Down)
                    selected ++, selected %= 4;
                if (event.key.code == Keyboard::Up)
                    selected = (selected - 1) + 4 , selected%=4;
            }
            else if(menu_scene == 3)
            {
                if (event.key.code == Keyboard::Escape)
                    menu_scene = 1;
            }
            else if(menu_scene == 4)
            {
                if (event.key.code == Keyboard::Escape)
                    menu_scene = 1;
            }
            else if(menu_scene == 5)
            {
                if (event.key.code == Keyboard::Escape)
                    menu_scene = 1;
            }
            else if((menu_scene == 6 || menu_scene == 11 || menu_scene == 12||menu_scene == 13 || menu_scene == 14) && help)
            {
                help->handle_movements(event, selected, menu_scene, window, settings);
            }
        }

if (event.type == Event::MouseMoved)
{
    Vector2i mousePixel = Mouse::getPosition(window);
    Vector2f mouse_position = window.mapPixelToCoords(mousePixel);
    switch(menu_scene)
    {
        case 1:
            selected = -1;
            for(int i = 0; i < 6; i++)
            {
                if(main_menu_buttons_sprite[i][0].getGlobalBounds().contains(mouse_position))
                {
                    selected = i;
                    break;
                }
                else
                    selected = -1;
            }
            break;
        case 2:
            for(int i = 0; i < 4; i++)
            {
                if(single_player_sprite[i][0].getGlobalBounds().contains(mouse_position))
                {
                    selected = i;
                    break;
                }
                else
                    selected = -1;
            }
            break;
        case 6: case 11: case 12: case 13: case 14:
            if(help)
                help->handle_movements(event, selected, menu_scene, window, settings);
            break;
    }
}

        if (event.type == Event::MouseButtonPressed)
        {

            Click_sound(settings);
            Vector2i mousePixel = Mouse::getPosition(window);
            Vector2f mouse_position = window.mapPixelToCoords(mousePixel);

            if(event.mouseButton.button == Mouse::Left)
            {
                if (menu_scene == 0)
                {
                    logo_transformation('f');
                    menu_scene++;
                }

                if(menu_scene == 1)
                {
                    for(int i = 0; i <6; i++)
                    {
                        if(selected == 5)
                            window.close();
                        if(selected == i)
                            menu_scene = i+2, selected = -1;
                    }
                }

                else if(menu_scene == 2)
                {
                    for(int i = 0; i < 4; i++)
                    {
                        if(i == 3)
                            menu_scene = 1, selected = -1;
                        if(selected == i)
                            menu_scene = i + 8, selected = -1; 
                    }
                }
                else if((menu_scene == 6 || menu_scene == 11 || menu_scene == 12 ||menu_scene == 13 || menu_scene == 14) && help)
                    help->handle_movements(event, selected, menu_scene, window, settings);
            }
        }
    };

    void update_menu_scenes(RenderWindow& window, GameSettings& settings)
    {
        if (main_theme_sound.getStatus() != Sound::Playing && settings.controls[1])
            main_theme_sound.play();

        if(!settings.controls[1])
            main_theme_sound.stop();

        switch(menu_scene)
        {
            case 0 : draw_starting_menu(window);break;
            case 1 : draw_main_menu(window); break;
            case 2 : draw_single_player_menu(window); break;
            case 3 : draw_multiplayer_menu(window); break;
            case 4 : draw_leaderboards_menu(window); break;
            case 5 : draw_achievements_menu(window); break;
            case 6 :
                if (!help)
                {
                    help = new help_and_options();
                    help->load_assets(Res, settings.game_font);
                }
                help->draw(window, selected, main_menu_background_sprite,menu_rocks_sprite);
                break;
            case 11 : help->draw_settings(window, selected, main_menu_background_sprite,menu_rocks_sprite, settings); break;
            case 12 : help->draw_controls(window, selected, main_menu_background_sprite,menu_rocks_sprite); break;
            case 13 : help->draw_how_to_play(window, selected, main_menu_background_sprite,menu_rocks_sprite); break;
            case 14 : help->draw_credits(window, selected, main_menu_background_sprite,menu_rocks_sprite); break;
        }
    }

    void draw_starting_menu(RenderWindow& window)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
        //window.draw(refs);
        window.draw(logo_sprite);
        //window.draw(tube_sprite);
        window.draw(menu_rocks_sprite);
        window.draw(txts);
        window.display();
    }
    
    void draw_main_menu(RenderWindow& window)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
        window.draw(logo_sprite);
        //window.draw(tube_sprite);
        
        for(int i =5; i>=0; i--)
        {
            window.draw(main_menu_buttons_sprite[i][selected == i ? 1 : 0]);
        }
        //window.draw(menu_text[0]);
        window.draw(menu_rocks_sprite);
        window.draw(tool_tip_sprite);
        window.draw(the_sign_sprite);
        window.display();
    };
    
    void draw_single_player_menu(RenderWindow& window)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
        window.draw(logo_sprite);

        for(int i = 0; i < 4; i++)
        {
            window.draw(single_player_sprite[i][selected == i ? 1 : 0]);
            if(i > 2)
                continue;
            window.draw(quads_sprite[i]);
        }

        window.draw(menu_rocks_sprite);
        window.draw(tool_tip_sprite);
        window.display();
    }

    void draw_multiplayer_menu(RenderWindow& window)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
        window.draw(logo_sprite);
        window.draw(menu_rocks_sprite);
        window.display();
    }

    void draw_leaderboards_menu(RenderWindow& window)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
        window.draw(logo_sprite);
        window.draw(menu_rocks_sprite);
        window.display();
    }

    void draw_achievements_menu(RenderWindow& window)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
        window.draw(logo_sprite);
        window.draw(menu_rocks_sprite);
        window.display();
    }

    void draw_help_menu(RenderWindow& window)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
        window.draw(logo_sprite);
        window.draw(menu_rocks_sprite);
        window.display();
    }
};

const int MAX_STARS = 30;

struct Mermaid{
    Texture mermaidTexture;
    Sprite mermaid;

    int frame = 0;
    float timer = 0;

    Texture starTexture;
    Sprite stars[MAX_STARS];
    bool active[MAX_STARS];
    
    float starTimer = 0;

    void start_mermaid();
    void update_mermaid(float dt);
    void draw_mermaid(RenderWindow& window);
    void mermaidMovement(float speed);
    void mermaidAnimation(int& fraame);
};

struct SORRY {
    Sprite sprites[5];
    Texture textures[5];
    bool draw = 0;
    float timer = 0;
    bool active = 0;
};

struct Cont {
    Text CONTtext, YEStext, NOtext, ContinuesLeft;
	Texture CONTtexture, YESt[2], NOt[2];
	Sprite CONTsprite, YESsprite[2], NOsprite[2]; //where 0 is nonactivated and 1 is activated
    bool showen = 0;

};

struct Preal {
    bool aten;
};
fishes enemy[100];


Cont Continue;
SORRY sorry;
fishes smallFish, medFish, bigFish, player;
Preal preal;

int main()
{
    mainMenu* menu = nullptr;
    GameSettings settings;
    RenderWindow window;
    window_mode(window, settings.controls[2], settings.res);
    Event event;
    Image icon;
    icon.loadFromFile("Sprites\\icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    short scene = 2; // 0 for start_mermaid menu, 1 -> mermaid animation, 2-> shark
    Clock clock;
    Mermaid mermaid; Shark shark;
    mermaid.start_mermaid();
    shark.start();
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if(menu)
                menu->handle_movements(event, scene, window, settings);
        }
        switch (scene)
        {
        case 0:
            if (!menu)
            {
                menu = new mainMenu();
                menu->load_assets(settings.res, settings);
            }
            menu->update_menu_scenes(window, settings);
            break;
        case 1:
            mermaid.update_mermaid(dt);
            mermaid.draw_mermaid(window);
            break;
        case 2:
            shark.update(dt);
            shark.draw(window);
            break;
        }


            while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Space)
                {
                    if (!shark.isTurning && !shark.eating)
                    {
                        shark.isTurning = true;
                        shark.turnTimer = 0;
                        shark.sharkFrame = 0;
                    }
                }

                if (event.key.code == Keyboard::G)
                {
                    if (!shark.eating)
                    {
                        shark.eating = true;
                        shark.eatFrame = 0;
                        shark.eatTimer = 0;
                    }
                }
            }
        }
    }


    return 0;
}




void addfishs(Texture& t, int f, int w, int h, const int n) {
    for (int i = 0; i < n; i++) {
        enemy[i] = fishes(t, f, w, h);
        enemy[i].sprite.setPosition(rand() % 800, rand() % 600);
        enemy[i].speed = rand() % 100 + 100; // to make speed random
    }

}

Vector2f Mouthposition(fishes& player) {
    Vector2f pos = player.sprite.getPosition();
    float offset = player.frameW / 2;
    if (player.currentframe == 1) return Vector2f(pos.x + offset, pos.y);
    else
        return Vector2f(pos.x - offset, pos.y);
}

void setback(Sprite& sp, Texture& t) { // to make background suit with window
    sp.setTexture(t);
    sp.setScale(
        1600.f / t.getSize().x,
        900.f / t.getSize().y
    );
}

void intersection(RectangleShape playerHitbox, RectangleShape fishHitbox, Vector2f playerPosition, Vector2f fishPosition, int playerSize, int fishSize, int score, int lives, bool drawPlayer, bool canMove) {

	if (playerHitbox.getGlobalBounds().intersects(fishHitbox.getGlobalBounds())) {

		if (playerSize >= fishSize) {

			score += 50;
			playerSize += 5;

		}

		else {

			lives--;

		}

	}

	if (lives <= 0) {

		lives = 0;
		drawPlayer = false;
		canMove = false;
		playerPosition = fishPosition;

	}

}

int start() {

    //test stuff//
    player.frameW = 356 / 2;
    player.frameH = 167;

    smallFish.frameW = 127;
    smallFish.frames = 105;

    medFish.frameW = 127;
    medFish.frames = 105;

    bigFish.frameW = 127;
    bigFish.frames = 105;

    smallFish.texture.loadFromFile("Assets\\Untitled-1.png");
    smallFish.sprite.setTexture(smallFish.texture);
    smallFish.sprite.setPosition(900, 100);
    smallFish.sprite.setScale(.7, .7);

    medFish.texture.loadFromFile("Assets\\Untitled-1.png");
    medFish.sprite.setTexture(medFish.texture);
    medFish.sprite.setPosition(900, 300);
    medFish.sprite.setScale(1.2, 1.2);

    bigFish.texture.loadFromFile("Assets\\Untitled-1.png");
    bigFish.sprite.setTexture(bigFish.texture);
    bigFish.sprite.setPosition(900, 500);
    bigFish.sprite.setScale(2, 2);
    //  //


    player.size = 15; // then 35 then 55
    smallFish.size = 10;
    medFish.size = 30;
    bigFish.size = 50;

    smallFish.score = 10;
    medFish.score = 20;
    bigFish.score = 50;

    player.texture.loadFromFile("Assets\\fish.png");
    player.sprite.setTexture(player.texture);
    player.sprite.setPosition(320, 450);
    player.sprite.setTextureRect(IntRect(0, 0, player.frameW, player.frameH));
    player.sprite.setScale(1.0f, 1.0f);


    score.font.loadFromFile("Assets\\font.TTF");
    for (int q = 0; q < 7; q++) {
        score.FRENZYiText[q].setFont(score.font);
        score.FRENZYiText[q].setPosition(200 + 20 * q, 200);
        score.FRENZYiText[q].setString(string(1, score.word[q]));
        score.FRENZYiText[q].setCharacterSize(40);
    }
    score.scoretext.setFont(score.font);
    score.scoretext.setPosition(300, 300);
    score.scoretext.setString(to_string(score.score));
    score.scoretext.setCharacterSize(30);


    sorry.textures[0].loadFromFile("Assets\\s.jpeg");
    sorry.textures[1].loadFromFile("Assets\\o.jpeg");
    sorry.textures[2].loadFromFile("Assets\\r1.jpeg");
    sorry.textures[3].loadFromFile("Assets\\r2.jpeg");
    sorry.textures[4].loadFromFile("Assets\\y.jpeg");

    for (int w = 0; w < 5; w++) {
        sorry.sprites[w].setTexture(sorry.textures[w]);
        sorry.sprites[w].setPosition(80, 200 + 10 * w);
    }


	Continue.CONTtexture.loadFromFile("Assets\\CONT.png");
	Continue.CONTsprite.setTexture(Continue.CONTtexture);
	Continue.YESt[0].loadFromFile("Assets\\nonactive yes.png");
	Continue.YESt[1].loadFromFile("Assets\\active yes.png");
	Continue.YESsprite[0].setTexture(Continue.YESt[0]);
	Continue.YESsprite[1].setTexture(Continue.YESt[1]);
	Continue.YESsprite[0].setPosition(130, 600);
	Continue.YESsprite[1].setPosition(130, 600);
	Continue.NOt[0].loadFromFile("Assets\\nonactive no.png");
	Continue.NOt[1].loadFromFile("Assets\\active no.png");
	Continue.NOsprite[0].setTexture(Continue.NOt[0]);
	Continue.NOsprite[1].setTexture(Continue.NOt[1]);
	Continue.NOsprite[0].setPosition(160, 600);
	Continue.NOsprite[1].setPosition(160, 600);
    Continue.ContinuesLeft.setFont(score.font);
	Continue.ContinuesLeft.setPosition(100, 650);
	Continue.ContinuesLeft.setString("continues left: " + to_string(score.trials));
    return 0;
}

int scoring(Shark &shark, float dt) {
    int greenCount = 0, redCount = 0;
    score.timer += dt;

    if (player.ate) {
        if (score.timer < 1) {
            if (score.currentFrenzy < 7) {
                score.FRENZYi[score.currentFrenzy]++;
                score.currentFrenzy++;
            }
        }
        score.lessThanSec = 1;
        score.timer = 0;
        player.ate = 0;
    }

    if (player.aten) {
        player.dead = 1;
        score.lessThanSec = 0;
        //dead and back
        for (int q = 0; q < 7; q++) {
            score.FRENZYi[q] = 0;
            score.FRENZYiText[q].setFillColor(Color::White);
        }
        dead();
        player.aten = 0;
    }

    if (score.lives <= 0)
        score.lives = 0;

    if (score.timer >= 1) {
        if (score.currentFrenzy > 0) {
            score.currentFrenzy--;
            if (score.FRENZYi[score.currentFrenzy] > 0) {
                score.FRENZYi[score.currentFrenzy]--;
            }
        }
        score.timer = 0;
    }

    if (shark.tailBite)         score.score += score.taileBscore * score.bouns;

    if (preal.aten)         score.score += score.prealscore * score.bouns;


    for (int q = 0; q < 7; q++) {
        if (score.FRENZYi[q] <= 0) {
            score.FRENZYi[q] = 0;
            score.FRENZYiText[q].setFillColor(Color::White);
        }
        else if (score.FRENZYi[q] == 1) {
            greenCount++;
            score.FRENZYiText[q].setFillColor(Color::Green);
        }
        else if (score.FRENZYi[q] >= 2) {
            score.FRENZYi[q] = 2;
            redCount++;
            score.FRENZYiText[q].setFillColor(Color::Red);
        }
    }
    if (greenCount == 7 && score.bouns < 2) {
        score.bouns = 2;
    }

    if (redCount == 7 && score.bouns < 3) {
        score.bouns = 3;
    }

    if (score.scale >= 30) {
        score.medFishCanBeAten = 1;
    }
    if (score.scale >= 80) {
        score.bigFishCanBeAten = 1;
    }

    if (score.score >= 300 * score.livelnum) {
        player.size = 55;
    }
    else if (score.score >= 1200 * score.livelnum) {
        player.size = 35;
    }

    score.scoretext.setString(to_string(score.score));

    return score.score;
}

void playermovement(float dt, fishes& fish, Sprite& sprite, Texture& t, RenderWindow& window) {
    Vector2f target = window.mapPixelToCoords(Mouse::getPosition(window));
    Vector2f direction = target - fish.sprite.getPosition();

    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 5) {
        direction /= length;
        if (direction.x > 0) {
            fish.currentframe = 1;
        }
        else if (direction.x < 0) {
            fish.currentframe = 0;
        }

        fish.sprite.setTextureRect(IntRect(fish.currentframe * fish.frameW, 0, fish.frameW, fish.frameH));
        fish.sprite.move(direction.x * score.speedP * dt, direction.y * score.speedP * dt);
    }
}

void intersection() {
    if (player.dead) return;
}

int draw(RenderWindow& window, float dt) {

    if (player.draw) window.draw(player.sprite);
    if (smallFish.draw) window.draw(smallFish.sprite);
    if (medFish.draw) window.draw(medFish.sprite);
    if (bigFish.draw) window.draw(bigFish.sprite);
    window.draw(score.scoretext);

    for (int q = 0; q < 7; q++) {
        window.draw(score.FRENZYiText[q]);
    }

    Sorry(window, dt);
    ContinueY_N(window);

    return 0;
}

void dead() {
    player.draw = 0;

    player.sprite.setPosition(200, -100);

    if (score.lives > 0) {
        score.lives--;
        sorry.active = 1;
        sorry.timer = 0;
        // Don't call Sorry()
    }
    else {
        score.lives = 0;
        score.trials--;
        Continue.showen = 1;
        //screen off
        // Don't call ContinueY_N()

    }

    if (score.trials <= 0) {
        score.lives = 0;
        //gameover
    }
}

void Sorry(RenderWindow& window, float dt) {

    if (!sorry.active) return;

    sorry.timer += dt;

    if (sorry.timer < 2) {
        player.draw = 0;
        for (int w = 0; w < 5; w++) {
            window.draw(sorry.sprites[w]);
        }
    }
    else {

        player.dead = 0;
        player.draw = 1;
        if (player.sprite.getPosition().y < 250) {
            player.sprite.move(0, 350 * dt);
        }
        else {
            player.aten = 0;//--------------------------
            sorry.timer = 0;
            sorry.active = 0;
        }
        return;
    }
}

void ContinueY_N(RenderWindow& window) {
    if (!Continue.showen) return;
    //change screen
    
    Vector2f mouse = window.mapPixelToCoords(Mouse::getPosition(window));
    bool hoverYES = Continue.YESsprite[0].getGlobalBounds().contains(mouse);
    bool hoverNO = Continue.NOsprite[0].getGlobalBounds().contains(mouse);
    
    window.draw(hoverYES ? Continue.YESsprite[1] : Continue.YESsprite[0]);
    window.draw(hoverNO ? Continue.NOsprite[1] : Continue.NOsprite[0]);

    window.draw(Continue.ContinuesLeft);

    if (Mouse::isButtonPressed(Mouse::Left)) {
        int mouseX = Mouse::getPosition(window).x;
        int mouseY = Mouse::getPosition(window).y;

        if (Continue.YESsprite[0].getGlobalBounds().contains(mouseX, mouseY)) {
            //player comes in again
            //trials--;                             
            score.lives = 3;
            player.sprite.setPosition(200, -100);
            player.draw = 1;
            player.dead = 0;
            player.aten = 0;
            Continue.showen = 0;
        }
        else if (Continue.NOsprite[0].getGlobalBounds().contains(mouseX, mouseY)) {
            //gameover
            window.close();
        }
    }
}

int mouthintersect(fishes& a, fishes& b) {
    if (&a == &b) return 0;

    fishes& bigger = (a.size > b.size) ? a : b;
    fishes& smaller = (a.size > b.size) ? b : a;

    if (smaller.aten) return 0;

    Vector2f mouth = Mouthposition(bigger);
    Vector2f targetPos = smaller.sprite.getPosition();

    float dx = mouth.x - targetPos.x;
    float dy = mouth.y - targetPos.y;
    float dist = sqrt(dx * dx + dy * dy);

    float radius = smaller.frameW / 2;

    if (dist <= radius) {
        // bigger fish eats smaller one

        bigger.ate = 1;
        smaller.aten = 1;
        smaller.dead = 1;
        smaller.draw = 0;

        if (player.ate) {
            score.score += smaller.score * score.bouns;
        }

        return 1;
    }
    else if (dist <= radius * 2) {
        // if the smaller fish isnt the player, then it should run away
        return 2;
    }
    else {
        // two fishs are far from each other so nothing happens
        return 0;
    }

}

//MOVEMENT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Mermaid::mermaidMovement(float speed)
{
    Vector2f pos = mermaid.getPosition();
    pos.x -= speed;
    mermaid.setPosition(pos);
}

//ANIMATION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
void Mermaid::mermaidAnimation(int& frame)
{
    int frameWidth = 3011 / 10;
    int frameHeight = 202 / 2;

    int column = frame % 10;
    int row = frame / 10;
    mermaid.setTexture(mermaidTexture);
    mermaid.setTextureRect(IntRect(column * frameWidth, row * frameHeight, frameWidth, frameHeight));
}

void Mermaid::start_mermaid()
{
    mermaidTexture.loadFromFile("Sprites\\fish sprites\\Mermaid.png");

    mermaid.setTexture(mermaidTexture);
    mermaid.setPosition(1440, 200);
    mermaid.setScale(1.7f, 1.7f);

    starTexture.loadFromFile("Sprites\\bonus assets\\starBubble1.PNG");

    for (int i = 0; i < MAX_STARS; i++)
    {
        stars[i].setTexture(starTexture);
        stars[i].setScale(1.2f, 1.2f);
        active[i] = false;
    }
}

void Mermaid::update_mermaid(float dt)
{
    timer += dt;

    if (timer >= 0.1f)
    {
        frame++;
        if (frame >= 20)
            frame = 0;
        timer = 0;
    }

    
    starTimer += dt;

    if (starTimer >= 1.0f)
    {
        for (int i = 0; i < MAX_STARS; i++)
        {
            if (!active[i])
            {
                Vector2f merPos = mermaid.getPosition();

                float tailOffsetX = mermaid.getGlobalBounds().width;
                float tailOffsetY = mermaid.getGlobalBounds().height * 0.5f;

                stars[i].setPosition(merPos.x + tailOffsetX - 100,
                    merPos.y + tailOffsetY);

                active[i] = true;
                break;
            }
        }

        starTimer = 0;
    }

    
    for (int i = 0; i < MAX_STARS; i++)
    {
        if (active[i])
        {
            Vector2f pos = stars[i].getPosition();
            pos.y += 0.1f;
            stars[i].setPosition(pos);

            if (pos.y > 900)
                active[i] = false;
        }
    }

    mermaidMovement(0.17f);
    mermaidAnimation(frame);
}

void Mermaid::draw_mermaid(RenderWindow& window)
{
    window.clear();
    window.draw(mermaid);

    for (int i = 0; i < MAX_STARS; i++)
    {
        if (active[i])
            window.draw(stars[i]);
    }

    window.display();
}

void Shark::start()
{
    if(!signalTexture.loadFromFile("Sprites\\unkown 2\\dangerSign.png")) cout << "danger sign is not found"<< endl;
    signal.setTexture(signalTexture);

    if(!sharkTexture.loadFromFile("Sprites\\fish sprites\\Barracuda.png")) cout << "shark texture is not found"<< endl;
    shark.setTexture(sharkTexture);
    shark.setScale(2.0f, 2.0f);

    fromRight = true;

    if (fromRight) {
        startPos = Vector2f(1440 - signal.getGlobalBounds().width, 300);
    }
    else {
        startPos = Vector2f(0, 300);
    }
    signal.setPosition(startPos);

    if (fromRight) {
        shark.setPosition(1440 + 200, startPos.y);
    }
    else {
        shark.setPosition(-200, startPos.y);
    }
}

void Shark::update(float dt)
{
    sharkEntrance(sharkTimer, flashCount, dt, startPos, sharkReady);

    if (!sharkReady) {
        return;
    }

    if (eating)
    {
        eatTimer += dt;

        if (eatTimer >= 0.1f)
        {
            eatTimer = 0;
            eatFrame++;

            if (eatFrame >= 6)
            {
                eatFrame = 0;
                eating = false;
            }
        }

        sharkAnimation(eatFrame, 0, 6);
        return;
    }

    if (isTurning)
    {
        turnTimer += dt;
        sharkAnimTimer += dt;

        if (sharkAnimTimer >= 0.1f)
        {
            sharkAnimTimer = 0;
            sharkFrame++;
            if (sharkFrame >= 5) sharkFrame = 0;
        }

        sharkAnimation(sharkFrame, 2, 5);

        if (turnTimer >= 0.6f)
        {
            isTurning = false;
            sharkFrame = 0;
            turnTimer = 0;
            fromRight = !fromRight;

            if (fromRight)
                shark.setScale(2.0f, 2.0f);
            else
                shark.setScale(-2.0f, 2.0f);
        }

        return;
    }

    sharkAnimTimer += dt;

    if (sharkAnimTimer >= 0.1f)
    {
        sharkAnimTimer = 0;
        sharkFrame++;

        if (sharkFrame >= 14)
            sharkFrame = 0;
    }

    sharkAnimation(sharkFrame, 1, 14);
    sharkMovement(300.0f, fromRight, dt);
}

void Shark::draw(RenderWindow& window)
{
    window.clear();
    if (!sharkReady) {
        if (showSignal)
            window.draw(signal);
    }
    else {
        window.draw(shark);
    }
    window.display();
}

void Shark::sharkMovement(float speed, bool fromRight, float deltaTime)
{
    Vector2f pos = shark.getPosition();

    if (fromRight) {
        pos.x -= speed * deltaTime;
    }
    else {
        pos.x += speed * deltaTime;
    }
    shark.setPosition(pos);
}

void Shark::sharkEntrance(float& timer, int& flashCount, float deltaTime, Vector2f startPosition, bool& finished)
{
    const int totalFlashes = 6;
    const float totaltime = 4.0f;
    const float everyFlashTimeInterval = totaltime / totalFlashes / 2;
    
    if (finished) {
        return ;
    }
    timer += deltaTime;

    if (flashCount < totalFlashes)
    {
        if (timer >= everyFlashTimeInterval)
        { 
            

            showSignal = !showSignal;
            timer = 0;


            if (!showSignal) {
                flashCount++;
            }
        }
    }
    else
    {
        if (shark.getPosition().x < 0) {
            shark.setPosition(-200, startPosition.y);
        }
        else {
            shark.setPosition(1440 + 200, startPosition.y);
        }
        finished = true;
    }
}

void Shark::sharkAnimation(int frame, int row, int frameCountInEveryRow)
{
    int frameHeight = sharkTexture.getSize().y / 3;


    int baseFrameWidth = sharkTexture.getSize().x / 14;
    int frameWidth = baseFrameWidth;
    int column = frame % frameCountInEveryRow;

    shark.setTextureRect(IntRect(column * frameWidth, row * frameHeight, frameWidth, frameHeight));
    shark.setOrigin(frameWidth / 2, frameHeight / 2);
}

void window_mode(RenderWindow& window, bool fullscreen, Vector2f res) {
    if (fullscreen)
        window.create(VideoMode(res.x, res.y), "Feeding Frenzy 2 - Shipwrech Showdown", Style::Fullscreen);
    else
        window.create(VideoMode(res.x, res.y), "Feeding Frenzy 2 - Shipwrech Showdown");
}