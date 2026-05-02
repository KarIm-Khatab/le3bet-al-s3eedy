#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <SFML/Audio.hpp>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace sf;



int state = 9;
// state values:
// 0  = main menu
// 1  = help & options
// 2  = level map screen
// 3 = lvl 1 text intro
// 4  = level 1 gameplay
// 5 = lvl 2 text intro
// 6  = level 2 gameplay
// 7 = lvl 2 text intro
// 8  = level 3 gameplay
// 9  = end screen (score)

struct Score {
    int score = 0, bouns = 1, smallFscore = 10, medFscore = 20, bigFscore = 50,
        taileBscore = 100, starscore = 60, prealscore = 50,
        scale = 0, lives = 3, trials = 3,
        levelnum = 3;
    short unlocked_levels = 2, completed_levels = unlocked_levels - 1;
    short number_of_small_fish_eaten, number_of_mid_fish_eaten, number_of_large_fish_eaten = 0;
    short foodbank = number_of_small_fish_eaten + number_of_mid_fish_eaten + number_of_large_fish_eaten;
    float dt, timer = 0;

    bool lessThanSec;
    float speedP = 700; //p for player
    float speedE = 100; //E for enemy
    Vector2f prev;
    Texture player;
    Sprite setplayers;

    Font font;
};

struct GameSettings {
    short sound_volume = 100, music_volume = 100;
    bool controls[3] = { 1,1,1 }; // 0 -> sfx, 1-> music, 2->full screen
    Vector2f res = { 1920,1080 };
};
Score score;
GameSettings settings;

struct AnimatedSprite {

    Sprite sprite;

    int frameW = 0, frameH = 0;
    int countframe = 0, currentframe = 0, startframe, totalcols;

    float anitimer = 0;
    float speed = 100;

    void setup(Texture& t, int cols, int rows, int frames, int start_Frame) {

        sprite.setTexture(t);
        startframe = start_Frame;
        totalcols = cols;

        frameW = t.getSize().x / cols;
        frameH = t.getSize().y / rows;

        countframe = frames;
        currentframe = 0;
        anitimer = 0;

        int startframecols = startframe % totalcols;
        int startframerows = startframe / totalcols;

        sprite.setOrigin(frameW / 2, frameH / 2);
        sprite.setTextureRect(IntRect(startframecols * frameW, startframerows * frameH, frameW, frameH));



    }

    void animate(float dt) {

        anitimer += dt;

        if (anitimer > 0.2f * 6 / totalcols) {

            currentframe = (currentframe + 1) % countframe;
            int absframe = startframe + currentframe;
            int framecols = absframe % totalcols;
            int framerows = absframe / totalcols;
            sprite.setTextureRect(IntRect(framecols * frameW, framerows * frameH, frameW, frameH));
            anitimer = 0;

        }

    }

};

struct Shark {
    Texture signalTexture, sharkTexture;
    Sprite signal, shark;

    float sharkAnimTimer = 0, sharkTimer = 0, turnTimer = 0, eatTimer = 0;
    int flashCount = 0, sharkFrame = 0, eatFrame = 0;
    bool fromRight = 1, isTurning = 0, sharkReady = 0, showSignal = 1, eating = 0;
    bool ate, tailBite;
    float dt;

    bool tailTriggered = false;

    Vector2f startPos;

    void start();
    void update(float dt);
    void draw(RenderWindow& window);
    void sharkMovement(float speed, float deltaTime);
    void sharkEntrance(float& timer, int& flashCount, float deltaTime, Vector2f startPosition, bool& finished);
    void sharkAnimation(int frame, int row, int frameCountInEveryRow);

    FloatRect  SharkTailRect();
};

struct commonAssets {
    Font game_font, highlights_font;
    Texture main_menu_background_text, menu_rocks_text, shell_stage_background_text;
    Sprite main_menu_background_sprite, menu_rocks_sprite, shell_stage_background_sprite;

    void load();
};

struct gameSounds {
    SoundBuffer click_buffer, main_theme_buffer;
    Sound click_sound, main_theme_sound;

    void load();

    void Click_sound();

    void play_sound();
};

struct pauseMenu {

    Texture buttons[3][2], stage_Texture, background_Texture;
    Sprite buttons_sprite[3], stageSprite, backgroundSprite;

    RenderWindow* window = nullptr;

    short selectedOption = 0;

    bool pause = true;

    void load();

    void selection();

    void drawBackground();

    void draw();

    void eventHandler(Event& event);

};

struct helpAndOptions {

    struct menuButtons {

        Texture checkbox_texture[2][2];
        Sprite checkbox_sprite[2][2];

        void load_buttons()
        {
            if (!checkbox_texture[0][0].loadFromFile("Sprites\\menu\\option menu check box.png")) cout << "option menu check box texture is not found" << endl;
            if (!checkbox_texture[0][1].loadFromFile("Sprites\\menu\\option menu check box-hoovered.png")) cout << "option menu check box texture is not found" << endl;
            if (!checkbox_texture[1][0].loadFromFile("Sprites\\menu\\marked check box.png")) cout << "option menu marked check box texture is not found" << endl;
            if (!checkbox_texture[1][1].loadFromFile("Sprites\\menu\\marked check box-hoovered.png")) cout << "option menu hoovered marked check box texture is not found" << endl;

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    checkbox_sprite[i][j].setTexture(checkbox_texture[i][j]);
                    checkbox_sprite[i][j].setOrigin(checkbox_sprite[i][j].getGlobalBounds().width / 2, checkbox_sprite[i][j].getGlobalBounds().height / 2);
                    checkbox_sprite[i][j].setScale(1.1f, 1.1f);
                }
            }
        }
    };

    RenderWindow* window = nullptr;
    menuButtons buttons[7];
    Texture buttons_texture[6][2], menu_shell_texture[6], spongebob_text, mouse_texture, reft;
    Sprite buttons_sprite[6][2], menu_shell_sprite[6], spongebob_sprite, mouse_sprite, refs;
    string settings_string[4] = { "Sound:", "Music:", "Full Screen:", "Mouse Speed:" },
        how_to_play_string[2]{ "Welcome to the beautiful Frenzy Coast.\nEnjoy the sights, but don't get too comfortable...\nA fish still needs his lunch, and it's a fish-eat-fish\nworld out there!",
            "- Use your mouse to control Boris.\n- Eat fish that are smaller than you.\n- Avoid anything that's larger than you.\n- Eat enough fish and you'll grow bigger!\n" };

    string namesString[9] = { "Made By", "Amal Mohamed", "Fatma Saaed", "Ghadeer Hamed", "Karim Khattab","Karim Hossam",  "Menna Saad", "Youssef Ayman", "Credits" };

    Text settings_text[4], how_to_play_text[2], namesText[9];
    commonAssets* assets = nullptr;
    gameSounds* sound = nullptr;

    short selected = -1, options_scene = 0; // main-> 0, controls -> 1, how to play -> 3, settings -> 2, credits -> 4

    void load_assets();

    void handle_movements(Event& event);

    void update_menu_scenes();

    void draw_basics();

    void draw();

    void update_settings();

    void draw_settings();

    void draw_controls();

    void draw_how_to_play();

    void draw_credits();
};

struct mainMenu {
    RenderWindow* window = nullptr;

    gameSounds* sound;

    short selected = -1, menu_scene = 0;

    // scenes: 0 -> click to start, 1 -> main menu, 2 -> single player,
    //         3 -> multiplayer, 4 -> leaderboards, 5 -> achievements,
    //         6 -> help, 7 -> quit, 8 -> story mode, 9 -> story lite,
    //         10 -> time attack, 11 -> settings, 12 -> controls,
    //         13 -> how to play, 14 -> credits

    Texture logo_texture, main_menu_buttons_texture[3][2], reft, txt,
        tool_tip_texture, tube_texture, ment, rr,
        single_player_texture[2][2], quads_texture[6], the_sign_texture;

    Sprite main_menu_buttons_sprite[3][2], logo_sprite,
        tool_tip_sprite, tube_sprite, mens, refs, txts, rrs, single_player_sprite[2][2], quads_sprite[6], the_sign_sprite;

    string menu_strings[2] = {
        //        "Single Player Game Modes",
                "Adjust Sound and Music Settings \n\
         Or Learn How to Play",
        "Return to Desktop" };

    string single_player_string[3] = { "Story Game Mode", "Back To Main Menu", "Select An Option to Continue" };

    Text menu_text[3], single_player_text[3];

    Font* menu_font;

    commonAssets* assets = nullptr;

    void load_assets();

    void logo_transformation(char a);

    void handle_movements(Event& event);

    void update_menu_scenes();

    void draw_basics();

    void draw_starting_menu();

    void draw_main_menu();

    void draw_single_player_menu();

    void draw_multiplayer_menu();

    void draw_leaderboards_menu();

    void draw_achievements_menu();

    void draw_help_menu();
};

struct stageComplete{

    Texture shell_texture, buttons_texture[2][2], buttons_text_texture[2][2], level1fish_text[3], level2fish_text[2], level3fish_text[3], reft;
    Sprite shell_sprite, buttons_sprite[2][2], buttons_text_sprite[2][2], level1fish_sprite[3], level2fish_sprite[3], level3fish_sprite[3], refs;
    RenderWindow *window = nullptr;
    commonAssets* assets;
    short selected = -1;
    short* menu_scene = nullptr;

    string level_string[3] = {"Food Bank: " + to_string(score.foodbank),"You've earned the rank of: Newbie Nibbler", "The humphead wrasse is one of the largest reef\nfishes in the world"};
    string titles_string[2] = {"Lunch Report", "Fun Fact"};
    string scores_string[3];// = {to_string(score.number_of_small_fish_eaten), to_string(score.number_of_mid_fish_eaten),to_string(score.number_of_large_fish_eaten)};
    Text level_text[3], titles_text[2], scores_text[3];

    bool scores_updated = false;

    void load()
    {
        if(!shell_texture.loadFromFile("Sprites/menu/stage_complete-shell.png")) cout << "win shell's not found" << endl;
        shell_sprite.setTexture(shell_texture);
        shell_sprite.setOrigin(shell_sprite.getGlobalBounds().width/2,shell_sprite.getGlobalBounds().height/2);
        shell_sprite.setPosition(settings.res.x / 2,settings.res.y/ 2 );

        if(!reft.loadFromFile("Sprites/menu/stagerefpng.png")) cout << "win shell's not found" << endl;
        refs.setTexture(reft);
        refs.setOrigin(refs.getGlobalBounds().width/2,refs.getGlobalBounds().height/2);
        refs.setPosition(settings.res.x / 2,settings.res.y/ 2 );

        if(!level1fish_text[0].loadFromFile("Sprites/Stage complete/Minnow.png")) cout << "Minnow's not found" << endl;
        if(!level1fish_text[1].loadFromFile("Sprites/Stage complete/cod.png")) cout << "cod's not found" << endl;
        if(!level1fish_text[2].loadFromFile("Sprites/Stage complete/Lionfish.png")) cout << "Lionfish's not found" << endl;

        if(!buttons_texture[0][0].loadFromFile("Sprites/Stage complete/newbutton3.png")) cout << "win shell's not found" << endl;
        if(!buttons_texture[0][1].loadFromFile("Sprites/Stage complete/newbutton3glow.png")) cout << "win shell's not found" << endl;
        if(!buttons_texture[1][0].loadFromFile("Sprites/Text screen/shortButtonOff.png")) cout << "win shell's not found" << endl;
        if(!buttons_texture[1][1].loadFromFile("Sprites/Text screen/shortButtonOn.png")) cout << "win shell's not found" << endl;

        if(!buttons_text_texture[0][0].loadFromFile("Sprites/Text screen/continueTextOff.png")) cout << "win shell's not found" << endl;
        if(!buttons_text_texture[0][1].loadFromFile("Sprites/Text screen/continueTextOn.png")) cout << "win shell's not found" << endl;
        if(!buttons_text_texture[1][0].loadFromFile("Sprites/Text screen/quitTextOff.png")) cout << "win shell's not found" << endl;
        if(!buttons_text_texture[1][1].loadFromFile("Sprites/Text screen/quitTextOn.png")) cout << "win shell's not found" << endl;

        for(int i =0; i < 2;i++)
        {
            for(int j =0; j < 2; j++)
            {
                buttons_sprite[i][j].setTexture(buttons_texture[i][j]);
                buttons_sprite[i][j].setOrigin(buttons_sprite[i][j].getGlobalBounds().width/2,buttons_sprite[i][j].getGlobalBounds().height/2);
                if(!i) buttons_sprite[i][j].setPosition(settings.res.x/2, settings.res.y/2 + 425);
                else buttons_sprite[i][j].setPosition(settings.res.x/2 + 400, settings.res.y/2 + 425);
                buttons_sprite[i][j].setScale(2.f,2.f);
                
                buttons_text_sprite[i][j].setTexture(buttons_text_texture[i][j]);
                buttons_text_sprite[i][j].setOrigin(buttons_text_sprite[i][j].getGlobalBounds().width/2,buttons_text_sprite[i][j].getGlobalBounds().height/2);

                if(!i) buttons_text_sprite[i][j].setPosition(settings.res.x/2, settings.res.y/2 + 425);
                else buttons_text_sprite[i][j].setPosition(settings.res.x/2 + 400, settings.res.y/2 + 425);
                if(j) buttons_text_sprite[i][j].setScale(0.4f,0.4f);
                else buttons_text_sprite[i][j].setScale(0.5f,0.5f);
            }
        }

        for(int i =0; i < 3;i++)
        {
            level1fish_sprite[i].setTexture(level1fish_text[i]);
            level1fish_sprite[i].setOrigin(level1fish_sprite[i].getGlobalBounds().width/2,level1fish_sprite[i].getGlobalBounds().height/2);
            level1fish_sprite[i].setPosition(settings.res.x/2 - 140 + i * 140 ,settings.res.y/2 - 170);
        }
        level1fish_sprite[0].setScale(1.2f,1.2f);
        level1fish_sprite[1].setScale(0.4f,0.4f);
        level1fish_sprite[2].setScale(0.55f,0.55f);

        level2fish_sprite[0] = level1fish_sprite[0];
        if (!level2fish_text[1].loadFromFile("Sprites/Stage complete/John Dory.png")) cout << "John Dory's not found" << endl;
        if (!level2fish_text[2].loadFromFile("Sprites/Stage complete/Marlin.png")) cout << "Marlin's not found" << endl;
       

        for (int i = 1; i < 3; i++)
        {
            level2fish_sprite[i].setTexture(level2fish_text[i]);
            level2fish_sprite[i].setOrigin(level2fish_sprite[i].getGlobalBounds().width / 2, level2fish_sprite[i].getGlobalBounds().height / 2);
            level2fish_sprite[i].setPosition(settings.res.x / 2 - 140 + i * 140, settings.res.y / 2 - 170);
        }
        level2fish_sprite[1].setScale(0.6f, 0.6f);
        level2fish_sprite[2].setScale(0.6f, 0.6f);

        if (!level3fish_text[0].loadFromFile("Sprites/Stage complete/pompano.png")) cout << "pompano's not found" << endl;
        if (!level3fish_text[1].loadFromFile("Sprites/Stage complete/tuna.png")) cout << "tuna's not found" << endl;
        if (!level3fish_text[2].loadFromFile("Sprites/Stage complete/shark.png")) cout << "shark's not found" << endl;

        for (int i = 0; i < 3; i++)
        {
            level3fish_sprite[i].setTexture(level3fish_text[i]);
            level3fish_sprite[i].setOrigin(level3fish_sprite[i].getGlobalBounds().width / 2, level3fish_sprite[i].getGlobalBounds().height / 2);
            level3fish_sprite[i].setPosition(settings.res.x / 2 - 140 + i * 140, settings.res.y / 2 - 170);
        }
        level3fish_sprite[0].setScale(0.4f, 0.4f);
        level3fish_sprite[1].setScale(0.6f, 0.6f);
        level3fish_sprite[2].setScale(0.4f, 0.4f);

        for(int i = 0; i < 3; i++)
        {
            level_text[i].setString(level_string[i]);
            level_text[i].setFillColor(Color::White);
            level_text[i].setOutlineColor(Color::Black);
            level_text[i].setOutlineThickness(2);
            level_text[i].setCharacterSize(34);
            level_text[i].setFont(assets->game_font);
            FloatRect bounds = level_text[i].getLocalBounds();
            level_text[i].setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
            level_text[i].setPosition(settings.res.x/2 - 70,settings.res.y/2 + 10 + i * 40);
            if(i == 2)
                continue;
            titles_text[i].setString(titles_string[i]);
            titles_text[i].setFillColor(Color::White);
            titles_text[i].setOutlineColor(Color::Black);
            titles_text[i].setOutlineThickness(0.3f);
            titles_text[i].setCharacterSize(43);
            titles_text[i].setFont(assets->highlights_font);
            titles_text[i].setLetterSpacing(1.8f);
            bounds = titles_text[i].getLocalBounds();
            titles_text[i].setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
            titles_text[i].setPosition(settings.res.x/2 ,settings.res.y/2 - 270 + i * 385);
        }
        level_text[2].setPosition(settings.res.x/2 ,settings.res.y/2 + 200);
    }

    void update_scores()
    {
        scores_string[0] = to_string(score.number_of_small_fish_eaten);
        scores_string[1] = to_string(score.number_of_mid_fish_eaten);
        scores_string[2] = to_string(score.number_of_large_fish_eaten);
        for(int i =0; i < 3;i++)
        {
            scores_text[i].setString(scores_string[i]);
            scores_text[i].setFillColor(Color::White);
            scores_text[i].setOutlineColor(Color::Black);
            scores_text[i].setOutlineThickness(2);
            scores_text[i].setCharacterSize(34);
            scores_text[i].setFont(assets->game_font);
            FloatRect bounds = scores_text[i].getLocalBounds();
            scores_text[i].setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
            scores_text[i].setPosition(settings.res.x/2 - 140 + i * 136 ,settings.res.y/2 - 100);
        }
        scores_updated = true;
    }

