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


struct Score {
    int score = 0, bouns = 1, smallFscore = 10, medFscore = 20, bigFscore = 50,
    taileBscore = 100, starscore = 60, prealscore = 50,
    scale = 0, lives = 3, trials = 3,
    livelnum = 1,
    currentFrenzy = 0, i = 0; //for counting frenzy word
    float speedP = 800, speedE = 100;

    int FRENZYi[7]{ 0,0,0,0,0,0,0 };
    float dt, timer = 0;

    bool lessThanSec, medFishCanBeAten = 0, bigFishCanBeAten = 0;


    Text FRENZYiText[7], scoretext;
    Font font;
    string word = "FRENZY!";
};

struct GameSettings {
    short sound_volume = 100, music_volume = 100;
    bool controls[3] ={1,1,1}; // 0 -> sfx, 1-> music, 2->full screen, 3 -> 0
    //bool sfx = 1, music = 1, full_screen = 1, font = 0;
    Vector2f res = {1920,1080};
};

    Score score; GameSettings settings;

struct AnimatedSprite {
    Sprite sprite;
    int frameW = 0, frameH = 0;
    int countframe = 0, currentframe = 0;
    float anitimer = 0;
    float speed = 100;

    void setup(Texture& t, int cols, int rows, int frames, int rowIndex) {
        sprite.setTexture(t);
        frameW = t.getSize().x / cols;
        frameH = t.getSize().y / rows;
        countframe = frames;
        currentframe = 0;
        anitimer = 0;
        sprite.setOrigin(frameW / 2, frameH / 2);
        sprite.setTextureRect(IntRect(0, rowIndex * frameH, frameW, frameH));
    }

    void animate(float dt, int rowIndex) {
        anitimer += dt;
        if (anitimer > 0.1f) {
            currentframe = (currentframe + 1) % countframe;
            sprite.setTextureRect(IntRect(currentframe * frameW, rowIndex * frameH, frameW, frameH));
            anitimer = 0;
        }
    }
};

struct setplayer {
    AnimatedSprite anim;
    bool ate = false, dead = false, aten = false, draw = true, isalive = true, iseating = false;
    float dir = 1;
    int size = 0;
    float eatT = 0;
};

struct fishes {
    AnimatedSprite anim;
    bool aten = false, ate = false, playerintersectfish = false, draw = true, isalive = true;
    float endx = 0, direction = 1, basespeed = 100;
fishes() {}

fishes(Texture& t, int f, int c, int r, int RI) {
    anim.setup(t, c, r, f, RI);
    isalive = true;
    endx = rand() % 1300 + 300;
    direction = 1;
}
};

struct Shark {
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
    void sharkMovement(float speed, float deltaTime);
    void sharkEntrance(float& timer, int& flashCount, float deltaTime, Vector2f startPosition, bool& finished);
    void sharkAnimation(int frame, int row, int frameCountInEveryRow);
};

Vector2f enemyMouthposition(fishes& fish);

bool enemymouthintersect(setplayer& player, fishes& enemy);

bool isfront(setplayer& player, fishes& enemy);

bool enemyisfront(setplayer& player, fishes& enemy);

void cameraMovement(View& camera, Vector2f playerPosition, float BGWidth, float BGHeight);

void intersection(setplayer& player, fishes& smallFish, fishes& medFish, fishes& bigFish);

void window_mode(RenderWindow &window, bool full_screen);

void addfishs(Texture& t, int cols, int rows, int frames, int rowindx,const int n);

void setback(Sprite& sp, Texture& t);

void resetgames(RenderWindow& window);

void Fishmovement(float dt, fishes& fish, int rowindex, setplayer& player);

void playermovement(float dt, setplayer& isplayer, RenderWindow& window);

Vector2f Mouthposition(setplayer& player);

bool mouthintersect(setplayer& player, fishes& enemy) {
    Vector2f mouth = Mouthposition(player), enemypos = enemy.anim.sprite.getPosition();
    float dist = sqrt((mouth.x - enemypos.x) * (mouth.x - enemypos.x) + (mouth.y - enemypos.y) * (mouth.y - enemypos.y));
    float  radius = enemy.anim.frameW / 2;
    return dist < radius;
}
//int start();

void Sorry(setplayer&player, RenderWindow& window, float dt);

int draw(setplayer&player, RenderWindow& window, float dt);

int scoring(Shark& shark, float dt);

void playermovement(float dt, fishes& fish, Sprite& sprite, Texture& t,RenderWindow& window);

void dead();

void ContinueY_N(RenderWindow& window);

//int mouthintersect(fishes& a, fishes& b);

struct commonAssets {

    Font game_font;
    Texture main_menu_background_text, menu_rocks_text;
    Sprite main_menu_background_sprite, menu_rocks_sprite;

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
    Sprite buttons_sprite[3] , stageSprite, backgroundSprite;

    RenderWindow* window = nullptr;

    short selectedOption = 0;

    bool pause = true;

    void load();

    void selection(short &scene);

    void drawBackground();

    void draw();

    void eventHandler(Event& event, short &scene);

};

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

void pauseMenu::selection(short &scene) {

    if (selectedOption == -1) {
        return;
    }

    if (selectedOption == 0) {
        pause = false;
    }

    if (selectedOption == 2) {
        scene = 0;
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

void pauseMenu::eventHandler(Event& event, short &scene) {

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
            selection(scene);
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
            selection(scene);
        }
    }
};

struct helpAndOptions {

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

    RenderWindow* window = nullptr;
    menuButtons buttons[7];
    Texture buttons_texture[6][2], menu_shell_texture[6],Rarrow_texture[1][2], Larrow_texture[1][2], mouse_texture, reft;
    Sprite buttons_sprite[6][2],menu_shell_sprite[6],Rarrow_sprite[1][2], Larrow_sprite[1][2],mouse_sprite, refs;
    string settings_string[4] = {"Sound:", "Music:", "Full Screen:", "Mouse Speed:"}, 
    how_to_play_string[2] {"Welcome to the beautiful Frenzy Coast.\nEnjoy the sights, but don't get too comfortable...\nA fish still needs his lunch, and it's a fish-eat-fish\nworld out there!",
        "- Use your mouse to control Boris.\n- Eat fish that are smaller than you.\n- Avoid anything that's larger than you.\n- Eat enough fish and you'll grow bigger!\n"};
    Text settings_text[4],how_to_play_text[2], sound_volume, music_volume;
    commonAssets* assets = nullptr;
    gameSounds* sound = nullptr;