    void handle_movements(Event& event)
    {
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Escape)
                state = 2, scores_updated = false;
        }
        if (event.type == Event::MouseMoved)
        {
            Vector2i mousePixel = Mouse::getPosition(*window);
            Vector2f mouse_position = window->mapPixelToCoords(mousePixel);
            for (int i = 0; i < 2; i++)
            {
                if (buttons_sprite[i][0].getGlobalBounds().contains(mouse_position))
                {
                    selected = i;
                    break;
                }
                else
                    selected = -1;
            }
        }
        if (event.type == Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == Mouse::Left)
            {
                if (selected == 0)
                    state = 2, scores_updated = false;
                else
                {
                    // if (menu_scene) 
                    //     *menu_scene = 2;
                    state = 0, scores_updated = false;
                }
            }
        }
    }

    void draw()
    {
        if(!scores_updated) update_scores();
        window->clear();
        window->draw(assets->main_menu_background_sprite);
        window->draw(assets->shell_stage_background_sprite);
        //window->draw(refs);
        window->draw(shell_sprite);

        for(int i = 0; i < 2; i++)
        {
            window->draw(buttons_sprite[i][selected == i ? 1 : 0]);
            window->draw(buttons_text_sprite[i][selected == i ? 1 : 0]);
        }

        for(int i =0; i < 2; i++)
            window->draw(titles_text[i]);

        if(score.levelnum == 1)
            for(int i =0;i < 3; i++)
                window->draw(level1fish_sprite[i]);
        else if (score.levelnum == 2)
            for (int i = 0; i < 3; i++)
                window->draw(level2fish_sprite[i]);
        else
            for (int i = 0; i < 3; i++)
                window->draw(level3fish_sprite[i]);

        for(int i =0; i < 3; i++)
        {
            window->draw(level_text[i]);
            window->draw(scores_text[i]);
        }
        
        window->display();
    }
};

struct Mermaid {
    static constexpr int MAX_STARS = 30; // Make MAX_STARS static constexpr

    Texture mermaidTexture, starTexture;
    Sprite mermaid, stars[MAX_STARS];

    int frame = 0;

    float timer = 0;
    float starTimer = 0;

    bool active[MAX_STARS];

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
    Text ContinuesLeft;
    Texture CONTtexture, YESt[2], NOt[2];
    Sprite CONTsprite, YESsprite[2], NOsprite[2]; //where 0 is nonactivated and 1 is activated
    bool showen = 0;
};

struct Preal {
    bool aten;
};

struct levelsmap {

    struct Buttons {
        Texture buttons_text[2][2];
        Sprite buttons_sprite[2][2];

        // 0,0 -> msh selected 3ady | 0,1 selected | 1,0 done | 1,1 done w selected
        void load()
        {
            if (!buttons_text[0][0].loadFromFile("Sprites/menu/gameMap_mapPinOffLg.png")) cout << "Error loading refs" << endl;
            if (!buttons_text[0][1].loadFromFile("Sprites/menu/map_menu_hoov.png")) cout << "Error loading refs" << endl;
            if (!buttons_text[1][0].loadFromFile("Sprites/menu/gameMap_noExport.png")) cout << "Error loading refs" << endl;
            if (!buttons_text[1][1].loadFromFile("Sprites/menu/gameMap_mapPinOnLg.png")) cout << "Error loading refs" << endl;

            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                {
                    buttons_sprite[i][j].setTexture(buttons_text[i][j]);
                    buttons_sprite[i][j].setOrigin(buttons_sprite[i][j].getGlobalBounds().width / 2, buttons_sprite[i][j].getGlobalBounds().height / 2);
                    buttons_sprite[i][j].setScale(1.4f, 1.4f);
                }
        }
    };

    RenderWindow* window = nullptr;
    Texture map_texture, tool_tip_texture, ref;
    Sprite map_sprite, tool_tip_sprite, refs;
    Buttons buttons[3];
    string levels_string[5] = { "1. Sandy Shoal", "2. Angler Ambush","3. Reef Raider", "Click To Select The Level", "Select A Level To Play!" };
    Text levels_text[5];
    Font* font;
    short selected = -1;
    short* menu_scene = nullptr;

    void load()
    {
        if (!ref.loadFromFile("Sprites/menu/mapref.png")) cout << "Error loading refs" << endl;
        refs.setTexture(ref);
        refs.setOrigin(refs.getGlobalBounds().width / 2, refs.getGlobalBounds().height / 2);
        refs.setPosition(settings.res.x / 2, settings.res.y / 2);

        if (!map_texture.loadFromFile("Sprites/menu/mpa-menu1.png")) cout << "Error loading map" << endl;
        map_sprite.setTexture(map_texture);
        map_sprite.setOrigin(map_sprite.getGlobalBounds().width / 2, map_sprite.getGlobalBounds().height / 2);
        map_sprite.setPosition(settings.res.x / 2, settings.res.y / 2);

        for (int i = 0; i < 3; i++)
        {
            buttons[i].load();
            for (int j = 0; j < 2; j++)
            {
                for (int h = 0; h < 2; h++)
                {
                    if (i == 0)
                        buttons[i].buttons_sprite[j][h].setPosition(settings.res.x / 2 - 247, settings.res.y / 2 - 305.5f);
                    else if (i == 1)
                        buttons[i].buttons_sprite[j][h].setPosition(settings.res.x / 2 - 500, settings.res.y / 2 + 25);
                    else
                        buttons[i].buttons_sprite[j][h].setPosition(settings.res.x / 2 - 150, settings.res.y / 2 + 70);
                }
            }
        }

        for (int i = 0; i < 5; i++)
        {
            levels_text[i].setString(levels_string[i]);
            levels_text[i].setFont(*font);
            levels_text[i].setFillColor(Color::White);
            levels_text[i].setOutlineColor(Color::Black);
            levels_text[i].setCharacterSize(36);
            levels_text[i].setOutlineThickness(2);
            levels_text[i].setOrigin(levels_text[i].getGlobalBounds().width / 2, levels_text[i].getGlobalBounds().height / 2);
            levels_text[i].setPosition(settings.res.x / 2 + 20, settings.res.y / 2 + 380);
            if (i == 3)
            {
                levels_text[i].setPosition(settings.res.x / 2 + 20, settings.res.y / 2 + 320);
                levels_text[i].setCharacterSize(32);
            }
        }
        levels_text[4].setPosition(settings.res.x / 2 + 20, settings.res.y / 2 + 320);
    }

    void handle_movements(Event& event)
    {
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Escape)
            {
                state = 0;
                if (menu_scene) *menu_scene = 2;
            }
        }
        if (event.type == Event::MouseMoved)
        {
            Vector2i mousePixel = Mouse::getPosition(*window);
            Vector2f mouse_position = window->mapPixelToCoords(mousePixel);
            for (int i = 0; i < 3; i++)
            {
                if (buttons[i].buttons_sprite[0][0].getGlobalBounds().contains(mouse_position))
                {
                    if (i >= score.unlocked_levels)
                        break;
                    selected = i;
                    break;
                }
                else
                    selected = -1;
            }
        }
        if (event.type == Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == Mouse::Left)
            {
                if (selected == 0)
                    state = 3;
                else if (selected == 1)
                    state = 6;
                else
                    state = 8;
            }
        }
    }

    void draw()
    {
        window->clear();
        //window->draw(refs);
        window->draw(map_sprite);
        for (int i = 0; i < 3; i++)
            window->draw(buttons[i].buttons_sprite[score.completed_levels == i + 1 ? 1 : 0][selected == i ? 1 : 0]);
        if (selected >= 0)
        {
            window->draw(levels_text[selected]);
            window->draw(levels_text[3]);
        }
        else
            window->draw(levels_text[4]);
        window->display();
    }
};

struct menuTextBeforeLevels {

    bool leftMouseClicked = false;
    bool inTextMenuLevel1 = false;
    bool inTextMenuLevel2 = false;
    bool inTextMenuLevel3 = false;
    bool inTextMenuLevel4 = false;
    bool isOnContinue = false;
    bool isOnQuit = false;



    Texture textMenuBorderTexture, textMenuBackgroundTexture, longButtonOffTexture, longButtonOnTexture, shortButtonOffTexture, shortButtonOnTexture, continueTextOffTexture, continueTextOnTexture, quitTextOffTexture, quitTextOnTexture;
    Texture borisTexture, orangeFishTexture, blueFishTexture, brownFishTexture, barracudaTexture, humpheadTexture, eddieTexture, johnDorryTexture, orvilleTexture, mineTexture;

    Sprite textMenuBorderSprite, textMenuBackgroundSprite, longButtonOffSprite, longButtonOnSprite, shortButtonOffSprite, shortButtonOnSprite, continueTextOffSprite, continueTextOnSprite, quitTextOffSprite, quitTextOnSprite;
    Sprite borisSprite, orangeFishSprite, blueFishSprite, brownFishSprite, barracudaSprite, humpheadSprite, eddieSprite, johnDorrySprite, orvilleSprite, mineSprite;

    Font* infoFont;
    Sprite* background_sprite;

    Text level1Text, level1BorisText1, level1BorisText2, level1BorisText3, level1BorisText4, level1BorisText5, level1BorisText6;
    Text howToPlayTitle, howToPlayText1, howToPlayText2, howToPlayText3, howToPlayText4, tip1Text;

    Text level2Text, level2BarracudaTitle, level2BarracudaText1, level2BarracudaText2, level2BarracudaText3, level2BarracudaText4, level2BarracudaText5;
    Text level2HumpheadText1, level2HumpheadText2, level2HumpheadText3, level2HumpheadText4, tip2Text;

    Text level3Text, level3EddieText1, level3EddieText2, level3EddieText3, level3EddieText4;
    Text level3JohnDorryText1, level3JohnDorryText2, level3JohnDorryText3, level3JohnDorryText4, level3JohnDorryText5, level3JohnDorryText6, level3JohnDorryText7, tip3Text;

    Text level4Text, level4OrvilleText1, level4OrvilleText2, level4OrvilleText3, level4OrvilleText4, level4OrvilleText5, level4OrvilleText6, tip4Text;
    Text level4MineTitle, level4MineText1, level4MineText2, level4MineText3;

    void textMenuIntialization(RenderWindow& window);

    void textMenuLevel1(RenderWindow& window);

    void textMenuLevel2(RenderWindow& window);

    void textMenuLevel3(RenderWindow& window);

    void textMenuLevel4(RenderWindow& window);

    void textMenucontrols(RenderWindow& window, Event& event);

};

void setback(Sprite& sp, Texture& t) { // to make background suit with window
    sp.setTexture(t);
    sp.setScale(
        2000.f / t.getSize().x,
        1200.f / t.getSize().y
    );

}


// structs fishes & player


//struct setplayer //of menna {
//    Texture texture;
//    Sprite sprite;
//    bool ate = false;
//    bool dead = 0;
//    bool aten = false;
//    bool draw = true;
//    bool isalive = true;
//    float dir = 1;
//    int size = 0;
//    float speedP = 100;
//    int frameW, frameH;
//    float playerscale = 0.6;
//    int frames, countframe, currentframe;
//    float anitimer;
//    float speed;
//    bool iseating = false;
//    float eatT = 0;
//    setplayer() { // to deatach each level from the other
//        frameW = 0, frameH = 0, frames = 0, anitimer = 0, countframe = currentframe = 0, speed = 100;
//
//    }
//    setplayer(Texture& t, int f, int W, int H) {
//
//
//        countframe = f;
//        currentframe = 0;
//        anitimer = 0;
//        frameW = W;
//        frameH = H;
//        sprite.setTexture(t);
//        sprite.setOrigin(frameW / 2, frameH / 2);
//
//    }
//
//
//
//};

struct setplayer //of hossam
{
    AnimatedSprite anim;
    AnimatedSprite idle, swim, eat, turn;

    bool ate = false;
    bool dead = 0;
    bool aten = false;
    bool draw = true;
    bool isalive = true;
    float dir = 1;
    int size = 0;
    int frameW, frameH;
    float playerscale = 0.6;
    int frames, countframe, currentframe;
    float anitimer;
    float speedP;
    bool iseating = false;
    float eatT = 0;
    int col, rows, eatFrames, swimFrames, idleFrames, turnFrames, eatstart_frame, idlestart_frame, swimstart_frame, turnstart_frame;

    void setupAnimations(Texture& t) {
        frameW = t.getSize().x / col;
        frameH = t.getSize().y / rows;
        eat.setup(t, col, rows, eatFrames, eatstart_frame);
        idle.setup(t, col, rows, idleFrames, idlestart_frame);
        swim.setup(t, col, rows, swimFrames, swimstart_frame);
        turn.setup(t, col, rows, turnFrames, turnstart_frame);
        anim = idle;
    }


};

struct UpperBar {
    Texture growth_texture, growthBorder_texture, menuOfFish_texture[3], score_texture, lives_texture, FRENZYborder_texture;
    Sprite growth_sprite, growthBorder_sprite[2], menuOfFish_sprite[3], score_sprite, lives_sprite, FRENZYborder_sprite;
    RectangleShape growthBar;
    Text FRENZYiText[7], scoretext, livesText;
    string word = "FRENZY!";
    int FRENZYi[7]{ 0,0,0,0,0,0,0 }, currentFrenzy = 0, i = 0; //for counting frenzy word
    //if current=0 it will be white else it will be green one by one if player eat in less than one second if player still plying with same speed 1111 will be 22222 and the word will be colored in red
    Font font;
    float frenzyTimer = 0;
    bool lessThanSec = false;

    void start(int lives);
    void update(float dt, Score& score);
    void draw(RenderWindow& window);

};

struct fishes {

    AnimatedSprite anim;
    AnimatedSprite swim, eat, turn;

    bool aten = false, ate = false, playerintersectfish = false, draw = true, isalive = true;

    float endx = 0, direction = 1, basespeed = 100;

    float basescale = 1.f;
    float changeDir = 0.f;
    bool iseating = false;
    float eatT = 0.f;
    float speed;
    enum FishType { small, mid, big } type;
    int size() {
        if (type == small) return 10;
        else if (type == mid) return 20;
        else return 30;
    }
    int score() {
        if (type == small) return 10;
        else if (type == mid) return 20;
        else return 30;
    }
    int col, rows, eatFrames, swimFrames, turnFrames, eatstart_Frame, swimstart_frame, turnstart_frame;
    void setup(Texture& t) {

        swim.setup(t, col, rows, swimFrames, swimstart_frame);
        eat.setup(t, col, rows, eatFrames, eatstart_Frame);
        turn.setup(t, col, rows, turnFrames, turnstart_frame);
        isalive = true;
        endx = rand() % 1300 + 300;
        anim = swim;

    }


};

struct assets
{
    Texture playerTex;
    Texture smallTex;
    Texture medTex;
    Texture bigTex;
    Texture backgroundTex;
    Texture sharkTex;
    Texture sharkSignalTex;
    Sprite background;
    Texture mermaidTex;
};

Cont Continue;
SORRY sorry;
Mermaid mermaid;
Shark shark;
//setplayer player;
Preal preal;
//fishes enemy[100];
UpperBar bar;

pauseMenu* pause = new pauseMenu;
FloatRect tail = shark.SharkTailRect();
//___________________________________________________________________________________

Vector2f Mouthposition(setplayer& player);

Vector2f enemyMouthPosition(fishes& fish);

void window_mode(RenderWindow& window, bool full_screen);

void cameraMovement(View& camera, Vector2f playerPosition, float BGWidth, float BGHeight);

void playermovement(float dt, setplayer& isplayer, RenderWindow& window);

//void playermovement(float dt, fishes& fish, Sprite& sprite, Texture& t, RenderWindow& window);

void addfishs(Texture& t, int cols, int rows, const int fishNumber, fishes arr[], float scaleValue, fishes::FishType type, int SwimFrames,
    int Swimstart_frame, int EatFrames, int Eatstart_Frame, int TurnFrames, int Turnstart_frame);

void avoiding(fishes& fish, fishes arr[], int n, float dt);

void Fishmovement(float dt, fishes& fish, int rowindex, setplayer& player);

void setback(Sprite& sp, Texture& t);

bool isfront(setplayer& player, fishes& enemy);

bool enemyisfront(setplayer& player, fishes& enemy);

bool enemymouthintersect(setplayer& player, fishes& enemy);

bool mouthintersect(setplayer& player, fishes& enemy);

//void intersection(setplayer& player, fishes& smallFish);

int scoring(Shark& shark, float dt, Preal& preal, setplayer& player);

void dead(setplayer& player, SORRY& sorry, Cont& Continue);

void Sorry(setplayer& player, RenderWindow& window, float dt);

void ContinueY_N(RenderWindow& window, setplayer& player);

void resetgames(RenderWindow& window, bool intialized, bool sharkInLevel, assets levelAssets,
    fishes smallFishes[], fishes mediumFishes[], fishes bigFishes[],
    setplayer& player, int smallFishesNumber, int mediumFishesNumber,
    int bigFishesNumber);

int draw(setplayer& player, RenderWindow& window, float dt);

FloatRect Shark::SharkTailRect()
{
    FloatRect bounds = shark.getGlobalBounds();

    float tailWidth = bounds.width * 0.25f;
    float height = bounds.height * 0.6f;

    float x;

    if (fromRight)
        x = bounds.left + bounds.width - tailWidth;
    else
        x = bounds.left;

    float y = bounds.top + bounds.height * 0.25f;

    return FloatRect(x, y, tailWidth, height);
}


//____________________________________________________________

//Movement and interaction functions


void addfishs(Texture& t, int cols, int rows, const int fishNumber, fishes arr[], float scaleValue, fishes::FishType type, int SwimFrames,
    int Swimstart_frame, int EatFrames, int Eatstart_Frame, int TurnFrames, int Turnstart_frame)
{

    for (int i = 0; i < fishNumber; i++) {
        arr[i].col = cols;
        arr[i].rows = rows;
        arr[i].swimFrames = SwimFrames;
        arr[i].swimstart_frame = Swimstart_frame;
        arr[i].eatFrames = EatFrames;
        arr[i].eatstart_Frame = Eatstart_Frame;
        arr[i].turnFrames = TurnFrames;
        arr[i].turnstart_frame = Turnstart_frame;
        arr[i].setup(t);

        if (type == fishes::small)
            arr[i].anim.sprite.setPosition(rand() % 300, 200 + rand() % 700);

        else if (type == fishes::mid)
            arr[i].anim.sprite.setPosition(300 + rand() % 1000, 150 + rand() % 800);

        else
            arr[i].anim.sprite.setPosition(
                (rand() % 2 == 0 ? rand() % 200 : 1600 + rand() % 200),
                100 + rand() % 900
            );


        if (type == fishes::small)
            arr[i].speed = rand() % 100 + 120;
        else if (type == fishes::mid)
            arr[i].speed = rand() % 80 + 100;
        else
            arr[i].speed = rand() % 80 + 120;

        arr[i].basespeed = arr[i].speed;

        arr[i].basespeed = arr[i].speed;

        arr[i].basescale = scaleValue;
        arr[i].anim.sprite.setScale(scaleValue, scaleValue);

        arr[i].type = type;
    }
}

void avoidOverlap(fishes& fish, fishes arr[], int n, float dt) {

    for (int i = 0; i < n; i++) {
        if (&fish == &arr[i]) continue;

        float dx = fish.anim.sprite.getPosition().x - arr[i].anim.sprite.getPosition().x;
        float dy = fish.anim.sprite.getPosition().y - arr[i].anim.sprite.getPosition().y;

        float dist = sqrt(dx * dx + dy * dy);

        if (dist < 60 && dist > 0.1f) {

            float pushForce;

            if (fish.type == fishes::big)
                pushForce = 20.f;
            else
                pushForce = 50.f;

            fish.anim.sprite.move((dx / dist) * pushForce * dt,
                (dy / dist) * pushForce * dt);
        }
    }
}
void FishMovement(fishes& fish, float dt, setplayer& player, fishes small[], int smallN)
{
    float speedMul = 1.f;

    if (fish.type == fishes::small)
        speedMul = 1.3f;
    else if (fish.type == fishes::mid)
        speedMul = 1.f;
    else if (fish.type == fishes::big)
        speedMul = 0.7f;

    float dis = player.anim.sprite.getPosition().x - fish.anim.sprite.getPosition().x;

    fish.changeDir -= dt;


    if (fish.type == fishes::small)
    {
        if (abs(dis) < 150 && fish.changeDir <= 0)
        {
            if (dis > 0) {
                fish.direction = -1;
            }
            else
                fish.direction = 1;
            fish.changeDir = 0.5f;
        }
    }

    fish.anim.sprite.move(fish.speed * fish.direction * speedMul * dt, 0);

    float x = fish.basescale;
    float y = fish.basescale;

    if (fish.direction == 1)
        fish.anim.sprite.setScale(-x, y);
    else
        fish.anim.sprite.setScale(x, y);

    if (fish.anim.sprite.getPosition().x > 1400)
        fish.direction = -1;

    if (fish.anim.sprite.getPosition().x < 200)
        fish.direction = 1;


    avoidOverlap(fish, small, smallN, dt);
    // aninamted sprite struct should use here insted of Frames function below bec it handel only 2 frames
}

//void updateFishFrame(fishes& fish, int rowindex) {
//
//    int frame = 0;
//
//    if (fish.iseating)
//        frame = 1;
//    else
//        frame = 0;
//
//    fish.sprite.setTextureRect(IntRect(
//        frame * fish.frameW,
//        rowindex * fish.frameH,
//        fish.frameW,
//        fish.frameH
//    ));
//}
//------------------------------------------------------------


// you can use assets struct or make your own struct for your level



void loadAssets(assets& a,
    string back,
    string playert,
    string smallt,
    string midt,
    string bigt, string sharkt, string sharkSignalt, string mermaidt)
{
    a.backgroundTex.loadFromFile(back);
    a.playerTex.loadFromFile(playert);
    a.smallTex.loadFromFile(smallt);
    a.medTex.loadFromFile(midt);
    a.bigTex.loadFromFile(bigt);
    a.sharkTex.loadFromFile(sharkt);
    a.sharkSignalTex.loadFromFile(sharkSignalt);
    a.mermaidTex.loadFromFile(mermaidt);
    a.background.setTexture(a.backgroundTex);
    setback(a.background, a.backgroundTex);
}

void setupLevel(int levelnum,
    assets& a,
    fishes smallFishes[], fishes medFishes[], fishes bigFishes[],
    setplayer& player, Shark& shark, Mermaid& mermaid,
    int cols, int rows, int eatFrames, int idleFrames, int swimFrames, int turnFrames,
    int eatStart, int idleStart, int swimStart, int turnStart, int smallFishcols,
    int smallFishRows, int smallFishEatFrames, int smallFishSwimFrames, int smallFishTurnFrames,
    int smallFishEatStart, int smallFishSwimStart, int smallFishTurnStart, int mediumFishcols,
    int mediumFishRows, int mediumFishEatFrames, int mediumFishSwimFrames, int mediumFishTurnFrames,
    int mediumFishEatStart, int mediumFishSwimStart, int mediumFishTurnStart, int bigFishcols,
    int bigFishRows, int bigFishEatFrames, int bigFishSwimFrames, int bigFishTurnFrames,
    int bigFishEatStart, int bigFishSwimStart, int bigFishTurnStart)
{
    int smallN = 0, medN = 0, bigN = 0;
    float playerScale = 1.f;
    bool useshark = false, usemermaid = false;

    switch (levelnum)
    {
    case 1:
        smallN = 10;
        medN = 5;
        bigN = 3;
        usemermaid = true;
        break;

    case 2:
        smallN = 15;
        medN = 8;
        bigN = 3;
        useshark = true;
        usemermaid = true;
        break;

    case 3:
        smallN = 20;
        medN = 15;
        bigN = 5;
        useshark = true;
        usemermaid = true;
        break;
    }

    /*player = setplayer(a.playerTex, 2,
         a.playerTex.getSize().x / 2,
         a.playerTex.getSize().y);*/

    player.col = cols; player.rows = rows; // علي حسب الاسبريت اللي معاك
    player.swimFrames = swimFrames;
    player.swimstart_frame = swimStart; // نفس الكلام
    player.eatFrames = eatFrames;
    player.eatstart_frame = eatStart;
    player.idleFrames = idleFrames;
    player.idlestart_frame = idleStart;
    player.turnFrames = turnFrames;
    player.turnstart_frame = turnStart;

    player.frameW = a.playerTex.getSize().x / player.col;
    player.frameH = a.playerTex.getSize().y / player.rows;
    player.setupAnimations(a.playerTex);

    player.playerscale = playerScale;
    player.anim.sprite.setPosition(400, 400);
    player.dir = 1;
    player.anim.sprite.setScale(-player.playerscale, player.playerscale);

    player.speedP = score.speedP;

    if (smallN > 0)
        addfishs(a.smallTex, smallFishcols, smallFishRows, smallN, smallFishes, 0.7f,
            fishes::small, smallFishSwimFrames, smallFishSwimStart, smallFishEatFrames,
            smallFishEatStart, smallFishTurnFrames, smallFishTurnStart);

    if (medN > 0)
        addfishs(a.medTex, mediumFishcols, mediumFishRows, medN, medFishes, 1.2f,
            fishes::mid, mediumFishSwimFrames, mediumFishSwimStart, mediumFishEatFrames,
            mediumFishEatStart, mediumFishTurnFrames, mediumFishTurnStart);

    if (bigN > 0)
        addfishs(a.bigTex, bigFishcols, bigFishRows, bigN, bigFishes, 1.4f,
            fishes::big, bigFishSwimFrames, bigFishSwimStart, bigFishEatFrames, bigFishEatStart,
            bigFishTurnFrames, bigFishTurnStart);

    if (useshark) {
        shark.sharkTexture = a.sharkTex;
        shark.signalTexture = a.sharkSignalTex;
        shark.start();
    }
    if (usemermaid) {
        mermaid.mermaidTexture = a.mermaidTex;
        mermaid.start_mermaid();
    }
}


//---------------------------------------------------------------------

Vector2f enemyMouthPosition(fishes& fish) {

    Vector2f pos = fish.anim.sprite.getPosition();
    float offset = fish.anim.frameW * 0.5f * fish.basescale;

    float dir = (fish.anim.sprite.getScale().x > 0) ? 1.f : -1.f;

    return Vector2f(pos.x + offset * dir, pos.y);
}

void whoEatsWho(setplayer& player, fishes& enemy) {
    Vector2f playerMouth = Mouthposition(player);
    Vector2f enemyMouth = enemyMouthPosition(enemy);
    Vector2f playerPos = player.anim.sprite.getPosition();
    Vector2f enemyPos = enemy.anim.sprite.getPosition();

    float enemyDist = sqrt((enemyMouth.x - playerPos.x) * (enemyMouth.x - playerPos.x) + (enemyMouth.y - playerPos.y) * (enemyMouth.y - playerPos.y));
    float playerDist = sqrt((playerMouth.x - enemyPos.x) * (playerMouth.x - enemyPos.x) + (playerMouth.y - enemyPos.y) * (playerMouth.y - enemyPos.y));

    float  playerRadius = player.frameW / 2;
    float  enemyRadius = enemy.anim.frameW * enemy.basescale / 2;

    if (playerDist < enemyRadius) {
        if (player.size > enemy.size()) {
            enemy.aten = 1;
            enemy.draw = 0;
            player.iseating = true;
            player.eatT = .2f;
            player.ate = 1;
            score.score += enemy.score() * score.bouns;

        }
    }
    else if (enemyDist < playerRadius) {
        if (enemy.size() > player.size) {
            player.aten = 1;
            enemy.iseating = true;
            enemy.eatT = .2f;
            enemy.ate = 1;
        }
    }
}

bool enemyisfront(setplayer& player, fishes& enemy) {

    float dx = enemy.anim.sprite.getPosition().x - player.anim.sprite.getPosition().x;

    bool facingRight;

    if (player.anim.sprite.getScale().x < 0)
        facingRight = true;
    else
        facingRight = false;

    if (facingRight)
        return dx > 0;
    else
        return dx < 0;
}

Vector2f Mouthposition(setplayer& player) {
    Vector2f pos = player.anim.sprite.getPosition();
    float offset = player.frameW / 2;
    if (player.dir == 1) return Vector2f(pos.x + offset, pos.y);
    else
        return Vector2f(pos.x - offset, pos.y);
}

bool isfront(setplayer& player, fishes& enemy) {
    float dx = enemy.anim.sprite.getPosition().x - player.anim.sprite.getPosition().x;
    if (player.dir == 1)
        return dx > 0;
    else
        return      dx < 0;
}

bool canPlayerEat(fishes& fish, setplayer& player)
{
    return player.playerscale >= fish.basescale * 1.2f;
}

bool canFishEatPlayer(fishes& fish, setplayer& player)
{
    return fish.basescale >= player.playerscale * 1.2f;
}

void resetFishFish(fishes& fish, int row) {

    fish.iseating = false;
    fish.eatT = 0;

    fish.speed = fish.basespeed;


    fish.direction = (rand() % 2 == 0) ? 1 : -1;

    if (fish.direction == 1)
        fish.anim.sprite.setPosition(rand() % 300, rand() % 700 + 100);
    else
        fish.anim.sprite.setPosition(rand() % 1300 + 500, rand() % 700 + 100);


    if (fish.direction == 1)
        fish.anim.sprite.setScale(-fish.basescale, fish.basescale);
    else
        fish.anim.sprite.setScale(fish.basescale, fish.basescale);


    fish.anim.currentframe = 0;
    fish.anim.sprite.setTextureRect(IntRect(
        0,
        row * fish.anim.frameH,
        fish.anim.frameW,
        fish.anim.frameH
    ));
}

void playermovement(float dt, setplayer& isplayer, RenderWindow& window) {

    Vector2f target = window.mapPixelToCoords(Mouse::getPosition(window));
    Vector2f direction = target - isplayer.anim.sprite.getPosition();

    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 5) {
        direction /= length;
        if (!isplayer.iseating) {
            if (direction.x > 0)
            {
                isplayer.dir = 1;
                isplayer.anim.sprite.setScale(-isplayer.playerscale, isplayer.playerscale);
            }
            else if (direction.x < 0)
            {
                isplayer.dir = -1;
                isplayer.anim.sprite.setScale(isplayer.playerscale, isplayer.playerscale);
            }
        }
        if (!isplayer.iseating) {
            isplayer.anim.sprite.setTextureRect(IntRect(
                0,
                0,
                isplayer.frameW,
                isplayer.frameH
            ));
        }

        isplayer.anim.sprite.move(direction * isplayer.speedP * dt);
    }
}

void resetgames(RenderWindow& window, bool sharkInLevel, assets levelAssets,
    fishes smallFishes[], fishes mediumFishes[], fishes bigFishes[],
    setplayer& player, int smallFishesNumber, int mediumFishesNumber,
    int bigFishesNumber) {

    score.scale = 0;
    score.lives = 3;
    score.timer = 0;
    score.bouns = 1;
    score.lessThanSec = 0;

    if (sharkInLevel) {

        shark.sharkReady = 0;
        shark.eating = 0;
        shark.ate = 0;
        shark.tailBite = 0;
        shark.flashCount = 0;
        shark.sharkTimer = 0;
        shark.showSignal = 1;
        shark.fromRight = 1;

    }


    mermaid.timer = 0;
    mermaid.starTimer = 0;
    mermaid.frame = 0;

    sorry.draw = 0;
    sorry.timer = 0;

    Continue.showen = 0;

    player.ate = 0;
    player.aten = 0;
    player.dead = 0;
    player.dir = 1;
    player.draw = 1;
    player.isalive = 1;
    player.iseating = 0;
    player.playerscale = 0.6;
    player.size = 0;
    player.speedP = score.speedP;

    for (int i = 0; i < 7; i++) {

        bar.FRENZYi[i] = 0;

    }

    bar.currentFrenzy = 0;
    bar.i = 0;
    bar.frenzyTimer = 0;
    bar.lessThanSec = 0;

    for (int i = 0; i < smallFishesNumber; i++) {

        smallFishes[i].aten = 0;
        smallFishes[i].ate = 0;
        smallFishes[i].playerintersectfish = 0;
        smallFishes[i].draw = 1;
        smallFishes[i].direction = 1;
        smallFishes[i].setup(levelAssets.smallTex);
        smallFishes[i].small;
        addfishs(levelAssets.smallTex, smallFishes[i].col, smallFishes[i].rows, smallFishesNumber, smallFishes, 0.7f,
            fishes::small, smallFishes[i].swimFrames, smallFishes[i].swimstart_frame, smallFishes[i].eatFrames,
            smallFishes[i].eatstart_Frame, smallFishes[i].turnFrames, smallFishes[i].turnstart_frame);

    }

    for (int i = 0; i < mediumFishesNumber; i++) {

        mediumFishes[i].aten = 0;
        mediumFishes[i].ate = 0;
        mediumFishes[i].playerintersectfish = 0;
        mediumFishes[i].draw = 1;
        mediumFishes[i].direction = 1;
        mediumFishes[i].setup(levelAssets.medTex);
        mediumFishes[i].mid;
        addfishs(levelAssets.medTex, mediumFishes[i].col, mediumFishes[i].rows, mediumFishesNumber, mediumFishes, 1.2f,
            fishes::mid, mediumFishes[i].swimFrames, mediumFishes[i].swimstart_frame, mediumFishes[i].eatFrames,
            mediumFishes[i].eatstart_Frame, mediumFishes[i].turnFrames, mediumFishes[i].turnstart_frame);

    }

    for (int i = 0; i < bigFishesNumber; i++) {

        bigFishes[i].aten = 0;
        bigFishes[i].ate = 0;
        bigFishes[i].playerintersectfish = 0;
        bigFishes[i].draw = 1;
        bigFishes[i].direction = 1;
        bigFishes[i].setup(levelAssets.bigTex);
        bigFishes[i].big;
        addfishs(levelAssets.bigTex, bigFishes[i].col, bigFishes[i].rows, bigFishesNumber, bigFishes, 1.4f,
            fishes::big, bigFishes[i].swimFrames, bigFishes[i].swimstart_frame, bigFishes[i].eatFrames,
            bigFishes[i].eatstart_Frame, bigFishes[i].turnFrames, bigFishes[i].turnstart_frame);
    }




    Vector2f   target = window.mapPixelToCoords(Mouse::getPosition(window));


}

void updateCommonLogic(float fishSpeed, float deltaTime, RenderWindow& window, int& fishNum, fishes fishArr[],
    setplayer& mainpl, fishes& fish, Shark& shark, Preal& preal) {

    fish.basespeed = fishSpeed;

    FishMovement(fish, deltaTime, mainpl, fishArr, fishNum);

    playermovement(deltaTime, mainpl, window);

    //intersection(mainpl, fish);

    scoring(shark, deltaTime, preal, mainpl);
}

setplayer level2Player, level3Player;

assets  level2assets, level3Assets;

fishes level2smallFishes[15], level2medFishes[8], level2bigFishes[3];
fishes level3SmallFishes[20], level3MedFishes[15], level3BigFishes[5];

bool level2Intialized = false;

bool level3Intialized = false;

void level2Intialization();

void level2Update(float dt, RenderWindow& window);

void level2draw(RenderWindow& window);

void level3Intialization();

void level3Update(float dt, RenderWindow& window);