    short selected = -1, options_scene = 0; // main-> 0, controls -> 1, how to play -> 3, settings -> 2, credits -> 4

    void load_assets();

    void handle_movements(Event& event, short &scene);

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
    gameSounds *sound;
    short selected =-1, menu_scene =0;
    // scenes 0 -> click to start, 1 -> main menu, 2-> single player, 3-> multiplayer, 4-> leaderboards
    // 5 -> achievements, 6->help, 7-> quit 
    // 8 -> story mode, 9 - > story lite, 10 -> time attack, 
    // settings = 11, controls = 12, how to play = 13, credits = 14
    Texture logo_texture,main_menu_buttons_texture[6][2], reft, txt,
    tool_tip_texture,tube_texture, ment, rr,
    single_player_texture[4][2], quads_texture[6], the_sign_texture;
    Sprite main_menu_buttons_sprite[6][2], logo_sprite,
    tool_tip_sprite, tube_sprite, mens, refs, txts, rrs, single_player_sprite[4][2], quads_sprite[6], the_sign_sprite;

    string menu_strings[6] = {"Single Player Game Modes","Multiplayer Game Modes","View Leaderboards", "See Your Achievements", "Adjust Sound and Music Settings Or Learn How tp Play", "Return to Desktop"};
    Text menu_text[6];
    Font menu_font;
    commonAssets* assets = nullptr;

    void load_assets();

    void logo_transformation(char a);

    void handle_movements(Event& event, short &scene );

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

struct levelsmap{

    struct Buttons{
        Texture buttons_text[2][2];
        Sprite buttons_sprite[2][2];

        // 0,0 -> msh selected 3ady | 0,1 selected | 1,0 done | 1,1 done w selected
        void load()
        {
            if (!buttons_text[0][0].loadFromFile("Sprites/menu/gameMap_mapPinOffLg.png")) cout << "Error loading refs" << endl;
            if (!buttons_text[0][1].loadFromFile("Sprites/menu/map_menu_hoov.png")) cout << "Error loading refs" << endl;
            if (!buttons_text[1][0].loadFromFile("Sprites/menu/gameMap_noExport.png")) cout << "Error loading refs" << endl;
            if(!buttons_text[1][1].loadFromFile("Sprites/menu/gameMap_mapPinOnLg.png")) cout << "Error loading refs" << endl;

            for(int i =0; i < 2; i++)
                for(int j = 0; j < 2; j++)
                {
                    buttons_sprite[i][j].setTexture(buttons_text[i][j]);
                    buttons_sprite[i][j].setOrigin(buttons_sprite[i][j].getGlobalBounds().width/2,buttons_sprite[i][j].getGlobalBounds().height/2);
                    buttons_sprite[i][j].setScale(1.4f,1.4f);
                }
        }
    };

    RenderWindow *window = nullptr;
    Texture map_texture, tool_tip_texture,ref;
    Sprite map_sprite, tool_tip_sprite,refs;
    Buttons buttons[3];
    short selected = -1;

    void load()
    {
        if (!ref.loadFromFile("Sprites/menu/mapref.png")) cout << "Error loading refs" << endl;
        refs.setTexture(ref);
        refs.setOrigin(refs.getGlobalBounds().width/2,refs.getGlobalBounds().height/2);
        refs.setPosition(settings.res.x/2,settings.res.y/2);

        if (!map_texture.loadFromFile("Sprites/menu/mpa-menu1.png")) cout << "Error loading map" << endl;
        map_sprite.setTexture(map_texture);
        map_sprite.setOrigin(map_sprite.getGlobalBounds().width/2,map_sprite.getGlobalBounds().height/2);
        map_sprite.setPosition(settings.res.x/2,settings.res.y/2);

        for(int i =0; i < 3 ;i++) 
        {
            buttons[i].load();
            for(int j = 0; j < 2; j++)
            {
                for(int h = 0; h < 2; h++)
                {
                if(i == 0)
                    buttons[i].buttons_sprite[j][h].setPosition(settings.res.x/2 - 247, settings.res.y /2 - 315.5f);
                else if(i == 1)
                    buttons[i].buttons_sprite[j][h].setPosition(settings.res.x/2 - 500, settings.res.y /2 + 15);
                else
                    buttons[i].buttons_sprite[j][h].setPosition(settings.res.x/2 - 150, settings.res.y /2 + 60);
                }
            }
        }
        // if (!tool_tip_texture.loadFromFile("Sprites\\menu\\gameMap_text.png")) cout << "tooltip's texture is not found" << endl;
        // tool_tip_sprite.setTexture(tool_tip_texture);
        // tool_tip_sprite.setOrigin(tool_tip_sprite.getGlobalBounds().width / 2, tool_tip_sprite.getGlobalBounds().height / 2);
        // tool_tip_sprite.setPosition(settings.res.x / 2 , settings.res.y / 2 + 360);
        // //tool_tip_sprite.setScale((settings.res.x / 2) / tool_tip_texture.getSize().x, (settings.res.y * 0.19f) / tool_tip_texture.getSize().y);
        // tool_tip_sprite.setScale(1.8f,1.8f);
    }

    void handle_movements(Event& event,short &scene)
    {
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Escape)
                scene = 0;
        }
        if (event.type == Event::MouseMoved)
        {
            Vector2i mousePixel = Mouse::getPosition(*window);
            Vector2f mouse_position = window->mapPixelToCoords(mousePixel);
            for(int i =0; i < 3; i++)
            {
                if(buttons[i].buttons_sprite[0][0].getGlobalBounds().contains(mouse_position))
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
            if(event.mouseButton.button == Mouse::Left)
            {
                if(selected == 0)
                    scene = 3;
            }
        }
    }

    void draw()
    {
        window->clear();
        //window->draw(refs);
        window->draw(map_sprite);
        for(int i =0; i < 3;i++)
            window->draw(buttons[i].buttons_sprite[0][selected == i ? 1 : 0]);
        window->display();
    }
};

struct menuTextBeforeLevels {

	bool leftMouseClicked = false;
	bool inTextMenuLevel1 = true;
	bool inTextMenuLevel2 = false;
	bool inTextMenuLevel3 = false;
	bool inTextMenuLevel4 = false;
	bool isOnContinue = false;
	bool isOnQuit = false;

    