int main()
{
    mainMenu* menu = nullptr;
    helpAndOptions* help = nullptr;
    RenderWindow window;
    pauseMenu* pause = nullptr;
    levelsmap* map = nullptr;
    commonAssets assets;
    assets.load();
    stageComplete complete;
    complete.assets = &assets;
    complete.window = &window;
    complete.load();
    //pause->load();

    if (!map)
    {
        map = new levelsmap();
        map->font = &assets.game_font;
        map->window = &window;
        map->load();
    }
    menuTextBeforeLevels texts_before_levels;
    window_mode(window, settings.controls[2]);
    texts_before_levels.infoFont = &assets.game_font;
    texts_before_levels.background_sprite = &assets.main_menu_background_sprite;
    texts_before_levels.textMenuIntialization(window);
    Event event;
    Image icon;
    icon.loadFromFile("Sprites\\icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Clock clock;
    gameSounds sounds;
    //pause = new pauseMenu();
    sounds.load();
    //pause->window = &window;
    mermaid.start_mermaid();
    shark.start();

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (state == 0 && menu)
                menu->handle_movements(event);
            else if (state == 1 && help)
                help->handle_movements(event);
            //else if (pause && pause->pause)
                //pause->eventHandler(event);
            else if (state == 2)
            map->handle_movements(event);
            else if (state == 3 || state == 6 || state == 8)
            texts_before_levels.textMenucontrols(window, event);
            else if (state == 9)
                complete.handle_movements(event);

            if (event.type == Event::KeyPressed)
            {

            }
        }


        switch (state)
        {
        case 0:
            if (!menu)
            {
                menu = new mainMenu();
                menu->assets = &assets;
                menu->sound = &sounds;
                menu->menu_font = &assets.game_font;
                menu->window = &window;
                map->menu_scene = &menu->menu_scene;
                menu->load_assets();
            }
            menu->update_menu_scenes();
            break;

        case 1:
            if (!help)
            {
                help = new helpAndOptions();
                help->assets = &assets;
                help->window = &window;
                help->sound = &sounds;
                help->load_assets();
            }
            help->update_menu_scenes();
            break;

        case 2: //map
            map->draw();

            break;

        case 3:
            if (!texts_before_levels.inTextMenuLevel1)
                texts_before_levels.inTextMenuLevel1 = true;
            window.clear();
            texts_before_levels.textMenuLevel1(window);
            window.display();

            shark.update(dt);
            shark.draw(window);
            break;

        case 4: //level 1
            window.clear();
            //pause->draw();
            window.display();
            break;

        case 5:
            if (!level2Intialized)
                level2Intialization();
            window.clear();
            level2Update(dt, window);
            level2draw(window);
            window.display();
            mermaid.update_mermaid(dt);
            mermaid.draw_mermaid(window);
            break;

        case 6:
            if (!texts_before_levels.inTextMenuLevel2)
                texts_before_levels.inTextMenuLevel2 = true;
            window.clear();
            texts_before_levels.textMenuLevel2(window);
            window.display();
            break;

        case 7:
            shark.update(dt);
            shark.draw(window);
            break;

        case 8:
            if (!texts_before_levels.inTextMenuLevel4)
                texts_before_levels.inTextMenuLevel4 = true;
            window.clear();
            texts_before_levels.textMenuLevel4(window);
            window.display();
            break;

        case 9:
            complete.draw();
            break;
        }
    }

    return 0;
}


void window_mode(RenderWindow& window, bool fullscreen) {
    if (fullscreen)
        window.create(VideoMode(settings.res.x, settings.res.y), "Feeding Frenzy 2 - Shipwrech Showdown", Style::Fullscreen);
    else
        window.create(VideoMode(settings.res.x, settings.res.y), "Feeding Frenzy 2 - Shipwrech Showdown");
}

void cameraMovement(View& camera, Vector2f playerPosition, float BGWidth, float BGHeight)
{
    float x = playerPosition.x;
    float y = playerPosition.y;

    if (x < 960)            x = 960;
    if (y < 540)            y = 540;
    if (x > BGWidth - 960)  x = BGWidth - 960;
    if (y > BGHeight - 540) y = BGHeight - 540;

    camera.setCenter(x, y);

}

int scoring(Shark& shark, float dt, Preal& preal, setplayer& player) {

    if (shark.tailBite)         score.score += score.taileBscore * score.bouns;

    if (preal.aten)         score.score += score.prealscore * score.bouns;

    if (score.score >= 1200 * score.levelnum)
        player.size = 55;
    else if (score.score >= 390 * score.levelnum)
        player.size = 35;
    //and if score is higher than 3900 then the level is done and the player wins


    return score.score;
}

void dead(setplayer& player, SORRY& sorry, Cont& Continue) {
    player.draw = 0;

    player.anim.sprite.setPosition(200, -100);

    if (score.lives > 0) {
        score.lives--;
        sorry.active = 1;
        sorry.timer = 0;
    }
    else {
        score.lives = 0;
        score.trials--;
        Continue.showen = 1;
    }

    if (score.trials <= 0) {
        score.lives = 0;
        //gameover
    }
}

void Sorry(setplayer& player, RenderWindow& window, float dt) {
    if (!sorry.active) return;

    sorry.timer += dt;

    if (sorry.timer < 3) {
        player.draw = 0;
        for (int w = 0; w < 5; w++) {
            window.draw(sorry.sprites[w]);
        }
    }
    else {
        player.aten = 0;
        player.dead = 0;
        player.draw = 1;
        if (player.anim.sprite.getPosition().y < 250) {
            player.anim.sprite.move(0, 350 * dt);
        }
        else {
            sorry.timer = 0;
            sorry.active = 0;
        }
        return;
    }
}

void ContinueY_N(RenderWindow& window, setplayer& player) {
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
            score.lives = 3;
            player.anim.sprite.setPosition(200, -100);
            player.draw = 1;
            player.dead = 0;
            player.aten = 0;
            Continue.showen = 0;
        }
        else if (Continue.NOsprite[0].getGlobalBounds().contains(mouseX, mouseY)) {

        }
    }
}

int draw(setplayer& player, RenderWindow& window, float dt, fishes& smallFish, fishes& medFish, fishes bigFish) {

    if (player.draw) window.draw(player.anim.sprite);
    if (smallFish.draw) window.draw(smallFish.anim.sprite);
    if (medFish.draw) window.draw(medFish.anim.sprite);
    if (bigFish.draw) window.draw(bigFish.anim.sprite);
    window.draw(bar.scoretext);

    for (int q = 0; q < 7; q++) {
        window.draw(bar.FRENZYiText[q]);
    }

    Sorry(player, window, dt);
    ContinueY_N(window, player);

    return 0;
}

void UpperBar::start(int lives_count) {
    font.loadFromFile("font.TTF (1)");

    scoretext.setFont(font);
    scoretext.setCharacterSize(30);
    scoretext.setFillColor(Color::Yellow);
    scoretext.setPosition(1600, 175);

    livesText.setFont(font);
    livesText.setCharacterSize(30);
    livesText.setFillColor(Color::Yellow);
    livesText.setPosition(1390, 225);

    growthBorder_texture.loadFromFile("foodbank_float.png");
    growthBorder_sprite[0].setTexture(growthBorder_texture);
    growthBorder_sprite[0].setPosition(100, 70);
    growthBorder_sprite[0].setScale(1.25, 1.25);


    growthBar.setFillColor(Color::Yellow);
    growthBar.setPosition(180, 182);


    FRENZYborder_texture.loadFromFile("tiny_frenzy.png");
    FRENZYborder_sprite.setTexture(FRENZYborder_texture);
    FRENZYborder_sprite.setPosition(800, 100);
    FRENZYborder_sprite.setScale(1.5, 1.5);

    score_texture.loadFromFile("score.png");
    score_sprite.setTexture(score_texture);
    score_sprite.setPosition(1360, 100);

    // lives_texture.loadFromFile("tiny_lives.png");
    lives_sprite.setTexture(lives_texture);
    lives_sprite.setPosition(130, 150);

    for (int q = 0; q < 7; q++) {
        FRENZYiText[q].setFont(font);
        FRENZYiText[q].setPosition(850 + (q * 35), 150);
        FRENZYiText[q].setString(string(1, word[q]));
        FRENZYiText[q].setCharacterSize(40);
        FRENZYiText[q].setFillColor(Color::White);
    }
}

void UpperBar::update(float dt, Score& scoreRef) {
    frenzyTimer += dt;

    float barFullWidth = 350.0f;
    float progress = (float)scoreRef.score / 2000.0f;
    if (progress > 1.0f) progress = 1.0f;
    growthBar.setSize(Vector2f(progress * barFullWidth, 15));


    scoretext.setString(to_string(scoreRef.score));
    livesText.setString(to_string(scoreRef.lives));


    if (frenzyTimer >= 1.5f && currentFrenzy > 0) {
        currentFrenzy--;
        FRENZYi[currentFrenzy] = 0;
        FRENZYiText[currentFrenzy].setFillColor(Color::White);
        frenzyTimer = 0;
    }


}

void UpperBar::draw(RenderWindow& window) {

    window.draw(score_sprite);
    window.draw(lives_sprite);
    window.draw(scoretext);
    window.draw(livesText);
    window.draw(growthBorder_sprite[0]);
    window.draw(growthBar);
    window.draw(FRENZYborder_sprite);
    for (int q = 0; q < 7; q++) {
        window.draw(FRENZYiText[q]);
    }
}

void commonAssets::load()
{
    if (!game_font.loadFromFile("Fonts\\hint.ttf")) cout << "Menu font is not found" << endl;
    if (!highlights_font.loadFromFile("Fonts\\Buttons1.ttf")) cout << "Menu font is not found" << endl;

    if (!main_menu_background_text.loadFromFile("Sprites\\2\\Menu assets\\mainMenu.jpg")) cout << "main menu background is not found" << endl;
    main_menu_background_sprite.setTexture(main_menu_background_text);
    main_menu_background_sprite.setScale(settings.res.x / main_menu_background_text.getSize().x, settings.res.y / main_menu_background_text.getSize().y);

    if (!menu_rocks_text.loadFromFile("Sprites\\2\\Menu assets\\rocks_g.png")) cout << "Rocks are not found" << endl;
    menu_rocks_sprite.setTexture(menu_rocks_text);
    menu_rocks_sprite.setScale((settings.res.x / menu_rocks_text.getSize().x), settings.res.y / menu_rocks_text.getSize().y);
    menu_rocks_sprite.setPosition(settings.res.x * 0.0645, settings.res.y * 0.314f);

    if (!shell_stage_background_text.loadFromFile("Sprites\\menu\\shell_stageBack.png")) cout << "main shell's not found" << endl;
    shell_stage_background_sprite.setTexture(shell_stage_background_text);
    shell_stage_background_sprite.setOrigin(shell_stage_background_sprite.getGlobalBounds().width / 2, shell_stage_background_sprite.getGlobalBounds().height / 2);
    shell_stage_background_sprite.setPosition(settings.res.x / 2, settings.res.y * 0.51f);
    shell_stage_background_sprite.setScale(1.76, 1.75);

}

void gameSounds::load()
{
    if (!main_theme_buffer.loadFromFile("Sounds\\music\\menu_theme.wav")) cout << "tube's texture is not found" << endl;

    main_theme_sound.setBuffer(main_theme_buffer);
    main_theme_sound.setVolume(settings.music_volume);
    main_theme_sound.setLoop(true);

    if (!click_buffer.loadFromFile("Sounds\\sfx\\pop.mp3")) cout << "quad q1 is not found" << endl;

    click_sound.setBuffer(click_buffer);
}

void gameSounds::Click_sound()
{
    if (settings.controls[0])
        click_sound.setVolume(70);
    else
        click_sound.setVolume(0);

    click_sound.play();
}

void gameSounds::play_sound()
{
    if (main_theme_sound.getStatus() != Sound::Playing && settings.controls[1])
        main_theme_sound.play();

    if (!settings.controls[1])
        main_theme_sound.stop();
}

//-------------------------------------------------------

void Mermaid::mermaidMovement(float speed)
{
    Vector2f pos = mermaid.getPosition();
    pos.x -= speed;
    mermaid.setPosition(pos);
}

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
    mermaidTexture.loadFromFile("level sprites/Mermaid.png");

    mermaid.setTexture(mermaidTexture);
    mermaid.setPosition(settings.res.x, 200);
    mermaid.setScale(1.7f, 1.7f);

    starTexture.loadFromFile("level sprites/starBubble1.PNG");

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

//-------------------------------------------------------------

void Shark::start()
{
    if (!signalTexture.loadFromFile("level sprites/dangerSign.png")) cout << "danger sign is not found" << endl;
    signal.setTexture(signalTexture);
    signal.setScale(1.4, 1.4);

    if (!sharkTexture.loadFromFile("level sprites/Barracuda.png")) cout << "shark texture is not found" << endl;
    shark.setTexture(sharkTexture);
    shark.setScale(2.0f, 2.0f);

    fromRight = true;

    if (fromRight) {
        startPos = Vector2f(settings.res.x - signal.getGlobalBounds().width, 300);
    }
    else {
        startPos = Vector2f(0, 300);
    }
    signal.setPosition(startPos);

    if (fromRight) {
        shark.setPosition(settings.res.x + 200, startPos.y);
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
    sharkMovement(300.0f, dt);
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

void Shark::sharkMovement(float speed, float deltaTime)
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
        return;
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
            shark.setPosition(settings.res.x + 250, startPosition.y);
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

//-----------------------------------------------------------

void mainMenu::load_assets()
{
    sound->play_sound();
    Vector2f buttons_scale;

    if (!rr.loadFromFile("Sprites\\menu\\ref.png")) cout << "rr's texture is not found" << endl;
    rrs.setTexture(rr);
    // Loading buttons!
    if (!main_menu_buttons_texture[0][0].loadFromFile("Sprites\\menu\\single-player.png")) cout << "button 1's texture is not found" << endl;
    if (!main_menu_buttons_texture[0][1].loadFromFile("Sprites\\menu\\single-player-glow.png")) cout << "glowing button 1's texture is not found" << endl;
    if (!main_menu_buttons_texture[1][0].loadFromFile("Sprites\\menu\\help-and-options.png")) cout << "button 5's texture is not found" << endl;
    if (!main_menu_buttons_texture[1][1].loadFromFile("Sprites\\menu\\help-and-options-glow.png")) cout << "glowing button 5 texture is not found" << endl;
    if (!main_menu_buttons_texture[2][0].loadFromFile("Sprites\\menu\\quit.png")) cout << "button 6's texture is not found" << endl;
    if (!main_menu_buttons_texture[2][1].loadFromFile("Sprites\\menu\\quit-glow.png")) cout << "glowing button 6 texture is not found" << endl;

    if (!single_player_texture[0][0].loadFromFile("Sprites\\menu\\story-mode.png")) cout << "glowing button 1's texture is not found" << endl;
    if (!single_player_texture[0][1].loadFromFile("Sprites\\menu\\story-mode-glow.png")) cout << "glowing button 1's texture is not found" << endl;
    if (!single_player_texture[1][0].loadFromFile("Sprites\\menu\\back.png")) cout << "glowing button 1's texture is not found" << endl;
    if (!single_player_texture[1][1].loadFromFile("Sprites\\menu\\back-glow.png")) cout << "glowing button 1's texture is not found" << endl;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            main_menu_buttons_sprite[i][j].setTexture(main_menu_buttons_texture[i][j]);
            main_menu_buttons_sprite[i][j].setOrigin(main_menu_buttons_sprite[i][j].getGlobalBounds().width / 2, main_menu_buttons_sprite[i][j].getGlobalBounds().height / 2);

            switch (i)
            {
            case 0:
                main_menu_buttons_sprite[i][j].setPosition(settings.res.x / 2 + 420, settings.res.y / 2 - 240);
                if (j) { main_menu_buttons_sprite[i][j].setScale(0.32f, 0.32f); break; }
                main_menu_buttons_sprite[i][j].setScale(0.315f, 0.315f);
                break;
            case 1:
                main_menu_buttons_sprite[i][j].setPosition(settings.res.x / 2 + 420, settings.res.y / 2 - 100);
                main_menu_buttons_sprite[i][j].setRotation(-1.3f);
                if (j) { main_menu_buttons_sprite[i][j].setScale(0.285f, 0.285f); break; }
                main_menu_buttons_sprite[i][j].setScale(0.275f, 0.275f);
                break;
            case 2:
                main_menu_buttons_sprite[i][j].setPosition(settings.res.x / 2 + 420, settings.res.y / 2 + 50);
                main_menu_buttons_sprite[i][j].setRotation(-1.45f);
                if (j) { main_menu_buttons_sprite[i][j].setScale(0.19f, 0.19f); break; }
                main_menu_buttons_sprite[i][j].setScale(0.18f, 0.18f);
                break;
            }
        }
    }

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            single_player_sprite[i][j].setTexture(single_player_texture[i][j]);
            if (j) single_player_sprite[i][j].setScale(0.3f, 0.3f);
            else single_player_sprite[i][j].setScale(0.29f, 0.29f);
            single_player_sprite[i][j].setOrigin(single_player_sprite[i][j].getGlobalBounds().width / 2, single_player_sprite[i][j].getGlobalBounds().height / 2);
            if (i == 0)
            {
                single_player_sprite[i][j].setPosition(settings.res.x / 2 + 220, settings.res.y / 2 - 200);
                single_player_sprite[i][j].rotate(1.8);
            }
            else
            {
                single_player_sprite[i][j].setPosition(settings.res.x / 2 + 220, settings.res.y / 2 - 30);
                single_player_sprite[i][j].rotate(-1.3);
            }
        }
    }

    // al original menu "for refrence"
    // ment.loadFromFile("men.jpg");
    // mens.setScale(settings.res.x / ment.getSize().x, settings.res.y / ment.getSize().y);
    // mens.setTexture(ment);

    if (!tool_tip_texture.loadFromFile("Sprites\\2\\Menu assets\\tooltip.png")) cout << "tooltip's texture is not found" << endl;
    tool_tip_sprite.setTexture(tool_tip_texture);
    tool_tip_sprite.setOrigin(tool_tip_sprite.getGlobalBounds().width / 2, 0);
    tool_tip_sprite.setPosition(settings.res.x / 2, settings.res.y * 0.735f);
    tool_tip_sprite.setScale((settings.res.x * 0.5f) / tool_tip_texture.getSize().x, (settings.res.y * 0.19f) / tool_tip_texture.getSize().y);
    // hehe
    if (!logo_texture.loadFromFile("Sprites\\2\\Menu assets\\logo.png")) cout << "logo's texture is not found" << endl;
    logo_sprite.setTexture(logo_texture);
    logo_sprite.setScale((settings.res.x * 0.396f) / logo_texture.getSize().x, (settings.res.y * 0.39f) / logo_texture.getSize().y);
    logo_sprite.setPosition(settings.res.x * 0.296f, settings.res.y * 0.033f);

    if (!tube_texture.loadFromFile("Sprites\\2\\Menu assets\\tubes_01.png")) cout << "tube's texture is not found" << endl;
    tube_sprite.setTexture(tube_texture);
    tube_sprite.setOrigin(tube_texture.getSize().x / 2, tube_texture.getSize().y);
    tube_sprite.setScale((settings.res.x * 0.135f) / tube_texture.getSize().x, (settings.res.y * 0.315f) / tube_texture.getSize().y);
    tube_sprite.setPosition(settings.res.x * 0.715f, settings.res.y * 1.f);

    if (!txt.loadFromFile("Fonts\\press_any_button.png")) cout << "button is not found" << endl;
    txts.setTexture(txt);
    txts.setScale(settings.res.x / txt.getSize().x, settings.res.y / txt.getSize().y);

    for (int i = 0; i < 3; i++)
    {
        FloatRect bounds;

        if (i == 2) menu_text[i].setString(single_player_string[i]);
        else menu_text[i].setString(menu_strings[i]);
        menu_text[i].setCharacterSize(48);
        menu_text[i].setFillColor(Color::White);
        menu_text[i].setOutlineColor(Color::Black);
        menu_text[i].setOutlineThickness(2);
        menu_text[i].setFont(*menu_font);
        bounds = menu_text[i].getLocalBounds();
        menu_text[i].setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        menu_text[i].setPosition(settings.res.x / 2, settings.res.y / 2 + 360);

        single_player_text[i].setString(single_player_string[i]);
        single_player_text[i].setCharacterSize(48);
        single_player_text[i].setFillColor(Color::White);
        single_player_text[i].setOutlineColor(Color::Black);
        single_player_text[i].setOutlineThickness(2);
        single_player_text[i].setFont(*menu_font);
        bounds = single_player_text[i].getLocalBounds();
        single_player_text[i].setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        single_player_text[i].setPosition(settings.res.x / 2, settings.res.y / 2 + 360);

    }

    // click to start refrence "hab2a ashelha ba3deen"
    if (!reft.loadFromFile("ref1.png")) cout << "ref's texture is not found" << endl;
    refs.setTexture(reft);
    refs.setScale(settings.res.x / reft.getSize().x, settings.res.y / reft.getSize().y);

    if (!the_sign_texture.loadFromFile("Sprites\\menu\\sprite_clean.png")) cout << "quad back is not found" << endl;
    the_sign_sprite.setTexture(the_sign_texture);
    the_sign_sprite.setOrigin(the_sign_sprite.getGlobalBounds().width / 2, the_sign_sprite.getGlobalBounds().height / 2);
    the_sign_sprite.setPosition(settings.res.x / 2, settings.res.y / 2 + 360);
    the_sign_sprite.setScale(0.32f, 0.32f);


    if (!quads_texture[0].loadFromFile("Sprites\\menu\\quad_back.png")) cout << "quad back is not found" << endl;
    if (!quads_texture[1].loadFromFile("Sprites\\menu\\quad_ring.png")) cout << "quad ring is not found" << endl;
    if (!quads_texture[2].loadFromFile("Sprites\\menu\\quad_q2.png")) cout << "quad q2 is not found" << endl;
    if (!quads_texture[3].loadFromFile("Sprites\\menu\\quad_q3.png")) cout << "quad q3 is not found" << endl;
    if (!quads_texture[4].loadFromFile("Sprites\\menu\\quad_q4.png")) cout << "quad q4 is not found" << endl;
    if (!quads_texture[5].loadFromFile("Sprites\\menu\\quad_q1.png")) cout << "quad q1 is not found" << endl;

    for (int i = 0; i < 6; i++)
    {
        quads_sprite[i].setTexture(quads_texture[i]);
        quads_sprite[i].setOrigin(quads_sprite[i].getGlobalBounds().width / 2, quads_sprite[i].getGlobalBounds().height / 2);
        quads_sprite[i].setPosition(settings.res.x * 0.69201f, settings.res.y * 0.165f);
        buttons_scale = { (settings.res.x * 0.171f) / quads_texture[i].getSize().x,(settings.res.y * 0.28f) / quads_texture[i].getSize().y };
        quads_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
    }
};

void mainMenu::handle_movements(Event& event)
{
    if (event.type == Event::KeyPressed)
    {
        sound->Click_sound();
        if (menu_scene == 0 && event.key.code != Keyboard::Escape)
            menu_scene++;

        else if (menu_scene == 1)
        {
            if (event.key.code == Keyboard::Down)
                selected++, selected %= 3;
            if (event.key.code == Keyboard::Up)
                selected = (selected - 1) + 3, selected %= 3;
            if (event.key.code == Keyboard::Escape)
            {
                menu_scene--; selected = -1;
                cout << menu_scene << endl;
            }
            if (event.key.code == Keyboard::Enter)
                if (menu_scene == 1)
                {
                    if (selected == 0)
                        menu_scene = 2;

                    else if (selected == 4)
                    {
                        state = 1;
                        return;
                    }
                    else if (selected == 5)
                    {
                        window->close();
                    }
                    selected = -1;
                }
                else if (menu_scene == 2)
                {
                    if (selected == 0)
                        menu_scene = 2;
                    else if (selected == 3)
                        menu_scene = 1;
                }
        }
        else if (menu_scene == 2)
        {
            if (event.key.code == Keyboard::Escape)
                menu_scene = 1;
            if (event.key.code == Keyboard::Down)
                selected++, selected %= 2;
            if (event.key.code == Keyboard::Up)
                selected = (selected - 1) + 2, selected %= 2;
        }
        else if (menu_scene == 3 || menu_scene == 5 || menu_scene == 4)
        {
            if (event.key.code == Keyboard::Escape)
                menu_scene = 1;
        }
    }

    if (event.type == Event::MouseMoved)
    {
        Vector2i mousePixel = Mouse::getPosition(*window);
        Vector2f mouse_position = window->mapPixelToCoords(mousePixel);
        switch (menu_scene)
        {
        case 1:
            selected = -1;
            for (int i = 0; i < 3; i++)
            {
                if (main_menu_buttons_sprite[i][0].getGlobalBounds().contains(mouse_position))
                {
                    selected = i;
                    break;
                }
                else
                    selected = -1;
            }
            break;
        case 2:
            for (int i = 0; i < 2; i++)
            {
                if (single_player_sprite[i][0].getGlobalBounds().contains(mouse_position))
                {
                    selected = i;
                    break;
                }
                else
                    selected = -1;
            }
            break;
        }
    }

    if (event.type == Event::MouseButtonPressed)
    {

        sound->Click_sound();
        Vector2i mousePixel = Mouse::getPosition(*window);
        Vector2f mouse_position = window->mapPixelToCoords(mousePixel);

        if (event.mouseButton.button == Mouse::Left)
        {
            if (menu_scene == 0)
                menu_scene++;


            if (menu_scene == 1)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (selected == 2)
                        window->close();
                    if (selected == 1)
                    {
                        state = 1; return;
                    }
                    if (selected == i)
                    {
                        menu_scene = i + 2, selected = -1; break;
                    }
                }
            }

            else if (menu_scene == 2)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (selected == 0)
                        state = 2, selected = -1;
                    else
                        menu_scene = 1, selected = -1;
                }
            }
        }
    }
};

void mainMenu::logo_transformation(char a)
{
    if (a == 'f')
    {
        logo_sprite.setScale((settings.res.x * 0.365f) / logo_texture.getSize().x, (settings.res.y * 0.37f) / logo_texture.getSize().y);
        logo_sprite.setPosition(settings.res.x * 0.1665f, settings.res.y * 0.05f);
    }
    else
    {
        logo_sprite.setScale((settings.res.x * 0.396f) / logo_texture.getSize().x, (settings.res.y * 0.39f) / logo_texture.getSize().y);
        logo_sprite.setPosition(settings.res.x * 0.296f, settings.res.y * 0.033f);
    }
};

void mainMenu::update_menu_scenes()
{
    switch (menu_scene)
    {
    case 0: logo_transformation('b'); draw_starting_menu(); break;
    case 1: logo_transformation('f'); draw_main_menu(); break;
    case 2: draw_single_player_menu(); break;
    }
}

void mainMenu::draw_basics()
{
    window->clear();
    window->draw(assets->main_menu_background_sprite);
    window->draw(logo_sprite);
}

void mainMenu::draw_starting_menu()
{
    draw_basics();
    //window.draw(tube_sprite);
    window->draw(assets->menu_rocks_sprite);
    window->draw(txts);
    window->display();
}

void mainMenu::draw_main_menu()
{
    draw_basics();

    for (int i = 2; i >= 0; i--)
    {
        window->draw(main_menu_buttons_sprite[i][selected == i ? 1 : 0]);
    }
    window->draw(assets->menu_rocks_sprite);
    window->draw(tool_tip_sprite);
    for (int i = 0; i < 2; i++)
    {
        if (selected == 0) window->draw(the_sign_sprite);
        else if (selected >= 1) window->draw(menu_text[selected - 1]);
        else window->draw(menu_text[2]);
    }
    window->display();
};

void mainMenu::draw_single_player_menu()
{
    draw_basics();
    window->draw(assets->menu_rocks_sprite);
    window->draw(tool_tip_sprite);

    for (int i = 0; i < 2; i++)
    {
        window->draw(single_player_sprite[i][selected == i ? 1 : 0]);
        if (selected >= 0) window->draw(single_player_text[selected]);
        else window->draw(single_player_text[2]);
        window->draw(quads_sprite[i]);
    }

    window->display();
}

void pauseMenu::load() {

    float centerX = window->getSize().x / 2;
    float startY = window->getSize().y / 2 - 125.f;
    float spacing = 150.f;

    if (!stage_Texture.loadFromFile("Sprites/pause menu/stage.png")) { cout << "Error loading stage" << endl; }
    if (!background_Texture.loadFromFile("Sprites/pause menu/stageBack.png")) { cout << "Error loading background" << endl; }
    if (!buttons[0][0].loadFromFile("Sprites/pause menu/resume off.png")) { cout << "Error loading resume1" << endl; }
    if (!buttons[0][1].loadFromFile("Sprites/pause menu/resume on.png")) { cout << "Error loading resume2" << endl; }
    if (!buttons[1][0].loadFromFile("Sprites/pause menu/options off.png")) { cout << "Error loading options1" << endl; }
    if (!buttons[1][1].loadFromFile("Sprites/pause menu/options on.png")) { cout << "Error loading options2" << endl; }
    if (!buttons[2][0].loadFromFile("Sprites/pause menu/exit off.png")) { cout << "Error loading exit1" << endl; }
    if (!buttons[2][1].loadFromFile("Sprites/pause menu/exit on 4.png")) { cout << "Error loading exit2" << endl; }

    for (int i = 0; i < 3; i++) {
            buttons_sprite[i].setTexture(buttons[i][0]);
            buttons_sprite[i].setOrigin(buttons_sprite[i].getGlobalBounds().width / 2, buttons_sprite[i].getGlobalBounds().height / 2);
    }
    stageSprite.setTexture(stage_Texture);
    backgroundSprite.setTexture(background_Texture);


    stageSprite.setPosition(0.f, -370.0f);
    backgroundSprite.setPosition(0.f, 110.0f);
    buttons_sprite[0].setPosition(centerX, startY);
    buttons_sprite[1].setPosition(centerX, startY + spacing);
    buttons_sprite[2].setPosition(centerX, startY + spacing * 2);

    stageSprite.setScale(1.3f, 1.3f);
    backgroundSprite.setScale(1.3f, 1.3f);

}

void pauseMenu::selection() {

    if (selectedOption == -1) {
        return;
    }

    if (selectedOption == 0) {
        pause = false;
    }

    if (selectedOption == 2) {
        state = 0;
    }
}

void pauseMenu::drawBackground() {

    window->draw(backgroundSprite);
    window->draw(stageSprite);
}

void pauseMenu::draw() {

    drawBackground();

    buttons_sprite[0].setTexture(selectedOption == 0 ? buttons[0][1] : buttons[0][0]);
    buttons_sprite[1].setTexture(selectedOption == 1 ? buttons[1][1] : buttons[1][0]);
    buttons_sprite[2].setTexture(selectedOption == 2 ? buttons[2][1] : buttons[2][0]);

    window->draw(buttons_sprite[0]);
    window->draw(buttons_sprite[1]);
    window->draw(buttons_sprite[2]);
}

void pauseMenu::eventHandler(Event& event) {

    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
        pause = !pause;
    }

    if (!pause) {
        return;
    }

    if (event.type == Event::KeyPressed) {

        if (event.key.code == Keyboard::Up) {
            selectedOption--;
        }

        if (event.key.code == Keyboard::Down) {
            selectedOption++;
        }

        selectedOption = (selectedOption + 3) % 3;

        if (event.key.code == Keyboard::Return) {
            selection();
        }
    }

    if (event.type == Event::MouseMoved || event.type == Event::MouseButtonReleased) {

        Vector2f worldPos = window->mapPixelToCoords(Mouse::getPosition(*window));

        if (buttons_sprite[0].getGlobalBounds().contains(worldPos)) {
            selectedOption = 0;
        }

        else if (buttons_sprite[1].getGlobalBounds().contains(worldPos)) {
            selectedOption = 1;
        }

        else if (buttons_sprite[2].getGlobalBounds().contains(worldPos)) {
            selectedOption = 2;
        }

        else {
            selectedOption = -1;
        }

        if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
            selection();
        }
    }
};

void helpAndOptions::load_assets()
{
    reft.loadFromFile("Sprites\\menu\\2.png");
    refs.setTexture(reft);
    for (int i = 0; i < 7; i++)
        buttons[i].load_buttons();

    if (!mouse_texture.loadFromFile("Sprites\\menu\\mouse.png")) cout << "mouse texture is not found" << endl;

    if (!spongebob_text.loadFromFile("Sprites\\menu\\sponge-bob.png")) cout << "sponge-bob texture is not found" << endl;

    if (!menu_shell_texture[0].loadFromFile("Sprites\\menu\\shell_stageBack.png")) cout << "main shell's not found" << endl;
    if (!menu_shell_texture[1].loadFromFile("Sprites\\menu\\help-and-options-shell1.png")) cout << "help-and-optionsshell's not found" << endl;
    if (!menu_shell_texture[2].loadFromFile("Sprites\\menu\\settings-shell.png")) cout << "settings shell is not found" << endl;
    if (!menu_shell_texture[3].loadFromFile("Sprites\\menu\\controls-shell.png")) cout << "controls shell is not found" << endl;
    if (!menu_shell_texture[4].loadFromFile("Sprites\\menu\\how-to-play-shell.png")) cout << "controls shell is not found" << endl;
    if (!menu_shell_texture[5].loadFromFile("Sprites\\menu\\shell_stage.png")) cout << "controls shell is not found" << endl;

    if (!buttons_texture[0][0].loadFromFile("Sprites\\menu\\controls.png")) cout << "not found" << endl;
    if (!buttons_texture[0][1].loadFromFile("Sprites\\menu\\controls-glow.png")) cout << "not found" << endl;
    if (!buttons_texture[1][0].loadFromFile("Sprites\\menu\\how-to-play.png")) cout << "not found" << endl;
    if (!buttons_texture[1][1].loadFromFile("Sprites\\menu\\how-to-play-glow.png")) cout << "not found" << endl;
    if (!buttons_texture[2][0].loadFromFile("Sprites\\menu\\settings.png")) cout << "not found" << endl;
    if (!buttons_texture[2][1].loadFromFile("Sprites\\menu\\settings-glow.png")) cout << "not found" << endl;
    if (!buttons_texture[3][0].loadFromFile("Sprites\\menu\\credits.png")) cout << "not found" << endl;
    if (!buttons_texture[3][1].loadFromFile("Sprites\\menu\\credits-glow.png")) cout << "not found" << endl;
    if (!buttons_texture[4][0].loadFromFile("Sprites\\menu\\done.png")) cout << "not found" << endl;
    if (!buttons_texture[4][1].loadFromFile("Sprites\\menu\\done-glow.png")) cout << "not found" << endl;
    if (!buttons_texture[5][0].loadFromFile("Sprites\\menu\\lil-done.png")) cout << "not found" << endl;
    if (!buttons_texture[5][1].loadFromFile("Sprites\\menu\\lil-done-glow.png")) cout << "not found" << endl;

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            buttons_sprite[i][j].setTexture(buttons_texture[i][j]);
            buttons_sprite[i][j].setOrigin(buttons_sprite[i][j].getGlobalBounds().width / 2, buttons_sprite[i][j].getGlobalBounds().height / 2);
            if (!j)
                buttons_sprite[i][j].setScale(0.245f, 0.245f);
            else
                buttons_sprite[i][j].setScale(0.255f, 0.255f);

            switch (i)
            {
            case 0: buttons_sprite[i][j].setPosition(settings.res.x / 2.f, settings.res.y * 0.265f); break;
            case 1: buttons_sprite[i][j].setPosition(settings.res.x / 2.f, settings.res.y * 0.38f); break;
            case 2: buttons_sprite[i][j].setPosition(settings.res.x / 2.f, settings.res.y * 0.495f); break;
            case 3: buttons_sprite[i][j].setPosition(settings.res.x / 2.f, settings.res.y * 0.61f); break;
            case 4: buttons_sprite[i][j].setPosition(settings.res.x / 2.f, settings.res.y * 0.79f); break;
            case 5: buttons_sprite[i][j].setPosition(settings.res.x / 2.f, settings.res.y * 0.79f);
                if (!j)
                    buttons_sprite[i][j].setScale(0.2, 0.2);
                else
                    buttons_sprite[i][j].setScale(0.22, 0.22);
                break;
            }

            if (i == 1 || i == 2)
            {
                if (!j)
                    buttons_sprite[i][j].setScale(0.245, 0.238);
                else
                    buttons_sprite[i][j].setScale(0.255, 0.239);
            }
        }
    }

    for (int i = 0; i < 6; i++)
    {
        menu_shell_sprite[i].setTexture(menu_shell_texture[i]);
        menu_shell_sprite[i].setOrigin(menu_shell_sprite[i].getGlobalBounds().width / 2, menu_shell_sprite[i].getGlobalBounds().height / 2);
        menu_shell_sprite[i].setPosition(settings.res.x / 2, settings.res.y * 0.51f);
        menu_shell_sprite[i].setScale(1.76, 1.75);
    }

    mouse_sprite.setTexture(mouse_texture);
    mouse_sprite.setOrigin(mouse_sprite.getGlobalBounds().width / 2, mouse_sprite.getGlobalBounds().height / 2);
    mouse_sprite.setPosition(settings.res.x * 0.43f, settings.res.y * 0.5f);
    mouse_sprite.setScale(0.9f, 0.9f);

    spongebob_sprite.setTexture(spongebob_text);
    spongebob_sprite.setOrigin(spongebob_sprite.getGlobalBounds().width / 2, spongebob_sprite.getGlobalBounds().height / 2);
    spongebob_sprite.setPosition(settings.res.x / 2 - 250, settings.res.y / 2 + 100);
    spongebob_sprite.setScale(0.6f, 0.6f);


    for (int i = 0; i < 4; i++)
    {
        settings_text[i].setString(settings_string[i]);
        settings_text[i].setFont(assets->game_font);
        settings_text[i].setFillColor(Color::White);
        settings_text[i].setOutlineColor(Color::Black);
        settings_text[i].setCharacterSize(48);
        settings_text[i].setOutlineThickness(2);
        FloatRect bounds = settings_text[i].getLocalBounds();
        settings_text[i].setOrigin(bounds.left + bounds.width, bounds.top);

        settings_text[i].setPosition(0.477f * settings.res.x, 0.272f * settings.res.y + i * 100);
    }

    for (int i = 0; i < 2; i++)
    {
        how_to_play_text[i].setString(how_to_play_string[i]);
        how_to_play_text[i].setFont(assets->game_font);
        how_to_play_text[i].setFillColor(Color::White);
        how_to_play_text[i].setOutlineColor(Color::Black);
        how_to_play_text[i].setCharacterSize(34);
        how_to_play_text[i].setOutlineThickness(2);
        how_to_play_text[i].setOrigin(how_to_play_text[i].getGlobalBounds().width / 2, how_to_play_text[i].getGlobalBounds().height / 2);
    }

    how_to_play_text[0].setPosition((0.44f) * settings.res.x, (0.335f) * settings.res.y);
    how_to_play_text[1].setPosition((0.435f) * settings.res.x, (0.585) * settings.res.y);

    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 2; j++)
            for (int h = 0; h < 2; h++)
            {
                if (i <= 3)
                    buttons[i].checkbox_sprite[j][h].setPosition(0.495f * settings.res.x, 0.295f * settings.res.y + i * 100);
                else
                    buttons[i].checkbox_sprite[j][h].setPosition(0.495f * settings.res.x + (i - 3) * 60, 0.295f * settings.res.y + 3 * 100);
            }

    for (int i = 0; i < 9; i++)
    {
        namesText[i].setFont(assets->game_font);
        namesText[i].setString(namesString[i]);
        namesText[i].setCharacterSize(40);
        namesText[i].setFillColor(Color::White);
        namesText[i].setOutlineThickness(2);
        namesText[i].setOutlineColor(Color::Black);
        FloatRect bounds = namesText[i].getLocalBounds();
        namesText[i].setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        namesText[i].setPosition(settings.res.x / 2, settings.res.y / 2 - 280 + (i * 70));
    }
    for (int i = 0; i < 10; i = i + 8)
    {
        namesText[i].setFont(assets->highlights_font);
        namesText[i].setFillColor(Color(222, 199, 75));
    }
    namesText[0].setCharacterSize(50);
    namesText[8].setCharacterSize(90);
    namesText[8].setPosition(settings.res.x / 2 - 90, settings.res.y / 2 - 440);
}