	Texture textMenuBorderTexture, textMenuBackgroundTexture, longButtonOffTexture, longButtonOnTexture, shortButtonOffTexture, shortButtonOnTexture, continueTextOffTexture, continueTextOnTexture, quitTextOffTexture, quitTextOnTexture;
	Texture borisTexture, orangeFishTexture, blueFishTexture, brownFishTexture, barracudaTexture, humpheadTexture, eddieTexture, johnDorryTexture, orvilleTexture, mineTexture;

	Sprite textMenuBorderSprite, textMenuBackgroundSprite, longButtonOffSprite, longButtonOnSprite, shortButtonOffSprite, shortButtonOnSprite, continueTextOffSprite, continueTextOnSprite, quitTextOffSprite, quitTextOnSprite;
	Sprite borisSprite, orangeFishSprite, blueFishSprite, brownFishSprite, barracudaSprite, humpheadSprite, eddieSprite, johnDorrySprite, orvilleSprite, mineSprite;

	Font *infoFont;

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

	void textMenucontrols(RenderWindow& window, short &scene, Event& event);

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


Cont Continue;
SORRY sorry;
fishes smallFish, medFish, bigFish;
setplayer player;
Preal preal;
fishes enemy[100];

int main()
{
    mainMenu* menu = nullptr;
    helpAndOptions* help = nullptr;
    RenderWindow window;
    pauseMenu* pause = nullptr;
    //pause->load();
    levelsmap* map = nullptr;
    if(!map)
    {
        map = new levelsmap();
        map->window = &window;
        map->load();
    }
    menuTextBeforeLevels texts_before_levels;
    commonAssets assets;
    assets.load();
    window_mode(window, settings.controls[2]);
    texts_before_levels.infoFont = &assets.game_font;
    texts_before_levels.textMenuIntialization(window);
    Event event;
    Image icon;
    icon.loadFromFile("Sprites\\icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    short scene = 0; // 0 for start_mermaid menu, 2 -> mermaid animation, 3-> shark
    Clock clock;
    Mermaid mermaid; Shark shark;
    gameSounds sounds;
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
            if(scene == 0 && menu)
                menu->handle_movements(event, scene);
            else if(scene == 1 && help)
                help->handle_movements(event, scene);
            else if(pause && pause->pause)
                pause->eventHandler(event, scene);
            else if(scene == 2)
                map->handle_movements(event, scene);
            else if(scene == 3)
                texts_before_levels.textMenucontrols(window, scene, event);

            // if (event.type == Event::KeyPressed)
            // {
            // }
        }


        switch (scene)
        {
        case 0:
            if (!menu)
            {
                menu = new mainMenu();
                //if(help)
                    //menu-> menu_scene = 1;//, delete help, help = nullptr;
                menu->assets = &assets;
                menu->sound = &sounds;
                menu->window = &window;
                menu->load_assets();
            }
            menu->update_menu_scenes();
            break;
        case 1:
            if(!help)
            {
                help = new helpAndOptions();
                //if(menu)
                //    delete menu, menu = nullptr;
                help->assets = &assets;
                help->window = &window;
                help->sound = &sounds;
                help->load_assets();
            }
            help->update_menu_scenes();
            break;
        case 2: // map
            map->draw();
            break;
        case 3: // menu text
            window.clear();
            texts_before_levels.textMenuLevel1(window);
            window.display();
            break;
        case 4: // level 1
            // window.clear();
            // pause->draw();
            // window.display();
        //     break;
        // case 5: 
            mermaid.update_mermaid(dt);
            mermaid.draw_mermaid(window);
            break;
        case 6:
            shark.update(dt);
            shark.draw(window);
            break;
        }
    }


    return 0;
}




void addfishs(Texture& t, int cols, int rows, int frames, int rowindx,const int n)
{
    for (int i = 0; i < n; i++) {
        enemy[i] = fishes(t, frames, cols, rows, rowindx);
        enemy[i].anim.sprite.setPosition(rand() % 800,300+ rand() % 600);
        enemy[i].anim.speed = rand() % 100 + 100; 
        enemy[i].basespeed = enemy[i].anim.speed;
    }
}

void intersection(setplayer& player, fishes& smallFish, fishes& medFish, fishes& bigFish) {
    if (player.aten) player.draw = false;

    if (player.anim.sprite.getGlobalBounds().intersects(smallFish.anim.sprite.getGlobalBounds()) && !smallFish.aten && player.draw && mouthintersect(player, smallFish) && isfront(player, smallFish)) {
        smallFish.aten = 1;
        smallFish.draw = 0;
        player.iseating = true;
        player.eatT = .2f;
        player.ate = 1;
            score.score += score.smallFscore * score.bouns;
    }

    if (player.anim.sprite.getGlobalBounds().intersects(medFish.anim.sprite.getGlobalBounds()) && !medFish.aten && player.draw && enemymouthintersect(player, medFish) && enemyisfront(player, medFish)) {
        if (player.anim.sprite.getScale().x >= medFish.anim.sprite.getScale().x) {
            medFish.aten = 1;
            medFish.draw = 0;
            player.ate = 1;
            player.iseating = true;
            player.eatT = .2f;
                score.score += score.medFscore * score.bouns;
                player.anim.sprite.setTextureRect(IntRect(player.anim.frameW * player.anim.currentframe, 0, player.anim.frameW, player.anim.frameH));
        }
        else {
            medFish.ate = 1;
            player.aten = 1;
        }

    }

    if (player.anim.sprite.getGlobalBounds().intersects(bigFish.anim.sprite.getGlobalBounds()) && !bigFish.aten && player.draw && enemymouthintersect(player, bigFish) && enemyisfront(player, bigFish)) {
        if (player.anim.sprite.getScale().x >= bigFish.anim.sprite.getScale().x) {
            bigFish.aten = 1;
            bigFish.draw = 0;
            player.ate = 1;
            player.iseating = true;
            player.eatT = .2f;
                score.score += score.bigFscore * score.bouns;
        }
        else {
            bigFish.ate = 1;
            player.aten = 1;
        }
    }
}