void helpAndOptions::handle_movements(Event& event)
{

    if (event.type == Event::KeyPressed)
    {
        sound->Click_sound();
        if (options_scene == 0)
        {
            if (event.key.code == Keyboard::Escape)
                state = 0;
            if (event.key.code == Keyboard::Down)
                selected++, selected %= 5;
            if (event.key.code == Keyboard::Up)
                selected = (selected - 1) + 5, selected %= 5;
            if (event.key.code == Keyboard::Enter)
            {
                if (selected == 0)
                    options_scene = 1, selected = -1;
                else if (selected == 1)
                    options_scene = 3;
                else if (selected == 2)
                    options_scene = 2, selected = -1;
                else if (selected == 3)
                    options_scene = 4, selected = -1;
                else if (selected == 4)
                    state = 0, selected = -1;
            }
        }

        else
        {
            if (event.key.code == Keyboard::Escape || (event.key.code == Keyboard::Enter && selected == 0))
                options_scene = 0, selected = -1;
            if (event.key.code == Keyboard::Up || event.key.code == Keyboard::Down)
                selected = 0;
        }
    }

    if (event.type == Event::MouseMoved)
    {

        Vector2i mousePixel = Mouse::getPosition(*window);
        Vector2f mouse_position = window->mapPixelToCoords(mousePixel);

        if (options_scene == 0)
        {
            for (int i = 0; i < 5; i++)
            {
                if (buttons_sprite[i][0].getGlobalBounds().contains(mouse_position))
                {
                    selected = i;
                    break;
                }
                else
                    selected = -1;
            }
        }

        else if (options_scene == 2)
        {
            for (int i = 0; i < 7; i++)
            {
                if (buttons_sprite[5][0].getGlobalBounds().contains(mouse_position))
                {
                    selected = 0;
                    break;
                }
                if (buttons[i].checkbox_sprite[0][0].getGlobalBounds().contains(mouse_position))
                {
                    selected = i + 1;
                    break;
                }
                else
                    selected = -1;
            }

        }
        else if (options_scene == 1 || options_scene == 3 || options_scene == 4)
        {
            selected = buttons_sprite[4][0].getGlobalBounds().contains(mouse_position) ? 0 : -1;
        }
    }

    if (event.type == Event::MouseButtonPressed)
    {
        sound->Click_sound();
        Vector2i mousePixel = Mouse::getPosition(*window);
        Vector2f mouse_position = window->mapPixelToCoords(mousePixel);
        if (event.mouseButton.button == Mouse::Left)
        {
            if (options_scene == 0)
            {
                if (selected == 0)
                    options_scene = 1, selected = -1;
                else if (selected == 1)
                    options_scene = 3;
                else if (selected == 2)
                    options_scene = 2, selected = -1;
                else if (selected == 3)
                    options_scene = 4, selected = -1;
                else if (selected == 4)
                    state = 0, selected = -1;
            }
            else if (options_scene == 2)
            {
                if (selected == 0)
                    options_scene = 0, selected = -1;
                else if (selected == 1)
                    settings.controls[0] = !settings.controls[0];
                else if (selected == 2)
                    settings.controls[1] = !settings.controls[1];
                else if (selected == 3)
                    settings.controls[2] = !settings.controls[2], window_mode(*window, settings.controls[2]);
            }
            else if (options_scene == 1 || options_scene == 3 || options_scene == 4)
            {
                if (selected == 0) options_scene = 0, selected = -1;
            }
        }
    }
}

void helpAndOptions::update_menu_scenes()
{
    switch (options_scene)
    {
    case 0: draw(); break;
    case 1: draw_controls(); break;
    case 2: draw_settings(); break;
    case 3: draw_how_to_play(); break;
    case 4: draw_credits(); break;
    }
}

void helpAndOptions::draw_basics()
{
    window->clear();
    window->draw(assets->main_menu_background_sprite);
    window->draw(assets->menu_rocks_sprite);
}

void helpAndOptions::draw()
{
    draw_basics();
    //window->draw(refs);
    for (int i = 0; i < 2; i++)
        window->draw(menu_shell_sprite[i]);
    for (int i = 0; i < 5; i++)
        window->draw(buttons_sprite[i][selected == i ? 1 : 0]);
    window->display();
}

void helpAndOptions::update_settings()
{
    sound->play_sound();
}

void helpAndOptions::draw_settings()
{
    update_settings();
    draw_basics();
    //window->draw(refs);
    for (int i = 0; i < 3; i = i + 2)
        window->draw(menu_shell_sprite[i]);

    for (int i = 0; i < 4; i++)
    {
        window->draw(settings_text[i]);
    }
    for (int i = 0; i < 6; i++)
        window->draw(buttons[i].checkbox_sprite[settings.controls[i] ? 1 : 0][selected == (i + 1) ? 1 : 0]);
    //window->draw(sound_volume);
    window->draw(buttons_sprite[5][selected == 0 ? 1 : 0]);
    window->display();
}

void helpAndOptions::draw_controls()
{
    draw_basics();

    for (int i = 0; i < 4; i = i + 3)
    {
        window->draw(menu_shell_sprite[i]);
        if (i == 3)
            continue;
        window->draw(mouse_sprite);
    }

    window->draw(buttons_sprite[5][selected == 0 ? 1 : 0]);
    window->display();
}

void helpAndOptions::draw_how_to_play()
{
    draw_basics();
    //window->draw(refs);
    for (int i = 0; i < 5; i = i + 4)
        window->draw(menu_shell_sprite[i]);
    for (int i = 0; i < 2; i++)
        window->draw(how_to_play_text[i]);
    window->draw(buttons_sprite[4][selected == 0 ? 1 : 0]);
    window->display();
}

void helpAndOptions::draw_credits()
{
    draw_basics();
    window->draw(menu_shell_sprite[0]);
    window->draw(spongebob_sprite);
    for (int i = 0; i < 8; i++)
        window->draw(namesText[i]);
    window->draw(menu_shell_sprite[5]);
    window->draw(buttons_sprite[4][selected == 0 ? 1 : 0]);
    window->draw(namesText[8]);
    window->display();
}

void menuTextBeforeLevels::textMenuIntialization(RenderWindow& window) {

    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;

    //The scale and position of everything is dependant on the window size

    float widthChange = windowWidth / 1080.f;
    float heightChange = windowHeight / 600.f;

    textMenuBorderTexture.loadFromFile("Sprites\\Text screen\\shell_stage.png");
    textMenuBackgroundTexture.loadFromFile("Sprites\\Text screen\\shell_stageBack.png");
    longButtonOffTexture.loadFromFile("Sprites\\Text screen\\longButtonOff.png");
    longButtonOnTexture.loadFromFile("Sprites\\Text screen\\longButtonOn.png");
    shortButtonOffTexture.loadFromFile("Sprites\\Text screen\\shortButtonOff.png");
    shortButtonOnTexture.loadFromFile("Sprites\\Text screen\\shortButtonOn.png");
    continueTextOffTexture.loadFromFile("Sprites\\Text screen\\continueTextOff.png");
    continueTextOnTexture.loadFromFile("Sprites\\Text screen\\continueTextOn.png");
    quitTextOffTexture.loadFromFile("Sprites\\Text screen\\quitTextOff.png");
    quitTextOnTexture.loadFromFile("Sprites\\Text screen\\quitTextOn.png");
    borisTexture.loadFromFile("Sprites\\Text screen\\boris.png");
    orangeFishTexture.loadFromFile("Sprites\\Text screen\\orangeFish.png");
    blueFishTexture.loadFromFile("Sprites\\Text screen\\blueFish.png");
    brownFishTexture.loadFromFile("Sprites\\Text screen\\brownFish.png");
    barracudaTexture.loadFromFile("Sprites\\Text screen\\barracuda.png");
    humpheadTexture.loadFromFile("Sprites\\Text screen\\humphead.png");
    eddieTexture.loadFromFile("Sprites\\Text screen\\eddie.png");
    johnDorryTexture.loadFromFile("Sprites\\Text screen\\johnDory.png");
    orvilleTexture.loadFromFile("Sprites\\Text screen\\orville.png");
    mineTexture.loadFromFile("Sprites\\Text screen\\mine.png");

    textMenuBorderSprite.setTexture(textMenuBorderTexture);
    textMenuBorderSprite.setOrigin(textMenuBorderSprite.getGlobalBounds().width / 2.f, textMenuBorderSprite.getGlobalBounds().height / 2.f);
    textMenuBorderSprite.setScale(windowWidth / textMenuBorderSprite.getGlobalBounds().width, windowHeight / textMenuBorderSprite.getGlobalBounds().height);
    textMenuBorderSprite.setPosition(windowWidth / 2.f, windowHeight / 2.f);

    textMenuBackgroundSprite.setTexture(textMenuBackgroundTexture);
    textMenuBackgroundSprite.setOrigin(textMenuBackgroundSprite.getGlobalBounds().width / 2.f, textMenuBackgroundSprite.getGlobalBounds().height / 2.f);
    textMenuBackgroundSprite.setScale(windowWidth / textMenuBackgroundSprite.getGlobalBounds().width, windowHeight / textMenuBackgroundSprite.getGlobalBounds().height);
    textMenuBackgroundSprite.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

    longButtonOffSprite.setTexture(longButtonOffTexture);
    longButtonOffSprite.setOrigin(longButtonOffSprite.getGlobalBounds().width / 2.f, longButtonOffSprite.getGlobalBounds().height / 2.f);
    longButtonOffSprite.setScale(1.2 * widthChange, 1.2 * heightChange);
    longButtonOffSprite.setPosition(windowWidth / 2.f, windowHeight - longButtonOffSprite.getGlobalBounds().height / 2.f - (50 * heightChange));

    longButtonOnSprite.setTexture(longButtonOnTexture);
    longButtonOnSprite.setOrigin(longButtonOnSprite.getGlobalBounds().width / 2.f, longButtonOnSprite.getGlobalBounds().height / 2.f);
    longButtonOnSprite.setScale(longButtonOffSprite.getScale());
    longButtonOnSprite.setPosition(longButtonOffSprite.getPosition());

    shortButtonOffSprite.setTexture(shortButtonOffTexture);
    shortButtonOffSprite.setOrigin(shortButtonOffSprite.getGlobalBounds().width / 2.f, shortButtonOffSprite.getGlobalBounds().height / 2.f);
    shortButtonOffSprite.setScale(longButtonOffSprite.getScale());
    shortButtonOffSprite.setPosition(longButtonOffSprite.getPosition().x + longButtonOffSprite.getGlobalBounds().width / 2.f + shortButtonOffSprite.getGlobalBounds().width / 2.f, longButtonOffSprite.getPosition().y);

    shortButtonOnSprite.setTexture(shortButtonOnTexture);
    shortButtonOnSprite.setOrigin(shortButtonOnSprite.getGlobalBounds().width / 2.f, shortButtonOnSprite.getGlobalBounds().height / 2.f);
    shortButtonOnSprite.setScale(shortButtonOffSprite.getScale());
    shortButtonOnSprite.setPosition(shortButtonOffSprite.getPosition());

    continueTextOffSprite.setTexture(continueTextOffTexture);
    continueTextOffSprite.setOrigin(continueTextOffSprite.getGlobalBounds().width / 2.f, continueTextOffSprite.getGlobalBounds().height / 2.f);
    continueTextOffSprite.setScale(0.3 * widthChange, 0.3 * heightChange);
    continueTextOffSprite.setPosition(longButtonOffSprite.getPosition());

    continueTextOnSprite.setTexture(continueTextOnTexture);
    continueTextOnSprite.setOrigin(continueTextOnSprite.getGlobalBounds().width / 2.f, continueTextOnSprite.getGlobalBounds().height / 2.f);
    continueTextOnSprite.setScale(0.25 * widthChange, 0.25 * heightChange);
    continueTextOnSprite.setPosition(longButtonOnSprite.getPosition());

    quitTextOffSprite.setTexture(quitTextOffTexture);
    quitTextOffSprite.setOrigin(quitTextOffSprite.getGlobalBounds().width / 2.f, quitTextOffSprite.getGlobalBounds().height / 2.f);
    quitTextOffSprite.setScale(continueTextOffSprite.getScale());
    quitTextOffSprite.setPosition(shortButtonOffSprite.getPosition());

    quitTextOnSprite.setTexture(quitTextOnTexture);
    quitTextOnSprite.setOrigin(quitTextOnSprite.getGlobalBounds().width / 2.f, quitTextOnSprite.getGlobalBounds().height / 2.f);
    quitTextOnSprite.setScale(continueTextOnSprite.getScale());
    quitTextOnSprite.setPosition(shortButtonOnSprite.getPosition());



    //infoFont.loadFromFile("Sprites\\Text screen\\font.otf");

    level1Text.setFont(*infoFont);
    level1Text.setString("Level 1");
    level1Text.setCharacterSize(50 * widthChange);
    level1Text.setOrigin(level1Text.getGlobalBounds().width / 2.f, level1Text.getGlobalBounds().height / 2.f);
    level1Text.setFillColor(Color(222, 199, 75));
    level1Text.setOutlineColor(Color(129, 97, 45));
    level1Text.setOutlineThickness(3 * widthChange);
    level1Text.setPosition(windowWidth / 2.f - (30 * widthChange), windowHeight / 8.f - (30 * heightChange));

    level2Text.setFont(*infoFont);
    level2Text.setString("Level 2");
    level2Text.setCharacterSize(50 * widthChange);
    level2Text.setOrigin(level2Text.getGlobalBounds().width / 2.f, level2Text.getGlobalBounds().height / 2.f);
    level2Text.setFillColor(Color(222, 199, 75));
    level2Text.setOutlineColor(Color(129, 97, 45));
    level2Text.setOutlineThickness(3 * widthChange);
    level2Text.setPosition(level1Text.getPosition());

    level3Text.setFont(*infoFont);
    level3Text.setString("Level 3");
    level3Text.setCharacterSize(50 * widthChange);
    level3Text.setOrigin(level3Text.getGlobalBounds().width / 2.f, level3Text.getGlobalBounds().height / 2.f);
    level3Text.setFillColor(Color(222, 199, 75));
    level3Text.setOutlineColor(Color(129, 97, 45));
    level3Text.setOutlineThickness(3 * widthChange);
    level3Text.setPosition(level1Text.getPosition());

    level4Text.setFont(*infoFont);
    level4Text.setString("Level 4");
    level4Text.setCharacterSize(50 * widthChange);
    level4Text.setOrigin(level4Text.getGlobalBounds().width / 2.f, level4Text.getGlobalBounds().height / 2.f);
    level4Text.setFillColor(Color(222, 199, 75));
    level4Text.setOutlineColor(Color(129, 97, 45));
    level4Text.setOutlineThickness(3 * widthChange);
    level4Text.setPosition(level1Text.getPosition());

    level1BorisText1.setFont(*infoFont);
    level1BorisText1.setString("Meet Boris the Butterfly Fish. Boris lives in");
    level1BorisText1.setCharacterSize(15 * widthChange);
    level1BorisText1.setFillColor(Color::White);
    level1BorisText1.setPosition(300 * widthChange, 120 * heightChange);

    level1BorisText2.setFont(*infoFont);
    level1BorisText2.setString("the warm waters of Sandy Shoal - a");
    level1BorisText2.setCharacterSize(15 * widthChange);
    level1BorisText2.setFillColor(Color::White);
    level1BorisText2.setPosition(level1BorisText1.getPosition().x, level1BorisText1.getPosition().y + level1BorisText2.getGlobalBounds().height + (5 * heightChange));

    level1BorisText3.setFont(*infoFont);
    level1BorisText3.setString("beautiful area of the Frenzy Coast. Enjoy");
    level1BorisText3.setCharacterSize(15 * widthChange);
    level1BorisText3.setFillColor(Color::White);
    level1BorisText3.setPosition(level1BorisText1.getPosition().x, level1BorisText2.getPosition().y + level1BorisText3.getGlobalBounds().height + (5 * heightChange));

    level1BorisText4.setFont(*infoFont);
    level1BorisText4.setString("the sights, but don't get too comfortable...");
    level1BorisText4.setCharacterSize(15 * widthChange);
    level1BorisText4.setFillColor(Color::White);
    level1BorisText4.setPosition(level1BorisText1.getPosition().x, level1BorisText3.getPosition().y + level1BorisText4.getGlobalBounds().height + (5 * heightChange));

    level1BorisText5.setFont(*infoFont);
    level1BorisText5.setString("A fish needs his lunch, and it's a");
    level1BorisText5.setCharacterSize(15 * widthChange);
    level1BorisText5.setFillColor(Color::White);
    level1BorisText5.setPosition(level1BorisText1.getPosition().x, level1BorisText4.getPosition().y + level1BorisText5.getGlobalBounds().height + (5 * heightChange));

    level1BorisText6.setFont(*infoFont);
    level1BorisText6.setString("fish-eat-fish world out there!");
    level1BorisText6.setCharacterSize(15 * widthChange);
    level1BorisText6.setFillColor(Color::White);
    level1BorisText6.setPosition(level1BorisText1.getPosition().x, level1BorisText5.getPosition().y + level1BorisText6.getGlobalBounds().height + (5 * heightChange));

    howToPlayTitle.setFont(*infoFont);
    howToPlayTitle.setString("How to play:");
    howToPlayTitle.setCharacterSize(18 * widthChange);
    howToPlayTitle.setFillColor(Color(184, 179, 87));
    howToPlayTitle.setPosition(level1BorisText1.getPosition().x - (10 + widthChange), level1BorisText6.getPosition().y + howToPlayTitle.getGlobalBounds().height + (5 * heightChange));

    howToPlayText1.setFont(*infoFont);
    howToPlayText1.setString("- Use your mouse to control Boris.");
    howToPlayText1.setCharacterSize(15 * widthChange);
    howToPlayText1.setFillColor(Color::White);
    howToPlayText1.setPosition(level1BorisText1.getPosition().x, howToPlayTitle.getPosition().y + howToPlayText1.getGlobalBounds().height + (5 * heightChange));

    howToPlayText2.setFont(*infoFont);
    howToPlayText2.setString("- Eat fish that are smaller than you.");
    howToPlayText2.setCharacterSize(15 * widthChange);
    howToPlayText2.setFillColor(Color::White);
    howToPlayText2.setPosition(level1BorisText1.getPosition().x, howToPlayText1.getPosition().y + howToPlayText2.getGlobalBounds().height + (5 * heightChange));

    howToPlayText3.setFont(*infoFont);
    howToPlayText3.setString("- Avoid anything that's larger than you.");
    howToPlayText3.setCharacterSize(15 * widthChange);
    howToPlayText3.setFillColor(Color::White);
    howToPlayText3.setPosition(level1BorisText1.getPosition().x, howToPlayText2.getPosition().y + howToPlayText3.getGlobalBounds().height + (5 * heightChange));

    howToPlayText4.setFont(*infoFont);
    howToPlayText4.setString("- Eat enough fish and you'll grow bigger!");
    howToPlayText4.setCharacterSize(15 * widthChange);
    howToPlayText4.setFillColor(Color::White);
    howToPlayText4.setPosition(level1BorisText1.getPosition().x, howToPlayText3.getPosition().y + howToPlayText4.getGlobalBounds().height + (5 * heightChange));

    borisSprite.setTexture(borisTexture);
    borisSprite.setOrigin(borisSprite.getGlobalBounds().width / 2.f, borisSprite.getGlobalBounds().height / 2.f);
    borisSprite.setScale(0.5 * widthChange, 0.5 * heightChange);
    borisSprite.setPosition(level1BorisText5.getPosition().x + level1BorisText1.getGlobalBounds().width + borisSprite.getGlobalBounds().width / 2.f + (50 * widthChange), level1BorisText4.getPosition().y);

    tip1Text.setFont(*infoFont);
    tip1Text.setString("Tip: Start with Minnows - they're the easiest prey.");
    tip1Text.setCharacterSize(13 * widthChange);
    tip1Text.setOrigin(tip1Text.getGlobalBounds().width / 2.f, tip1Text.getGlobalBounds().height / 2.f);
    tip1Text.setFillColor(Color::White);
    tip1Text.setPosition(window.getSize().x / 2.f, longButtonOffSprite.getPosition().y - longButtonOffSprite.getGlobalBounds().height / 2.f - tip1Text.getGlobalBounds().height / 2.f);

    orangeFishSprite.setTexture(orangeFishTexture);
    orangeFishSprite.setOrigin(orangeFishSprite.getGlobalBounds().width / 2.f, orangeFishSprite.getGlobalBounds().height / 2.f);
    orangeFishSprite.setScale(-0.45 * widthChange, 0.45 * heightChange);
    orangeFishSprite.setPosition(tip1Text.getPosition().x - tip1Text.getGlobalBounds().width / 4.f, tip1Text.getPosition().y - tip1Text.getGlobalBounds().height - orangeFishSprite.getGlobalBounds().height / 2.f + howToPlayText4.getGlobalBounds().height);

    blueFishSprite.setTexture(blueFishTexture);
    blueFishSprite.setOrigin(blueFishSprite.getGlobalBounds().width / 2.f, blueFishSprite.getGlobalBounds().height / 2.f);
    blueFishSprite.setScale(-0.25 * widthChange, 0.25 * heightChange);
    blueFishSprite.setPosition(orangeFishSprite.getPosition().x + orangeFishSprite.getGlobalBounds().width / 2.f + blueFishSprite.getGlobalBounds().width / 2.f, orangeFishSprite.getPosition().y);

    brownFishSprite.setTexture(brownFishTexture);
    brownFishSprite.setOrigin(brownFishSprite.getGlobalBounds().width / 2.f, brownFishSprite.getGlobalBounds().height / 2.f);
    brownFishSprite.setScale(-0.5 * widthChange, 0.5 * heightChange);
    brownFishSprite.setPosition(blueFishSprite.getPosition().x + blueFishSprite.getGlobalBounds().width / 2.f + brownFishSprite.getGlobalBounds().width, orangeFishSprite.getPosition().y + (15 * heightChange));

    level2BarracudaTitle.setFont(*infoFont);
    level2BarracudaTitle.setString("WARNING!");
    level2BarracudaTitle.setCharacterSize(18 * widthChange);
    level2BarracudaTitle.setFillColor(Color(184, 179, 87));
    level2BarracudaTitle.setPosition(howToPlayText1.getPosition().x, level1BorisText1.getPosition().y);

    level2BarracudaText1.setFont(*infoFont);
    level2BarracudaText1.setString("A BARRACUDA has been spotted cruising the Reef. Watch");
    level2BarracudaText1.setCharacterSize(15 * widthChange);
    level2BarracudaText1.setFillColor(Color::White);
    level2BarracudaText1.setPosition(level1BorisText2.getPosition().x, level2BarracudaTitle.getPosition().y + level2BarracudaTitle.getGlobalBounds().height + (10 * heightChange));

    level2BarracudaText2.setFont(*infoFont);
    level2BarracudaText2.setString("for warning signs, and keep away from his gaping maw! If");
    level2BarracudaText2.setCharacterSize(15 * widthChange);
    level2BarracudaText2.setFillColor(Color::White);
    level2BarracudaText2.setPosition(level1BorisText3.getPosition().x, level2BarracudaText1.getPosition().y + level2BarracudaText1.getGlobalBounds().height + (5 * heightChange));

    level2BarracudaText3.setFont(*infoFont);
    level2BarracudaText3.setString("you are feeling really lucky, try biting his tail. Bite it 4 times");
    level2BarracudaText3.setCharacterSize(15 * widthChange);
    level2BarracudaText3.setFillColor(Color::White);
    level2BarracudaText3.setPosition(level1BorisText3.getPosition().x, level2BarracudaText2.getPosition().y + level2BarracudaText2.getGlobalBounds().height + (5 * heightChange));

    level2BarracudaText4.setFont(*infoFont);
    level2BarracudaText4.setString("and you're in for a special surprise...");
    level2BarracudaText4.setCharacterSize(15 * widthChange);
    level2BarracudaText4.setFillColor(Color::White);
    level2BarracudaText4.setPosition(level1BorisText4.getPosition().x, level2BarracudaText3.getPosition().y + level2BarracudaText3.getGlobalBounds().height + (5 * heightChange));

    level2BarracudaText5.setFont(*infoFont);
    level2BarracudaText5.setString("if you survive that long");
    level2BarracudaText5.setCharacterSize(15 * widthChange);
    level2BarracudaText5.setFillColor(Color::White);
    level2BarracudaText5.setPosition(level1BorisText5.getPosition().x, level2BarracudaText4.getPosition().y + level2BarracudaText4.getGlobalBounds().height + (5 * heightChange));

    barracudaSprite.setTexture(barracudaTexture);
    barracudaSprite.setOrigin(barracudaSprite.getGlobalBounds().width / 2.f, barracudaSprite.getGlobalBounds().height / 2.f);
    barracudaSprite.setScale(widthChange, heightChange);
    barracudaSprite.setPosition(level2BarracudaText4.getPosition().x + level2BarracudaText4.getGlobalBounds().width + barracudaSprite.getGlobalBounds().width / 2.f - (20 * widthChange), level2BarracudaText5.getPosition().y + (40 * heightChange));

    level2HumpheadText1.setFont(*infoFont);
    level2HumpheadText1.setString("What's going on?");
    level2HumpheadText1.setCharacterSize(15 * widthChange);
    level2HumpheadText1.setFillColor(Color::White);
    level2HumpheadText1.setPosition(400 * widthChange, level2BarracudaText5.getPosition().y + (90 * heightChange));

    level2HumpheadText2.setFont(*infoFont);
    level2HumpheadText2.setString("These Humphead Wrasse don't usually");
    level2HumpheadText2.setCharacterSize(15 * widthChange);
    level2HumpheadText2.setFillColor(Color::White);
    level2HumpheadText2.setPosition(level2HumpheadText1.getPosition().x, level2HumpheadText1.getPosition().y + level2HumpheadText1.getGlobalBounds().height + (5 * heightChange));

    level2HumpheadText3.setFont(*infoFont);
    level2HumpheadText3.setString("venture in this close to the coast. This");
    level2HumpheadText3.setCharacterSize(15 * widthChange);
    level2HumpheadText3.setFillColor(Color::White);
    level2HumpheadText3.setPosition(level2HumpheadText1.getPosition().x, level2HumpheadText2.getPosition().y + level2HumpheadText2.getGlobalBounds().height + (5 * heightChange));

    level2HumpheadText4.setFont(*infoFont);
    level2HumpheadText4.setString("friendly little shoal is etting a it crowded!");
    level2HumpheadText4.setCharacterSize(15 * widthChange);
    level2HumpheadText4.setFillColor(Color::White);
    level2HumpheadText4.setPosition(level2HumpheadText1.getPosition().x, level2HumpheadText3.getPosition().y + level2HumpheadText3.getGlobalBounds().height + (5 * heightChange));

    humpheadSprite.setTexture(humpheadTexture);
    humpheadSprite.setOrigin(humpheadSprite.getGlobalBounds().width / 2.f, humpheadSprite.getGlobalBounds().height / 2.f);
    humpheadSprite.setScale(0.5 * widthChange, 0.5 * heightChange);
    humpheadSprite.setPosition(level2HumpheadText1.getPosition().x - humpheadSprite.getGlobalBounds().width / 2.f - (10 * widthChange), level2HumpheadText3.getPosition().y);

    tip2Text.setFont(*infoFont);
    tip2Text.setString("Tip: Barracuda and Lionfish are faster than they look - don't get cornered.");
    tip2Text.setCharacterSize(13 * widthChange);
    tip2Text.setOrigin(tip2Text.getGlobalBounds().width / 2.f, tip2Text.getGlobalBounds().height / 2.f);
    tip2Text.setFillColor(Color::White);
    tip2Text.setPosition(tip1Text.getPosition());

    level3EddieText1.setFont(*infoFont);
    level3EddieText1.setString("Meet Eddie the angler. Her small fins");
    level3EddieText1.setCharacterSize(15 * widthChange);
    level3EddieText1.setFillColor(Color::White);
    level3EddieText1.setPosition(level1BorisText1.getPosition());

    level3EddieText2.setFont(*infoFont);
    level3EddieText2.setString("make her a little slower, but she");
    level3EddieText2.setCharacterSize(15 * widthChange);
    level3EddieText2.setFillColor(Color::White);
    level3EddieText2.setPosition(level3EddieText1.getPosition().x, level3EddieText1.getPosition().y + level3EddieText1.getGlobalBounds().height + (5 * widthChange));

    level3EddieText3.setFont(*infoFont);
    level3EddieText3.setString("has a big bite! The deep sea is dark and");
    level3EddieText3.setCharacterSize(15 * widthChange);
    level3EddieText3.setFillColor(Color::White);
    level3EddieText3.setPosition(level3EddieText1.getPosition().x, level3EddieText2.getPosition().y + level3EddieText2.getGlobalBounds().height + (5 * widthChange));

    level3EddieText4.setFont(*infoFont);
    level3EddieText4.setString("dangerous, but Eddie will light the way.");
    level3EddieText4.setCharacterSize(15 * widthChange);
    level3EddieText4.setFillColor(Color::White);
    level3EddieText4.setPosition(level3EddieText1.getPosition().x, level3EddieText3.getPosition().y + level3EddieText3.getGlobalBounds().height + (5 * widthChange));

    eddieSprite.setTexture(eddieTexture);
    eddieSprite.setOrigin(eddieSprite.getGlobalBounds().width / 2.f, eddieSprite.getGlobalBounds().height / 2.f);
    eddieSprite.setScale(0.9 * widthChange, 0.9 * heightChange);
    eddieSprite.setPosition(level3EddieText3.getPosition().x + level3EddieText3.getGlobalBounds().width + eddieSprite.getGlobalBounds().width - (40 * widthChange), level3EddieText4.getPosition().y + (20 * heightChange));

    level3JohnDorryText1.setFont(*infoFont);
    level3JohnDorryText1.setString("Meet the John Dory. With his striking");
    level3JohnDorryText1.setCharacterSize(15 * widthChange);
    level3JohnDorryText1.setFillColor(Color::White);
    level3JohnDorryText1.setPosition(level3EddieText1.getPosition().x, level3EddieText4.getPosition().y + level3EddieText4.getGlobalBounds().height + (70 * heightChange));

    level3JohnDorryText2.setFont(*infoFont);
    level3JohnDorryText2.setString("yellow stripes and unique shape, he is");
    level3JohnDorryText2.setCharacterSize(15 * widthChange);
    level3JohnDorryText2.setFillColor(Color::White);
    level3JohnDorryText2.setPosition(level3EddieText1.getPosition().x, level3JohnDorryText1.getPosition().y + level3JohnDorryText1.getGlobalBounds().height + (5 * heightChange));

    level3JohnDorryText3.setFont(*infoFont);
    level3JohnDorryText3.setString("ready to chomp his way through the");
    level3JohnDorryText3.setCharacterSize(15 * widthChange);
    level3JohnDorryText3.setFillColor(Color::White);
    level3JohnDorryText3.setPosition(level3JohnDorryText1.getPosition().x, level3JohnDorryText2.getPosition().y + level3JohnDorryText2.getGlobalBounds().height + (5 * heightChange));

    level3JohnDorryText4.setFont(*infoFont);
    level3JohnDorryText4.setString("bustling waters of the reef. Keep your");
    level3JohnDorryText4.setCharacterSize(15 * widthChange);
    level3JohnDorryText4.setFillColor(Color::White);
    level3JohnDorryText4.setPosition(level3JohnDorryText1.getPosition().x, level3JohnDorryText3.getPosition().y + level3JohnDorryText3.getGlobalBounds().height + (5 * heightChange));

    level3JohnDorryText5.setFont(*infoFont);
    level3JohnDorryText5.setString("stomach full and your fins moving, because");
    level3JohnDorryText5.setCharacterSize(15 * widthChange);
    level3JohnDorryText5.setFillColor(Color::White);
    level3JohnDorryText5.setPosition(level3JohnDorryText1.getPosition().x, level3JohnDorryText4.getPosition().y + level3JohnDorryText4.getGlobalBounds().height + (5 * heightChange));

    level3JohnDorryText6.setFont(*infoFont);
    level3JohnDorryText6.setString("there's always a bigger fish waiting to");
    level3JohnDorryText6.setCharacterSize(15 * widthChange);
    level3JohnDorryText6.setFillColor(Color::White);
    level3JohnDorryText6.setPosition(level3JohnDorryText1.getPosition().x, level3JohnDorryText5.getPosition().y + level3JohnDorryText5.getGlobalBounds().height + (5 * heightChange));

    level3JohnDorryText7.setFont(*infoFont);
    level3JohnDorryText7.setString("turn you into their next snack!");
    level3JohnDorryText7.setCharacterSize(15 * widthChange);
    level3JohnDorryText7.setFillColor(Color::White);
    level3JohnDorryText7.setPosition(level3JohnDorryText1.getPosition().x, level3JohnDorryText6.getPosition().y + level3JohnDorryText6.getGlobalBounds().height + (5 * heightChange));

    johnDorrySprite.setTexture(johnDorryTexture);
    johnDorrySprite.setOrigin(johnDorrySprite.getGlobalBounds().width / 2.f, johnDorrySprite.getGlobalBounds().height / 2.f);
    johnDorrySprite.setScale(0.9 * widthChange, 0.9 * heightChange);
    johnDorrySprite.setPosition(level3JohnDorryText5.getPosition().x + level3JohnDorryText5.getGlobalBounds().width + johnDorrySprite.getGlobalBounds().width - (30 * widthChange), level3JohnDorryText4.getPosition().y);

    tip3Text.setFont(*infoFont);
    tip3Text.setString("Tip: Deep-sea angler fish are also called 'black devils' because of their frightening appearance.");
    tip3Text.setCharacterSize(13 * widthChange);
    tip3Text.setOrigin(tip3Text.getGlobalBounds().width / 2.f, tip3Text.getGlobalBounds().height / 2.f);
    tip3Text.setFillColor(Color::White);
    tip3Text.setPosition(tip1Text.getPosition());

    level4OrvilleText1.setFont(*infoFont);
    level4OrvilleText1.setString("Meet Orville the Orca. With his massive");
    level4OrvilleText1.setCharacterSize(15 * widthChange);
    level4OrvilleText1.setFillColor(Color::White);
    level4OrvilleText1.setPosition(level1BorisText1.getPosition());

    level4OrvilleText2.setFont(*infoFont);
    level4OrvilleText2.setString("size and sleek black-and-white colors, he");
    level4OrvilleText2.setCharacterSize(15 * widthChange);
    level4OrvilleText2.setFillColor(Color::White);
    level4OrvilleText2.setPosition(level4OrvilleText1.getPosition().x, level4OrvilleText1.getPosition().y + level4OrvilleText1.getGlobalBounds().height + (5 * heightChange));

    level4OrvilleText3.setFont(*infoFont);
    level4OrvilleText3.setString("is the undisputed king of the open ocean!");
    level4OrvilleText3.setCharacterSize(15 * widthChange);
    level4OrvilleText3.setFillColor(Color::White);
    level4OrvilleText3.setPosition(level4OrvilleText2.getPosition().x, level4OrvilleText2.getPosition().y + level4OrvilleText2.getGlobalBounds().height + (5 * heightChange));

    level4OrvilleText4.setFont(*infoFont);
    level4OrvilleText4.setString("Take control of this apex predator and chomp");
    level4OrvilleText4.setCharacterSize(15 * widthChange);
    level4OrvilleText4.setFillColor(Color::White);
    level4OrvilleText4.setPosition(level4OrvilleText3.getPosition().x, level4OrvilleText3.getPosition().y + level4OrvilleText3.getGlobalBounds().height + (5 * heightChange));

    level4OrvilleText5.setFont(*infoFont);
    level4OrvilleText5.setString("way to the top of the food chain, because now");
    level4OrvilleText5.setCharacterSize(15 * widthChange);
    level4OrvilleText5.setFillColor(Color::White);
    level4OrvilleText5.setPosition(level4OrvilleText4.getPosition().x, level4OrvilleText4.getPosition().y + level4OrvilleText4.getGlobalBounds().height + (5 * heightChange));

    level4OrvilleText6.setFont(*infoFont);
    level4OrvilleText6.setString("the whole ocean is your all-you-can-eat buffet!");
    level4OrvilleText6.setCharacterSize(15 * widthChange);
    level4OrvilleText6.setFillColor(Color::White);
    level4OrvilleText6.setPosition(level4OrvilleText5.getPosition().x, level4OrvilleText5.getPosition().y + level4OrvilleText5.getGlobalBounds().height + (5 * heightChange));

    orvilleSprite.setTexture(orvilleTexture);
    orvilleSprite.setOrigin(orvilleSprite.getGlobalBounds().width / 2.f, orvilleSprite.getGlobalBounds().height / 2.f);
    orvilleSprite.setScale(widthChange, heightChange);
    orvilleSprite.setPosition(window.getSize().x / 2.f, level4OrvilleText6.getPosition().y + level4OrvilleText6.getGlobalBounds().height + orvilleSprite.getGlobalBounds().height / 2.f);

    level4MineTitle.setFont(*infoFont);
    level4MineTitle.setString("DANGER: MINES!");
    level4MineTitle.setCharacterSize(18 * widthChange);
    level4MineTitle.setFillColor(Color(184, 179, 87));
    level4MineTitle.setPosition(orvilleSprite.getPosition().x - level4MineTitle.getGlobalBounds().width - (40 * widthChange), orvilleSprite.getPosition().y + orvilleSprite.getGlobalBounds().height / 2.f - (20 * heightChange));

    level4MineText1.setFont(*infoFont);
    level4MineText1.setString("Avoid mines that fall from the top of the");
    level4MineText1.setCharacterSize(15 * widthChange);
    level4MineText1.setFillColor(Color::White);
    level4MineText1.setPosition(level4MineTitle.getPosition().x + (10 * widthChange), level4MineTitle.getPosition().y + level4MineTitle.getGlobalBounds().height + (10 * heightChange));

    level4MineText2.setFont(*infoFont);
    level4MineText2.setString("screen. If you bump one of these touchy");
    level4MineText2.setCharacterSize(15 * widthChange);
    level4MineText2.setFillColor(Color::White);
    level4MineText2.setPosition(level4MineText1.getPosition().x, level4MineText1.getPosition().y + level4MineText1.getGlobalBounds().height + (5 * heightChange));

    level4MineText3.setFont(*infoFont);
    level4MineText3.setString("contraptions...KABOOM!");
    level4MineText3.setCharacterSize(15 * widthChange);
    level4MineText3.setFillColor(Color::White);
    level4MineText3.setPosition(level4MineText2.getPosition().x, level4MineText2.getPosition().y + level4MineText2.getGlobalBounds().height + (5 * heightChange));

    mineSprite.setTexture(mineTexture);
    mineSprite.setOrigin(mineSprite.getGlobalBounds().width / 2.f, mineSprite.getGlobalBounds().height / 2.f);
    mineSprite.setScale(1.2 * widthChange, 1.2 * heightChange);
    mineSprite.setPosition(level4MineText2.getPosition().x - mineSprite.getGlobalBounds().width / 2.f - (10 * widthChange), level4MineText2.getPosition().y + (10 * heightChange));

    tip4Text.setFont(*infoFont);
    tip4Text.setString("Tip: Mines don't care how big you are - one touch and you're done.");
    tip4Text.setCharacterSize(13 * widthChange);
    tip4Text.setOrigin(tip4Text.getGlobalBounds().width / 2.f, tip4Text.getGlobalBounds().height / 2.f);
    tip4Text.setFillColor(Color::White);
    tip4Text.setPosition(tip1Text.getPosition());

}