void Fishmovement(float dt, fishes& fish, int rowindex, setplayer& player) {

    if (player.anim.sprite.getScale().x > fish.anim.sprite.getScale().x && abs(player.anim.sprite.getPosition().x - fish.anim.sprite.getPosition().x) < 80) {
        if (player.anim.sprite.getPosition().x < fish.anim.sprite.getPosition().x) {
            fish.direction = 1;
            fish.endx = rand() % 1300 + 300;
        }
        else {
            fish.direction = -1;
            fish.endx = rand() % 400;
        }

    }
    else
        fish.anim.speed = fish.basespeed;

    //fish.animate(dt, rowindex);
    fish.anim.sprite.move(fish.anim.speed * fish.direction * dt, 0);
    if (fish.direction == 1 && fish.anim.sprite.getPosition().x >= fish.endx) {
        fish.direction = -1;
        fish.endx = rand() % 400;
    }
    if (fish.direction == -1 && fish.anim.sprite.getPosition().x <= fish.endx) {
        fish.direction = 1;
        fish.endx = rand() % 1300 + 300;
    }
    //fish.sprite.setScale(fish.direction, 1.f);
    if (fish.direction == 1) {
        fish.anim.currentframe = 1;
    }
    else {
        fish.anim.currentframe = 0;
    }

    fish.anim.sprite.setTextureRect(IntRect(
        fish.anim.currentframe * fish.anim.frameW,
        rowindex * fish.anim.frameH,
        fish.anim.frameW,
        fish.anim.frameH
    ));
    //fish.sprite.move(dt * speedE, 0);
}

Vector2f Mouthposition(setplayer& player) {
    Vector2f pos = player.anim.sprite.getPosition();
    float offset = player.anim.frameW / 2;
    if (player.anim.currentframe == 1) return Vector2f(pos.x + offset, pos.y);
    else
        return Vector2f(pos.x - offset, pos.y);
}

Vector2f enemyMouthposition(fishes& fish) {
    Vector2f pos = fish.anim.sprite.getPosition();
    float offset = fish.anim.frameW / 2;
    if (fish.anim.currentframe == 1) return Vector2f(pos.x + offset, pos.y);
    else
        return Vector2f(pos.x - offset, pos.y);
}

bool enemymouthintersect(setplayer& player, fishes& enemy) {
    Vector2f mouth = enemyMouthposition(enemy), enemypos = player.anim.sprite.getPosition();
        float dist = sqrt((mouth.x - enemypos.x) * (mouth.x - enemypos.x) + (mouth.y - enemypos.y) * (mouth.y - enemypos.y));
    float  radius = player.anim.frameW / 2;
    return dist < radius;
}

bool isfront(setplayer& player, fishes& enemy) {
    float dx = enemy.anim.sprite.getPosition().x - player.anim.sprite.getPosition().x;
    if (player.dir == 1)
        return dx > 0;
    else
        return dx < 0;
}

bool enemyisfront(setplayer& player, fishes& enemy) {
    float dx = enemy.anim.sprite.getPosition().x - player.anim.sprite.getPosition().x;
    if (player.anim.currentframe == 1)
        return dx > 0;
    else
        return      dx < 0;
}

void resetgames(RenderWindow& window) {
        float speedP = 800; //p for player
        float speedE = 100; //E for enemy
    Vector2f   target = window.mapPixelToCoords(Mouse::getPosition(window));
}

void setback(Sprite& sp, Texture& t) { // to make background suit with window
    sp.setTexture(t);
    sp.setScale(
        1600.f / t.getSize().x,
        900.f / t.getSize().y
    );
}

// int start() {

//     //test stuff//
//     player.frameW = 356 / 2;
//     player.frameH = 167;

//     smallFish.frameW = 127;
//     smallFish.frames = 105;

//     medFish.frameW = 127;
//     medFish.frames = 105;

//     bigFish.frameW = 127;
//     bigFish.frames = 105;

//     smallFish.texture.loadFromFile("Assets\\Untitled-1.png");
//     smallFish.sprite.setTexture(smallFish.texture);
//     smallFish.sprite.setPosition(900, 100);
//     smallFish.sprite.setScale(.7, .7);

//     medFish.texture.loadFromFile("Assets\\Untitled-1.png");
//     medFish.sprite.setTexture(medFish.texture);
//     medFish.sprite.setPosition(900, 300);
//     medFish.sprite.setScale(1.2, 1.2);

//     bigFish.texture.loadFromFile("Assets\\Untitled-1.png");
//     bigFish.sprite.setTexture(bigFish.texture);
//     bigFish.sprite.setPosition(900, 500);
//     bigFish.sprite.setScale(2, 2);
//     //  //


//     player.size = 15; // then 35 then 55
//     smallFish.size = 10;
//     medFish.size = 30;
//     bigFish.size = 50;

//     smallFish.score = 10;
//     medFish.score = 20;
//     bigFish.score = 50;

//     player.texture.loadFromFile("Assets\\fish.png");
//     player.sprite.setTexture(player.texture);
//     player.sprite.setPosition(320, 450);
//     player.sprite.setTextureRect(IntRect(0, 0, player.frameW, player.frameH));
//     player.sprite.setScale(1.0f, 1.0f);


//     score.font.loadFromFile("Assets\\font.TTF");
//     for (int q = 0; q < 7; q++) {
//         score.FRENZYiText[q].setFont(score.font);
//         score.FRENZYiText[q].setPosition(200 + 20 * q, 200);
//         score.FRENZYiText[q].setString(string(1, score.word[q]));
//         score.FRENZYiText[q].setCharacterSize(40);
//     }
//     score.scoretext.setFont(score.font);
//     score.scoretext.setPosition(300, 300);
//     score.scoretext.setString(to_string(score.score));
//     score.scoretext.setCharacterSize(30);


//     sorry.textures[0].loadFromFile("Assets\\s.jpeg");
//     sorry.textures[1].loadFromFile("Assets\\o.jpeg");
//     sorry.textures[2].loadFromFile("Assets\\r1.jpeg");
//     sorry.textures[3].loadFromFile("Assets\\r2.jpeg");
//     sorry.textures[4].loadFromFile("Assets\\y.jpeg");

//     for (int w = 0; w < 5; w++) {
//         sorry.sprites[w].setTexture(sorry.textures[w]);
//         sorry.sprites[w].setPosition(80, 200 + 10 * w);
//     }