void menuTextBeforeLevels::textMenuLevel1(RenderWindow& window) {

    if (inTextMenuLevel1) {

        window.draw(*background_sprite);
        window.draw(textMenuBackgroundSprite);
        window.draw(textMenuBorderSprite);

        if (isOnContinue) {

            window.draw(longButtonOnSprite);
            window.draw(continueTextOnSprite);

        }

        else {

            window.draw(longButtonOffSprite);
            window.draw(continueTextOffSprite);

        }

        if (isOnQuit) {

            window.draw(shortButtonOnSprite);
            window.draw(quitTextOnSprite);

        }

        else {

            window.draw(shortButtonOffSprite);
            window.draw(quitTextOffSprite);

        }

        window.draw(level1Text);
        window.draw(level1BorisText1);
        window.draw(level1BorisText2);
        window.draw(level1BorisText3);
        window.draw(level1BorisText4);
        window.draw(level1BorisText5);
        window.draw(level1BorisText6);
        window.draw(howToPlayTitle);
        window.draw(howToPlayText1);
        window.draw(howToPlayText2);
        window.draw(howToPlayText3);
        window.draw(howToPlayText4);
        window.draw(borisSprite);
        window.draw(orangeFishSprite);
        window.draw(blueFishSprite);
        window.draw(brownFishSprite);
        window.draw(tip1Text);

    }

}

void menuTextBeforeLevels::textMenuLevel2(RenderWindow& window) {

    if (inTextMenuLevel2) {
        window.draw(*background_sprite);
        window.draw(textMenuBackgroundSprite);
        window.draw(textMenuBorderSprite);

        if (isOnContinue) {

            window.draw(longButtonOnSprite);
            window.draw(continueTextOnSprite);

        }

        else {

            window.draw(longButtonOffSprite);
            window.draw(continueTextOffSprite);

        }

        if (isOnQuit) {

            window.draw(shortButtonOnSprite);
            window.draw(quitTextOnSprite);

        }

        else {

            window.draw(shortButtonOffSprite);
            window.draw(quitTextOffSprite);

        }

        window.draw(level2Text);
        window.draw(level2BarracudaTitle);
        window.draw(level2BarracudaText1);
        window.draw(level2BarracudaText2);
        window.draw(level2BarracudaText3);
        window.draw(level2BarracudaText4);
        window.draw(level2BarracudaText5);
        window.draw(barracudaSprite);
        window.draw(level2HumpheadText1);
        window.draw(level2HumpheadText2);
        window.draw(level2HumpheadText3);
        window.draw(level2HumpheadText4);
        window.draw(humpheadSprite);
        window.draw(tip2Text);

    }

}

void menuTextBeforeLevels::textMenuLevel3(RenderWindow& window) {

    if (inTextMenuLevel3) {
        window.draw(*background_sprite);
        window.draw(textMenuBackgroundSprite);
        window.draw(textMenuBorderSprite);

        if (isOnContinue) {

            window.draw(longButtonOnSprite);
            window.draw(continueTextOnSprite);

        }

        else {

            window.draw(longButtonOffSprite);
            window.draw(continueTextOffSprite);

        }

        if (isOnQuit) {

            window.draw(shortButtonOnSprite);
            window.draw(quitTextOnSprite);

        }

        else {

            window.draw(shortButtonOffSprite);
            window.draw(quitTextOffSprite);

        }

        window.draw(level3Text);
        window.draw(level3EddieText1);
        window.draw(level3EddieText2);
        window.draw(level3EddieText3);
        window.draw(level3EddieText4);
        window.draw(eddieSprite);
        window.draw(level3JohnDorryText1);
        window.draw(level3JohnDorryText2);
        window.draw(level3JohnDorryText3);
        window.draw(level3JohnDorryText4);
        window.draw(level3JohnDorryText5);
        window.draw(level3JohnDorryText6);
        window.draw(level3JohnDorryText7);
        window.draw(johnDorrySprite);
        window.draw(tip3Text);

    }
}

void menuTextBeforeLevels::textMenuLevel4(RenderWindow& window) {

    if (inTextMenuLevel4) {
        window.draw(*background_sprite);
        window.draw(textMenuBackgroundSprite);
        window.draw(textMenuBorderSprite);

        if (isOnContinue) {

            window.draw(longButtonOnSprite);
            window.draw(continueTextOnSprite);

        }

        else {

            window.draw(longButtonOffSprite);
            window.draw(continueTextOffSprite);

        }

        if (isOnQuit) {

            window.draw(shortButtonOnSprite);
            window.draw(quitTextOnSprite);

        }

        else {

            window.draw(shortButtonOffSprite);
            window.draw(quitTextOffSprite);

        }

        window.draw(level4Text);
        window.draw(level4OrvilleText1);
        window.draw(level4OrvilleText2);
        window.draw(level4OrvilleText3);
        window.draw(level4OrvilleText4);
        window.draw(level4OrvilleText5);
        window.draw(level4OrvilleText6);
        window.draw(orvilleSprite);
        window.draw(level4MineTitle);
        window.draw(level4MineText1);
        window.draw(level4MineText2);
        window.draw(level4MineText3);
        window.draw(mineSprite);
        window.draw(tip4Text);

    }

}

void menuTextBeforeLevels::textMenucontrols(RenderWindow& window, Event& event) {

    Vector2i pixelPosition = Mouse::getPosition(window);

    Vector2f worldPosition = window.mapPixelToCoords(pixelPosition);

    if (longButtonOffSprite.getGlobalBounds().contains(worldPosition) || longButtonOnSprite.getGlobalBounds().contains(worldPosition)) {

        isOnContinue = true;
        isOnQuit = false;

    }

    else if (shortButtonOffSprite.getGlobalBounds().contains(worldPosition) || shortButtonOnSprite.getGlobalBounds().contains(worldPosition)) {

        isOnContinue = false;
        isOnQuit = true;

    }

    else {

        isOnContinue = false;
        isOnQuit = false;

    }

    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
        leftMouseClicked = true;

    if (isOnContinue && leftMouseClicked) {



        if (state == 3) {

            state = 4;
            inTextMenuLevel1 = false;
            inTextMenuLevel2 = false;
            inTextMenuLevel3 = false;
        }

        else if (state == 6) {
            state = 7;
            inTextMenuLevel1 = false;
            inTextMenuLevel2 = false;
            inTextMenuLevel3 = false;
        }

        // else if (state == 8) {

        //     state = 9;
        //     inTextMenuLevel1 = false;
        // 	inTextMenuLevel2 = false;
        // 	inTextMenuLevel3 = false;
        // }

        else if (state == 8) {
            state = 9;
            inTextMenuLevel1 = false;
            inTextMenuLevel2 = false;
            inTextMenuLevel3 = false;
            inTextMenuLevel4 = false;

        }

    }

    if (isOnQuit && leftMouseClicked) {

        state = 2;

    }

    leftMouseClicked = false;

}

void level2Intialization() {
    loadAssets(level2assets, "background level 3.png", "level sprites/Anglerfish main pl.png", "level sprites/Minnow.png",
        "level sprites/John Dory.png", "level sprites/Marlin.png", "level sprites/Barracuda.png", "level sprites/dangerSign.png", "level sprites/Mermaid.png");
    if (level2assets.playerTex.getSize().x == 0 ||
        level2assets.smallTex.getSize().x == 0 ||
        level2assets.medTex.getSize().x == 0 ||
        level2assets.bigTex.getSize().x == 0) {
        cout << "LEVEL ASSETS MISSING - check file paths!" << endl;
        return;
    }
    setupLevel(2, level2assets, level2smallFishes, level2medFishes, level2bigFishes, level2Player, shark, mermaid, 15, 4, 6,
        7, 15, 5, 0, 15, 30, 45, 15, 2, 0, 0, 7, 0, 0, 15, 3,
        3, 6, 15, 5, 0, 15, 30, 15, 4, 5, 15, 5, 0, 15, 30);

    level2Intialized = true;
}

void level2Update(float dt, RenderWindow& window)
{
    int smallN = 15;
    int medN = 8;
    int bigN = 3;

    if (!level2Intialized) {
        resetgames(window, 0, level2assets, level2smallFishes, level2medFishes, level2bigFishes, level2Player, smallN, medN, bigN);
    }
    for (int i = 0; i < smallN; i++) {
        updateCommonLogic(level2smallFishes[i].speed, dt, window, smallN, level2smallFishes, level2Player, level2smallFishes[i], shark, preal);
    }
    for (int i = 0; i < medN; i++) {
        updateCommonLogic(level2medFishes[i].speed, dt, window, medN, level2medFishes, level2Player, level2medFishes[i], shark, preal);
    }
    for (int i = 0; i < bigN; i++) {
        updateCommonLogic(level2bigFishes[i].speed, dt, window, bigN, level2bigFishes, level2Player, level2bigFishes[i], shark, preal);
    }
}

void level2draw(RenderWindow& window)
{
    window.draw(level2assets.background);
    for (int i = 0; i < 15; i++)
    {
        if (!level2smallFishes[i].aten)
            window.draw(level2smallFishes[i].swim.sprite);
    }
    for (int i = 0; i < 8; i++)
    {
        if (!level2medFishes[i].aten)
            window.draw(level2medFishes[i].anim.sprite);
    }
    for (int i = 0; i < 3; i++)
    {
        if (!level2bigFishes[i].aten)
            window.draw(level2bigFishes[i].anim.sprite);
    }
    //if (mermaidActive) mermaid.draw_mermaid(window);
    if (level2Player.isalive)
        window.draw(level2Player.anim.sprite);


}

void level3Intialization() {

    loadAssets(level3Assets, "Sprites\\level3\\background.png", "Sprites\\level3\\player.png", "Sprites\\level3\\smallFish.png", "Sprites\\level3\\mediumFish.png", "Sprites\\level3\\bigFish.png", "Sprites\\level3\\barracuda.png", "Sprites\\level3\\signal.png", "Sprites\\level3\\mermaid.png");

    setupLevel(3, level3Assets, level3SmallFishes, level3MedFishes,
        level3BigFishes, level3Player, shark, mermaid, 15, 4, 6, 7, 15,
        5, 0, 15, 30, 45, 6, 7, 6, 28, 6, 0, 6, 34, 15, 3, 6, 15, 5, 0,
        15, 30, 14, 3, 6, 14, 5, 0, 14, 28);

    bar.start(3);
}

void level3Update(float dt, RenderWindow& window) {

    int smallN = 20;
    int medN = 15;
    int bigN = 5;

    if (!level3Intialized) {

        resetgames(window, 1, level3Assets, level3SmallFishes,
            level3MedFishes, level3BigFishes, level3Player,
            smallN, medN, bigN);

    }

    for (int i = 0; i < smallN; i++) {

        updateCommonLogic(level3SmallFishes[i].speed, dt, window, smallN, level3SmallFishes, level3Player, level3SmallFishes[i], shark, preal);

    }

    for (int i = 0; i < medN; i++) {

        updateCommonLogic(level3MedFishes[i].speed, dt, window, medN, level3MedFishes, level3Player, level3MedFishes[i], shark, preal);

    }

    for (int i = 0; i < bigN; i++) {

        updateCommonLogic(level3BigFishes[i].speed, dt, window, bigN, level3BigFishes, level3Player, level3BigFishes[i], shark, preal);

    }

}