// 	Continue.CONTtexture.loadFromFile("Assets\\CONT.png");
// 	Continue.CONTsprite.setTexture(Continue.CONTtexture);
// 	Continue.YESt[0].loadFromFile("Assets\\nonactive yes.png");
// 	Continue.YESt[1].loadFromFile("Assets\\active yes.png");
// 	Continue.YESsprite[0].setTexture(Continue.YESt[0]);
// 	Continue.YESsprite[1].setTexture(Continue.YESt[1]);
// 	Continue.YESsprite[0].setPosition(130, 600);
// 	Continue.YESsprite[1].setPosition(130, 600);
// 	Continue.NOt[0].loadFromFile("Assets\\nonactive no.png");
// 	Continue.NOt[1].loadFromFile("Assets\\active no.png");
// 	Continue.NOsprite[0].setTexture(Continue.NOt[0]);
// 	Continue.NOsprite[1].setTexture(Continue.NOt[1]);
// 	Continue.NOsprite[0].setPosition(160, 600);
// 	Continue.NOsprite[1].setPosition(160, 600);
//     Continue.ContinuesLeft.setFont(score.font);
// 	Continue.ContinuesLeft.setPosition(100, 650);
// 	Continue.ContinuesLeft.setString("continues left: " + to_string(score.trials));
//     return 0;
// }

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

    if (score.score >= 1200 * score.livelnum)
        player.size = 55;
    else if (score.score >= 300 * score.livelnum) 
        player.size = 35;

    score.scoretext.setString(to_string(score.score));

    return score.score;
}

void playermovement(float dt, fishes& fish, Sprite& sprite, Texture& t, RenderWindow& window) {
    Vector2f target = window.mapPixelToCoords(Mouse::getPosition(window));
    Vector2f direction = target - fish.anim.sprite.getPosition();

    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 5) {
        direction /= length;
        if (direction.x > 0) {
            fish.anim.currentframe = 1;
        }
        else if (direction.x < 0) {
            fish.anim.currentframe = 0;
        }

        fish.anim.sprite.setTextureRect(IntRect(fish.anim.currentframe * fish.anim.frameW, 0, fish.anim.frameW, fish.anim.frameH));
        fish.anim.sprite.move(direction.x * score.speedP * dt, direction.y * score.speedP * dt);
    }
}

void Sorry(setplayer&player, RenderWindow& window, float dt) {
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

int draw(setplayer&player, RenderWindow& window, float dt) {

    if (player.draw) window.draw(player.anim.sprite);
    if (smallFish.draw) window.draw(smallFish.anim.sprite);
    if (medFish.draw) window.draw(medFish.anim.sprite);
    if (bigFish.draw) window.draw(bigFish.anim.sprite);
    window.draw(score.scoretext);

    for (int q = 0; q < 7; q++) {
        window.draw(score.FRENZYiText[q]);
    }

    Sorry(player, window, dt);

    return 0;
}

void dead() {
    player.draw = 0;

    player.anim.sprite.setPosition(200, -100);

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
            player.anim.sprite.setPosition(200, -100);
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

// int mouthintersect(fishes& a, fishes& b) {
//     if (&a == &b) return 0;

//     fishes& bigger = (a.size > b.size) ? a : b;
//     fishes& smaller = (a.size > b.size) ? b : a;

//     if (smaller.aten) return 0;

//     Vector2f mouth = Mouthposition(bigger);
//     Vector2f targetPos = smaller.sprite.getPosition();

//     float dx = mouth.x - targetPos.x;
//     float dy = mouth.y - targetPos.y;
//     float dist = sqrt(dx * dx + dy * dy);

//     float radius = smaller.frameW / 2;

//     if (dist <= radius) {
//         // bigger fish eats smaller one

//         bigger.ate = 1;
//         smaller.aten = 1;
//         smaller.dead = 1;
//         smaller.draw = 0;

//         if (player.ate) {
//             score.score += smaller.score * score.bouns;
//         }

//         return 1;
//     }
//     else if (dist <= radius * 2) {
//         // if the smaller fish isnt the player, then it should run away
//         return 2;
//     }
//     else {
//         // two fishs are far from each other so nothing happens
//         return 0;
//     }

// }


void playermovement(float dt, setplayer& isplayer, RenderWindow& window) {

    Vector2f target = window.mapPixelToCoords(Mouse::getPosition(window));
    Vector2f direction = target - isplayer.anim.sprite.getPosition();

    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 5) {
        direction /= length;
        if (!isplayer.iseating) {
            if (direction.x > 0) {
                isplayer.anim.currentframe = 2;
                isplayer.dir = 1;
            }
            else if (direction.x < 0) {
                isplayer.anim.currentframe = 0;
                isplayer.dir = -1;
            }
        }

            // isplayer.sprite.setTextureRect(IntRect(isplayer.currentframe * isplayer.frameW, 0, isplayer.frameW, isplayer.frameH));

            isplayer.anim.sprite.move(direction * score.speedP * dt);
    }
}

void cameraMovement(View& camera, Vector2f playerPosition, float BGWidth, float BGHeight)
{
	float x = playerPosition.x;
	float y = playerPosition.y;

	if (x < 720) {
		x = 720;
	}
	if (y < 450) {
		y = 450;
	}
	if (x > BGWidth - 720) {
		x = BGWidth - 720;
	}
	if (y > BGHeight - 450) {
		y = BGHeight - 450;
	}
	camera.setCenter(x, y);
}
// different structs
void commonAssets::load()
{
    if (!game_font.loadFromFile("Fonts\\hint.ttf")) cout << "Menu font is not found" << endl;

    if (!main_menu_background_text.loadFromFile("Sprites\\2\\Menu assets\\mainMenu.jpg")) cout << "main menu background is not found" << endl;
    main_menu_background_sprite.setTexture(main_menu_background_text);
    main_menu_background_sprite.setScale(settings.res.x/main_menu_background_text.getSize().x, settings.res.y/main_menu_background_text.getSize().y);

    if (!menu_rocks_text.loadFromFile("Sprites\\2\\Menu assets\\rocks_g.png")) cout << "Rocks are not found" << endl;
    menu_rocks_sprite.setTexture(menu_rocks_text);
    menu_rocks_sprite.setScale((settings.res.x/menu_rocks_text.getSize().x), settings.res.y/menu_rocks_text.getSize().y);
    menu_rocks_sprite.setPosition(settings.res.x * 0.0645, settings.res.y * 0.314f);
}

void gameSounds::Click_sound()
{
    if(settings.controls[0])
        click_sound.setVolume(70);
    else
        click_sound.setVolume(0);
    click_sound.play();
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

void gameSounds::play_sound()
{
if (main_theme_sound.getStatus() != Sound::Playing && settings.controls[1])
    main_theme_sound.play();

if(!settings.controls[1])
    main_theme_sound.stop();
}
// Mermaid functions

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
    mermaid.setPosition(settings.res.x, 200);
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

// Shark functions
void Shark::start()
{
    if(!signalTexture.loadFromFile("Sprites\\unkown 2\\dangerSign.png")) cout << "danger sign is not found"<< endl;
    signal.setTexture(signalTexture);
    signal.setScale(1.4,1.4);

    if(!sharkTexture.loadFromFile("Sprites\\fish sprites\\Barracuda.png")) cout << "shark texture is not found"<< endl;
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

// Main menu functions
void mainMenu::load_assets()
{
    sound->play_sound();
    Vector2f buttons_scale; // hehe

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
                main_menu_buttons_sprite[i][j].setPosition(settings.res.x * 0.7f, settings.res.y * 0.12f);
                if(j){ main_menu_buttons_sprite[i][j].setScale(0.3f, 0.3f); break;}
                main_menu_buttons_sprite[i][j].setScale(0.285f, 0.285f);
                break;
            case 1:
                main_menu_buttons_sprite[i][j].setPosition(settings.res.x * 0.7f, settings.res.y * 0.25f);
                main_menu_buttons_sprite[i][j].setRotation(-1.3f);
                if(j) {main_menu_buttons_sprite[i][j].setScale(0.28f, 0.28f); break;}
                main_menu_buttons_sprite[i][j].setScale(0.265f,0.265f);
                break;
            case 2:
                main_menu_buttons_sprite[i][j].setPosition(settings.res.x * 0.7f, settings.res.y * 0.36f);
                break;
            case 3:
                main_menu_buttons_sprite[i][j].setPosition(settings.res.x * 0.7f, settings.res.y * 0.46f);
                if(j) {main_menu_buttons_sprite[i][j].setScale(0.26f, 0.26f); break;}
                main_menu_buttons_sprite[i][j].setScale(0.48f,0.48f);
                break;
            case 4:
                main_menu_buttons_sprite[i][j].setPosition(settings.res.x * 0.7f, settings.res.y * 0.565f);
                main_menu_buttons_sprite[i][j].setRotation(-1.05f);
                break;
            case 5:
                main_menu_buttons_sprite[i][j].setPosition(settings.res.x * 0.7, settings.res.y * 0.66f);
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
            buttons_scale= {(settings.res.x * 0.25f) / single_player_texture[i][j].getSize().x,(settings.res.y * 0.115f) / single_player_texture[i][j].getSize().y};
            single_player_sprite[i][j].setScale(buttons_scale.x, buttons_scale.y);
            if(i == 0)
            {
                single_player_sprite[i][j].setPosition(settings.res.x * 0.56501f, settings.res.y * 0.262f);
                single_player_sprite[i][j].rotate(1.8);
            }
            else if (i == 1)
            {
                single_player_sprite[i][j].setPosition(settings.res.x * 0.56501f, settings.res.y * 0.386f);
                single_player_sprite[i][j].rotate(-1.3);
            }
            else if (i == 2)
            {
                single_player_sprite[i][j].setPosition(settings.res.x * 0.56501f, settings.res.y * 0.486f);
                single_player_sprite[i][j].rotate(1.3);
            }
            else
            {
                single_player_sprite[i][j].setPosition(settings.res.x * 0.56501f, settings.res.y * 0.596f);
                single_player_sprite[i][j].rotate(1.3);
            }
        }
    }

    // al original menu "for refrence"
    ment.loadFromFile("men.jpg");
    mens.setScale(settings.res.x/ment.getSize().x, settings.res.y/ment.getSize().y);
    mens.setTexture(ment);
    
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
    tube_sprite.setOrigin(tube_texture.getSize().x / 2,tube_texture.getSize().y);
    tube_sprite.setScale((settings.res.x * 0.135f) / tube_texture.getSize().x, (settings.res.y * 0.315f) / tube_texture.getSize().y);
    tube_sprite.setPosition(settings.res.x * 0.715f, settings.res.y * 1.f);
    
    if (!txt.loadFromFile("Fonts\\press_any_button.png")) cout << "button is not found" << endl;
    txts.setTexture(txt);
    txts.setScale(settings.res.x/txt.getSize().x, settings.res.y/txt.getSize().y);
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
    
    // click to start refrence "hab2a ashelha ba3deen"
    if (!reft.loadFromFile("ref1.png")) cout << "ref's texture is not found" << endl;
    refs.setTexture(reft);
    refs.setScale(settings.res.x/reft.getSize().x, settings.res.y/reft.getSize().y);
    
    if (!the_sign_texture.loadFromFile("Sprites\\menu\\sprite_clean.png")) cout << "quad back is not found" << endl;
    the_sign_sprite.setTexture(the_sign_texture);
    the_sign_sprite.setPosition(settings.res.x * 0.355, settings.res.y * 0.785f);
    the_sign_sprite.setScale((settings.res.x * 0.3f) / the_sign_texture.getSize().x, (settings.res.y * 0.1f) / the_sign_texture.getSize().y);


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
        quads_sprite[i].setPosition(settings.res.x * 0.69201f, settings.res.y * 0.165f);
        buttons_scale= {(settings.res.x * 0.171f) / quads_texture[i].getSize().x,(settings.res.y * 0.28f) / quads_texture[i].getSize().y};
        quads_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
    }
};

void mainMenu::handle_movements(Event& event, short &scene )
{
    if (event.type == Event::KeyPressed)
    {
        sound->Click_sound();
        if(menu_scene == 0 && event.key.code != Keyboard::Escape)
            menu_scene++;

        else if(menu_scene == 1)
        {
            if (event.key.code == Keyboard::Down)
                selected ++, selected %= 6;
            if (event.key.code == Keyboard::Up)
                selected = (selected - 1) + 6 , selected%=6;
            if (event.key.code == Keyboard::Escape)
            {
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
                    {
                        scene = 1;
                        return;
                    }
                    else if(selected == 5)
                    {
                        window->close();
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
        else if(menu_scene == 3 || menu_scene == 5 || menu_scene == 4)
        {
            if (event.key.code == Keyboard::Escape)
                menu_scene = 1;
        }
    }

if (event.type == Event::MouseMoved)
{
Vector2i mousePixel = Mouse::getPosition(*window);
Vector2f mouse_position = window->mapPixelToCoords(mousePixel);
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
}
}

    if (event.type == Event::MouseButtonPressed)
    {

        sound->Click_sound();
        Vector2i mousePixel = Mouse::getPosition(*window);
        Vector2f mouse_position = window->mapPixelToCoords(mousePixel);

        if(event.mouseButton.button == Mouse::Left)
        {
            if (menu_scene == 0)
                menu_scene++;


            if(menu_scene == 1)
            {
                for(int i = 0; i <6; i++)
                {
                    if(selected == 5)
                        window->close();
                    if(selected == 4)
                    {
                        scene = 1; return;
                    }
                    if(selected == i)
                    {
                        menu_scene = i+2, selected = -1; break;
                    }
                }
            }

            else if(menu_scene == 2)
            {
                for(int i = 0; i < 4; i++)
                {
                    if(i == 3)
                        menu_scene = 1, selected = -1;
                    else if(selected == 0)
                        scene = 2;
                    else if(selected == i)
                        menu_scene = i + 8, selected = -1; 
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
    // new func


    switch(menu_scene)
    {
        case 0 : logo_transformation('b'); draw_starting_menu();break;
        case 1 : logo_transformation('f'); draw_main_menu(); break;
        case 2 : draw_single_player_menu(); break;
        case 3 : draw_multiplayer_menu(); break;
        case 4 : draw_leaderboards_menu(); break;
        case 5 : draw_achievements_menu(); break;
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

    for(int i =5; i>=0; i--)
    {
        window->draw(main_menu_buttons_sprite[i][selected == i ? 1 : 0]);
    }
    //window.draw(menu_text[0]);
    window->draw(assets->menu_rocks_sprite);
    window->draw(tool_tip_sprite);
    window->draw(the_sign_sprite);
    window->display();
};

void mainMenu::draw_single_player_menu()
{
    draw_basics();

    for(int i = 0; i < 4; i++)
    {
        window->draw(single_player_sprite[i][selected == i ? 1 : 0]);
        if(i > 2)
            continue;
        window->draw(quads_sprite[i]);
    }

    window->draw(assets->menu_rocks_sprite);
    window->draw(tool_tip_sprite);
    window->display();
}

void mainMenu::draw_multiplayer_menu()
{
    draw_basics();
    window->draw(assets->menu_rocks_sprite);
    window->display();
}

void mainMenu::draw_leaderboards_menu()
{
    draw_basics();
    window->draw(assets->menu_rocks_sprite);
    window->display();
}

void mainMenu::draw_achievements_menu()
{
    draw_basics();
    window->draw(assets->menu_rocks_sprite);
    window->display();
}

void mainMenu::draw_help_menu()
{
    draw_basics();
    window->draw(assets->menu_rocks_sprite);
    window->display();
}

//  help And Options
void helpAndOptions::load_assets()
{
    reft.loadFromFile("Sprites\\menu\\2.png");
    refs.setTexture(reft);
    for(int i =0; i< 7; i++)
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
            case 0: buttons_sprite[i][j].setPosition(settings.res.x /2.f, settings.res.y * 0.265f); break;
            case 1: buttons_sprite[i][j].setPosition(settings.res.x /2.f, settings.res.y * 0.38f); break;
            case 2: buttons_sprite[i][j].setPosition(settings.res.x /2.f, settings.res.y * 0.495f); break;
            case 3: buttons_sprite[i][j].setPosition(settings.res.x /2.f, settings.res.y * 0.61f); break;
            case 4: buttons_sprite[i][j].setPosition(settings.res.x /2.f, settings.res.y * 0.79f); break;
            case 5: buttons_sprite[i][j].setPosition(settings.res.x /2.f, settings.res.y * 0.79f);
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
        menu_shell_sprite[i].setPosition(settings.res.x /2, settings.res.y * 0.51f);
        menu_shell_sprite[i].setScale(1.76,1.75);
    }

    mouse_sprite.setTexture(mouse_texture);
    mouse_sprite.setOrigin(mouse_sprite.getGlobalBounds().width/2, mouse_sprite.getGlobalBounds().height/2);
    mouse_sprite.setPosition(settings.res.x * 0.43f, settings.res.y * 0.5f);
    mouse_sprite.setScale(0.9f,0.9f);

    for(int i =0; i < 4; i++)
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
    
    for(int i =0; i < 2; i++)
    {
        how_to_play_text[i].setString(how_to_play_string[i]);
        how_to_play_text[i].setFont(assets->game_font);
        how_to_play_text[i].setFillColor(Color::White);
        how_to_play_text[i].setOutlineColor(Color::Black);
        how_to_play_text[i].setCharacterSize(34);
        how_to_play_text[i].setOutlineThickness(2);
        how_to_play_text[i].setOrigin(how_to_play_text[i].getGlobalBounds().width/2,how_to_play_text[i].getGlobalBounds().height/2);
    }

    how_to_play_text[0].setPosition((0.44f) * settings.res.x,(0.335f) * settings.res.y);
    how_to_play_text[1].setPosition((0.435f) * settings.res.x,(0.585) * settings.res.y);

    for(int i =0; i<7; i++)
        for(int j =0; j < 2;j++)
            for(int h =0; h < 2; h++)
            {
                if(i <= 3)
                    buttons[i].checkbox_sprite[j][h].setPosition(0.495f * settings.res.x, 0.295f * settings.res.y + i * 100);
                else
                    buttons[i].checkbox_sprite[j][h].setPosition(0.495f * settings.res.x + (i - 3) * 60, 0.295f * settings.res.y + 3 * 100);
    }


    
    sound_volume.setPosition(0.495f * settings.res.x, 0.272f * settings.res.y);
    sound_volume.setFont(assets->game_font);
    sound_volume.setFillColor(Color::White);
    sound_volume.setOutlineColor(Color::Black);
    sound_volume.setCharacterSize(48);
    sound_volume.setOutlineThickness(2);

}

void helpAndOptions::handle_movements(Event& event, short &scene )
{

    if (event.type == Event::KeyPressed)
    {
        sound->Click_sound();
        if(options_scene == 0)
        {
            if (event.key.code == Keyboard::Escape)
                scene = 0;
            if (event.key.code == Keyboard::Down)
                selected++, selected %= 5;
            if (event.key.code == Keyboard::Up)
                selected = (selected - 1) + 5, selected %= 5;
            if(event.key.code == Keyboard::Enter)
            {
                if(selected == 0)
                    options_scene = 1, selected = -1;
                else if(selected == 1)
                    options_scene = 3;
                else if(selected == 2)
                    options_scene = 2, selected = -1;
                else if(selected == 3)
                    options_scene = 4, selected = -1;
                else if(selected == 4)
                    scene = 0, selected = -1;
            }
        }

        else
        {
            if (event.key.code == Keyboard::Escape || (event.key.code == Keyboard::Enter && selected == 0) )
                options_scene = 0, selected = -1;
            if (event.key.code == Keyboard::Up || event.key.code == Keyboard::Down)
                selected = 0;
        }
    }

    if (event.type == Event::MouseMoved)
    {

        Vector2i mousePixel = Mouse::getPosition(*window);
        Vector2f mouse_position = window->mapPixelToCoords(mousePixel);

        if(options_scene == 0)
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

        else if (options_scene == 2)
        {
            for(int i = 0; i < 7; i++)
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
            else if(options_scene == 1 || options_scene == 3 || options_scene == 4)
            {
                selected = buttons_sprite[4][0].getGlobalBounds().contains(mouse_position) ? 0 : -1;
            }
    }

    if (event.type == Event::MouseButtonPressed)
    {
        sound->Click_sound();
        Vector2i mousePixel = Mouse::getPosition(*window);
        Vector2f mouse_position = window->mapPixelToCoords(mousePixel);
        if(event.mouseButton.button == Mouse::Left)
        {
            if(options_scene == 0)
            {
                if(selected == 0)
                    options_scene = 1, selected = -1;
                else if(selected == 1)
                    options_scene = 3;
                else if(selected == 2)
                    options_scene = 2, selected = -1;
                else if(selected == 3)
                    options_scene = 4, selected = -1;
                else if(selected == 4)
                    scene = 0, selected = -1;
            }
            else if(options_scene == 2)
            {
                if(selected == 0)
                    options_scene = 0, selected = -1;
                else if(selected == 1)
                    settings.controls[0] = !settings.controls[0];
                else if(selected == 2)
                    settings.controls[1] = !settings.controls[1];
                else if(selected == 3)
                    settings.controls[2] = !settings.controls[2], window_mode(*window, settings.controls[2]);
                // else if(selected == 4)
                    
                // else if(selected == 5)
                
                // else
            }
            else if(options_scene == 1 || options_scene == 3 || options_scene == 4)
            {
                if(selected == 0) options_scene = 0, selected = -1;
            }
        }
    }
}

void helpAndOptions::update_menu_scenes()
{
    switch(options_scene)
    {
        case 0 : draw(); break;
        case 1 : draw_controls(); break;
        case 2 : draw_settings(); break;
        case 3 : draw_how_to_play(); break;
        case 4 : draw_credits(); break;
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
    for(int i = 0; i < 2; i++)
        window->draw(menu_shell_sprite[i]);
    for(int i = 0; i < 5; i++)
        window->draw(buttons_sprite[i][selected == i ? 1 : 0]);
    window->display();
}

void helpAndOptions::update_settings()
{
    sound_volume.setString(to_string(settings.sound_volume));
    music_volume.setString(to_string(settings.music_volume));
    FloatRect bounds = sound_volume.getLocalBounds();
    sound_volume.setOrigin(bounds.left + bounds.width, bounds.top);
    sound->play_sound();
}

void helpAndOptions::draw_settings()
{
    update_settings();
    draw_basics();
    //window->draw(refs);
    for(int i = 0; i < 3; i=i+2)
        window->draw(menu_shell_sprite[i]);
    
    for(int i =0; i < 4; i++)
    {
        window->draw(settings_text[i]);
    }
    for(int i = 0; i < 6; i++)
        window->draw(buttons[i].checkbox_sprite[settings.controls[i] ? 1 : 0][selected == (i + 1) ? 1 : 0]);
    //window->draw(sound_volume);
    window->draw(buttons_sprite[5][selected == 0 ? 1 : 0]);
    window->display();
}

void helpAndOptions::draw_controls()
{
    draw_basics();

    for(int i = 0; i < 4; i=i+3)
    {
        window->draw(menu_shell_sprite[i]);
        if(i == 3)
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
    for(int i = 0; i < 5; i=i+4)
        window->draw(menu_shell_sprite[i]);
    for(int i =0; i < 2; i++)
        window->draw(how_to_play_text[i]);
    window->draw(buttons_sprite[4][selected == 0 ? 1 : 0]);
    window->display();
}

void helpAndOptions::draw_credits()
{
    draw_basics();
    //window->draw(refs);
    for(int i = 0; i < 3; i=i+2)
        window->draw(menu_shell_sprite[i]);
    window->draw(buttons_sprite[4][selected == 0 ? 1 : 0]);
    window->display();
}

void window_mode(RenderWindow &window, bool fullscreen) {
    if (fullscreen)
        window.create(VideoMode(settings.res.x, settings.res.y), "Feeding Frenzy 2 - Shipwrech Showdown", Style::Fullscreen);
    else
        window.create(VideoMode(settings.res.x, settings.res.y), "Feeding Frenzy 2 - Shipwrech Showdown");
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

void menuTextBeforeLevels::textMenucontrols(RenderWindow& window, short &scene, Event& event) {

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



		if (inTextMenuLevel1) {

			inTextMenuLevel1 = false;
            scene = 4;
			// inTextMenuLevel2 = true;
			// inTextMenuLevel3 = false;
			// inTextMenuLevel4 = false;

		}

		else if (inTextMenuLevel2) {

			inTextMenuLevel1 = false;
			inTextMenuLevel2 = false;
			inTextMenuLevel3 = true;
			inTextMenuLevel4 = false;

		}

		else if (inTextMenuLevel3) {

			inTextMenuLevel1 = false;
			inTextMenuLevel2 = false;
			inTextMenuLevel3 = false;
			inTextMenuLevel4 = true;

		}

		else if (inTextMenuLevel4) {

			inTextMenuLevel1 = false;
			inTextMenuLevel2 = false;
			inTextMenuLevel3 = false;
			inTextMenuLevel4 = false;

		}

	}

	if (isOnQuit && leftMouseClicked) {

		scene = 2;

	}

	leftMouseClicked = false;

}