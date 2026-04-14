#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iomanip>
using namespace std;
using namespace sf;

void intersection(RectangleShape playerHitbox, RectangleShape fishHitbox, Vector2f playerPosition, Vector2f fishPosition, int playerSize, int fishSize, int score, int lives, bool drawPlayer, bool canMove);

struct help_and_options{
    Texture buttons_texture[5][2], menu_shell_texture[6],mouse_texture, reft;
    Sprite buttons_sprite[5][2],menu_shell_sprite[6],mouse_sprite, refs;
    void load_assets(Vector2f res)
    {
        reft.loadFromFile("Sprites\\menu\\2.png");
        refs.setTexture(reft);
        
        
        if(!mouse_texture.loadFromFile("Sprites\\menu\\mouse.png")) cout << "mouse texture is not found" << endl;
        
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

        for(int i = 0; i < 5; i++)
        {
            for(int j =0; j < 2; j++)
            {
            buttons_sprite[i][j].setTexture(buttons_texture[i][j]);
            Vector2f buttons_scale= {(res.x * 0.23f) / buttons_texture[i][j].getSize().x,(res.y * 0.105f) / buttons_texture[i][j].getSize().y};
            buttons_sprite[i][j].setScale(buttons_scale.x, buttons_scale.y);
            if(i == 0)
            {
                buttons_sprite[i][j].setPosition(res.x * 0.390f, res.y * 0.2179f);
                buttons_scale= {(res.x * 0.22f) / buttons_texture[i][j].getSize().x,(res.y * 0.1f) / buttons_texture[i][j].getSize().y};
            }
            else if (i == 1)
            {
                buttons_sprite[i][j].setPosition(res.x * 0.390f, res.y * 0.316f);
                buttons_sprite[i][j].rotate(1.3);
            }
            else if (i == 2)
                buttons_sprite[i][j].setPosition(res.x * 0.3880f, res.y * 0.42f);
            else if(i==3)
                buttons_sprite[i][j].setPosition(res.x * 0.3880f, res.y * 0.52f);
            else
                buttons_sprite[i][j].setPosition(res.x * 0.3870f, res.y * 0.765f);
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
    }

    void handle_movements(Event& event, short &selected, short &menu_scene, RenderWindow& window)
    {

        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Escape)
                menu_scene = 1;
            if (event.key.code == Keyboard::Down)
                selected++, selected %= 5;
            if (event.key.code == Keyboard::Up)
                selected = (selected - 1) + 5, selected %= 5;
        }

        if (event.type == Event::MouseMoved)
        {

            Vector2i mousePixel = Mouse::getPosition(window);
            Vector2f mouse_position = window.mapPixelToCoords(mousePixel);

            if(menu_scene == 6)
            {
                if(buttons_sprite[0][0].getGlobalBounds().contains(mouse_position))
                    selected = 0;
                else if(buttons_sprite[1][0].getGlobalBounds().contains(mouse_position))
                    selected = 1;
                else if(buttons_sprite[2][0].getGlobalBounds().contains(mouse_position))
                    selected = 2;
                else if(buttons_sprite[3][0].getGlobalBounds().contains(mouse_position))
                    selected = 3;
                else if(buttons_sprite[4][0].getGlobalBounds().contains(mouse_position))
                    selected = 4;
                else
                    selected = -1;
            }
            else if (menu_scene == 11)
            {
                if(buttons_sprite[4][0].getGlobalBounds().contains(mouse_position))
                    selected = 0;
                else
                    selected = -1;
            }
            else if (menu_scene == 12)
            {
                if(buttons_sprite[4][0].getGlobalBounds().contains(mouse_position))
                    selected = 0;
                else
                    selected = -1;
            }
            else if (menu_scene == 13)
            {
                if(buttons_sprite[4][0].getGlobalBounds().contains(mouse_position))
                    selected = 0;
                else
                    selected = -1;
            }
            else if (menu_scene == 14)
            {
                if(buttons_sprite[4][0].getGlobalBounds().contains(mouse_position))
                    selected = 0;
                else
                    selected = -1;
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
                else if(menu_scene == 11 && selected == 0)
                    menu_scene = 6, selected = -1;
                else if(menu_scene == 12 && selected == 0)
                    menu_scene = 6, selected = -1;
                else if(menu_scene == 13 && selected == 0)
                    menu_scene = 6, selected = -1;
                else if(menu_scene == 14 && selected == 0)
                    menu_scene = 6, selected = -1;
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

    void draw_settings(RenderWindow& window, short selected, Sprite main_menu_background_sprite,Sprite menu_rocks_sprite)
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

    void draw_controls(RenderWindow& window, short selected, Sprite main_menu_background_sprite,Sprite menu_rocks_sprite)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
        window.draw(menu_rocks_sprite);
        //window.draw(refs);
        for(int i = 0; i < 4; i=i+3)
        {
            window.draw(menu_shell_sprite[i]);
            if(i == 3)
                continue;
            window.draw(mouse_sprite);
        }
        window.draw(buttons_sprite[4][selected == 0 ? 1 : 0]);
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

struct mainMenu{
    help_and_options* help = nullptr;
    short selected =-1, menu_scene =0;
    // scenes 0 -> click to start, 1 -> main menu, 2-> single player, 3-> multiplayer, 4-> leaderboards
    // 5 -> achievements, 6->help, 7-> quit 
    // 8 -> story mode, 9 - > story lite, 10 -> time attack, 
    // settings = 11, controls = 12, how to play = 13, credits = 14
    Texture main_menu_background_text, logo_texture, button1_texture[4], button2_texture[2], reft, txt, menu_rocks_text,
    button3_texture[2], button4_texture[2], button5_texture[2], button6_texture[2],tool_tip_texture,tube_texture, ment, rr,
    single_player_texture[4][2], quads_texture[6], the_sign_texture;
    Sprite main_menu_background_sprite, logo_sprite, button1_sprite[4], button2_sprite[2],button3_sprite[2], menu_rocks_sprite,
    button4_sprite[2], button5_sprite[2], button6_sprite[2],tool_tip_sprite, tube_sprite, mens, refs, txts, rrs, single_player_sprite[4][2],
    quads_sprite[6], the_sign_sprite;
    Vector2f Res;
    SoundBuffer click_buffer, main_theme_buffer;
    Sound click_sound, main_theme_sound;
    string menu_strings[6] = {"Single Player Game Modes","Multiplayer Game Modes","View Leaderboards", "See Your Achievements", "Adjust Sound and Music Settings Or Learn How tp Play", "Return to Desktop"};
    Text menu_text[6];
    Font menu_font;
    void load_assets(Vector2f res)
    {
        Res = res;
        Vector2f buttons_scale;
        if (!main_menu_background_text.loadFromFile("Sprites\\2\\Menu assets\\mainMenu.jpg")) cout << "main menu background is not found" << endl;
        main_menu_background_sprite.setTexture(main_menu_background_text);
        main_menu_background_sprite.setScale(res.x/main_menu_background_text.getSize().x, res.y/main_menu_background_text.getSize().y);
        
        if (!rr.loadFromFile("Sprites\\menu\\ref.png")) cout << "rr's texture is not found" << endl;
        rrs.setTexture(rr);
        // Loading buttons!
        if (!button1_texture[0].loadFromFile("Sprites\\menu\\single-player.png")) cout << "button 1's texture is not found" << endl;
        if (!button1_texture[1].loadFromFile("Sprites\\menu\\single-player-glow.png")) cout << "glowing button 1's texture is not found" << endl;

        if (!button2_texture[0].loadFromFile("Sprites\\menu\\multiplayer.png")) cout << "button 2's texture is not found" << endl;
        if (!button2_texture[1].loadFromFile("Sprites\\menu\\multiplayer-glow.png")) cout << "glowing button 2 texture is not found" << endl;

        if (!button3_texture[0].loadFromFile("Sprites\\menu\\leaderboards.png")) cout << "button 3's texture is not found" << endl;
        if (!button3_texture[1].loadFromFile("Sprites\\menu\\leaderboards-glow.png")) cout << "glowing button 3 texture is not found" << endl;
        
        if (!button4_texture[0].loadFromFile("Sprites\\menu\\achievements.png")) cout << "button 4's texture is not found" << endl;
        if (!button4_texture[1].loadFromFile("Sprites\\menu\\achievements-glow.png")) cout << "glowing button 4 texture is not found" << endl;
        
        if (!button5_texture[0].loadFromFile("Sprites\\menu\\help-and-options.png")) cout << "button 5's texture is not found" << endl;
        if (!button5_texture[1].loadFromFile("Sprites\\menu\\help-and-options-glow.png")) cout << "glowing button 5 texture is not found" << endl;
        
        if (!button6_texture[0].loadFromFile("Sprites\\menu\\quit.png")) cout << "button 6's texture is not found" << endl;
        if (!button6_texture[1].loadFromFile("Sprites\\menu\\quit-glow.png")) cout << "glowing button 6 texture is not found" << endl;

        if (!single_player_texture[0][0].loadFromFile("Sprites\\menu\\story-mode.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[0][1].loadFromFile("Sprites\\menu\\story-mode-glow.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[1][0].loadFromFile("Sprites\\menu\\story-title.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[1][1].loadFromFile("Sprites\\menu\\story-title-glow.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[2][0].loadFromFile("Sprites\\menu\\time-attack.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[2][1].loadFromFile("Sprites\\menu\\time-attack-glow.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[3][0].loadFromFile("Sprites\\menu\\back.png")) cout << "glowing button 1's texture is not found" << endl;
        if (!single_player_texture[3][1].loadFromFile("Sprites\\menu\\back-glow.png")) cout << "glowing button 1's texture is not found" << endl;
        
        for(int i = 0; i < 2; i++)
        {
        button1_sprite[i].setTexture(button1_texture[i]);
        button1_sprite[i].setPosition(res.x * 0.55501f, res.y * 0.08f);
        buttons_scale= {(res.x * 0.27f) / button1_texture[i].getSize().x,(res.y * 0.14f) / button1_texture[i].getSize().y};
        button1_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
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
                    //if(j == 1)
                    //    buttons_scale= {(res.x * 0.22f) / single_player_texture[i][j].getSize().x,(res.y * 0.1f) / single_player_texture[i][j].getSize().y};
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
    //button1_sprite[1].setRotation(2.f);

        for(int i = 0; i < 2; i++)
        {
        button2_sprite[i].setTexture(button2_texture[i]);
        button2_sprite[i].setPosition(res.x * 0.56001f, res.y * 0.21f);
        button2_sprite[i].setRotation(-1.3f);
        buttons_scale= {(res.x * 0.25f) / button2_texture[i].getSize().x,(res.y * 0.13f) / button2_texture[i].getSize().y};
        button2_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
        }
        
        for(int i = 0; i < 2; i++)
        {
        button3_sprite[i].setTexture(button3_texture[i]);
        button3_sprite[i].setPosition(res.x * 0.56501f, res.y * 0.32f);
        //button3_sprite[i].setRotation(1.65f);
        buttons_scale= {(res.x * 0.255f) / button3_texture[i].getSize().x,(res.y * 0.10f) / button3_texture[i].getSize().y};
        button3_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
        }

        for(int i = 0; i < 2; i++)
        {
            button4_sprite[i].setTexture(button4_texture[i]);
            button4_sprite[i].setPosition(res.x * 0.56401f, res.y * 0.404f);
            //button4_sprite[i].setRotation(-1.05f);
            buttons_scale= {(res.x * 0.255f) / button4_texture[i].getSize().x,(res.y * 0.10f) / button4_texture[i].getSize().y};
            button4_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
        }

        for(int i = 0; i < 2; i++)
        {
            button5_sprite[i].setTexture(button5_texture[i]);
            button5_sprite[i].setPosition(res.x * 0.56401f, res.y * 0.499f);
            button5_sprite[i].setRotation(-1.05f);
            buttons_scale= {(res.x * 0.255f) / button5_texture[i].getSize().x,(res.y * 0.10f) / button5_texture[i].getSize().y};
            button5_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
        }

        for(int i = 0; i < 2; i++)
        {
        button6_sprite[i].setTexture(button6_texture[i]);
        button6_sprite[i].setPosition(res.x * 0.62401f, res.y * 0.584f);  // was 0.585f
        button6_sprite[i].setRotation(-1.45f);
        buttons_scale= {(res.x * 0.135f) / button6_texture[i].getSize().x,(res.y * 0.095f) / button6_texture[i].getSize().y};  // was 0.357f
        button6_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
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
        main_theme_sound.setVolume(70);
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
        click_sound.setVolume(70);
        
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

    void handle_movements(Event& event, short &scene, RenderWindow& window)
    {
        if (event.type == Event::KeyPressed)
        {
            click_sound.play();
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
                    menu_scene--;selected = -1;
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
                            window.close();
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
            else if((menu_scene == 6 || menu_scene == 11 || menu_scene == 12||menu_scene == 13 || menu_scene == 14) && help)
            {
                help->handle_movements(event, selected, menu_scene, window);
            }
        }

        if (event.type == Event::MouseMoved)
        {
            Vector2i mousePixel = Mouse::getPosition(window);
            Vector2f mouse_position = window.mapPixelToCoords(mousePixel);
            if(menu_scene == 1)
            {
                if(button1_sprite[0].getGlobalBounds().contains(mouse_position))
                    selected = 0;
                else if(button2_sprite[0].getGlobalBounds().contains(mouse_position))
                    selected = 1;
                else if(button3_sprite[0].getGlobalBounds().contains(mouse_position))
                    selected = 2;
                else if(button4_sprite[0].getGlobalBounds().contains(mouse_position))
                    selected = 3;
                else if(button5_sprite[0].getGlobalBounds().contains(mouse_position))
                    selected = 4;
                else if(button6_sprite[0].getGlobalBounds().contains(mouse_position))
                    selected = 5;
                else
                    selected = -1;
            }
            else if(menu_scene == 2)
            {
                if(single_player_sprite[0][0].getGlobalBounds().contains(mouse_position))
                    selected = 0;
                else if(single_player_sprite[1][0].getGlobalBounds().contains(mouse_position))
                    selected = 1;
                else if(single_player_sprite[2][0].getGlobalBounds().contains(mouse_position))
                    selected = 2;
                else if(single_player_sprite[3][0].getGlobalBounds().contains(mouse_position))
                    selected = 3;
                else
                    selected = -1;
            }
            else if((menu_scene == 6 || menu_scene == 11 || menu_scene == 12||menu_scene == 13 || menu_scene == 14) && help)
            {
                help->handle_movements(event, selected, menu_scene, window);
            }
        }

        if (event.type == Event::MouseButtonPressed)
        {

            click_sound.play();
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
                    if(selected == 0)
                        menu_scene = 2, selected = -1;
                    else if(selected == 1)
                        menu_scene = 3, selected = -1;
                    else if(selected == 2)
                        menu_scene = 4, selected = -1;
                    else if(selected == 3)
                        menu_scene = 5, selected = -1;
                    else if(selected == 4)
                        menu_scene = 6, selected = -1;
                    else if(selected == 5)
                        window.close();
                }
                else if(menu_scene == 2)
                {
                    if(selected == 0)
                        menu_scene = 8, selected = -1;
                    else if(selected == 1)
                        menu_scene = 9, selected = -1;
                    else if(selected == 2)
                        menu_scene = 10, selected = -1;
                    else if(selected == 3)
                        menu_scene = 1, selected = -1;
                }
                else if((menu_scene == 6 || menu_scene == 11 || menu_scene == 12 ||menu_scene == 13 || menu_scene == 14) && help)
                    help->handle_movements(event, selected, menu_scene, window);
            }
        }
    };

    void update_menu_scenes(RenderWindow& window)
    {
        if (main_theme_sound.getStatus() != Sound::Playing)
            main_theme_sound.play();
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
                    help->load_assets(Res);
                }
                help->draw(window, selected, main_menu_background_sprite,menu_rocks_sprite);
                break;
            case 11 : help->draw_settings(window, selected, main_menu_background_sprite,menu_rocks_sprite); break;
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
        //        window.draw(mens); // de al main menu 2l original "bazabat 3aleha al 7aga"
        window.draw(logo_sprite);
        window.draw(menu_rocks_sprite);
        //window.draw(tube_sprite);
        window.draw(tool_tip_sprite);
        window.draw(button6_sprite[selected == 5 ? 1 : 0]);
        window.draw(button5_sprite[selected == 4 ? 1 : 0]);
        window.draw(button4_sprite[selected == 3 ? 1 : 0]);
        window.draw(button3_sprite[selected == 2 ? 1 : 0]);
        window.draw(button2_sprite[selected == 1 ? 1 : 0]);
        window.draw(button1_sprite[selected == 0 ? 1 : 0]);
        window.draw(the_sign_sprite);
        //window.draw(menu_text[0]);
        window.display();
    };
    
    void draw_single_player_menu(RenderWindow& window)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
        window.draw(logo_sprite);
        window.draw(menu_rocks_sprite);
        //window.draw(rrs);
        for(int i = 0; i < 4; i++)
        {
            window.draw(single_player_sprite[i][selected == i ? 1 : 0]);
            if(i > 2)
                continue;
            window.draw(quads_sprite[i]);
        }
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
        window.display();
    }

    void draw_help_menu(RenderWindow& window)
    {
        window.clear();
        window.display();
    }
};

void start();
void update(float dt);
void draw_mermaid(RenderWindow& window);
void mermaidMovement(Sprite& mermaid, float speed);
void mermaidAnimation(Sprite& mermaid, Texture& mermaidTexture, int& fraame);

Texture mermaidTexture;
Sprite mermaid;

int frame = 0;
float timer = 0;

const int MAX_STARS = 30;

Texture starTexture;
Sprite stars[MAX_STARS];
bool active[MAX_STARS];

float starTimer = 0;

int main()
{
    mainMenu* menu = nullptr;
    Vector2f res = {1920,1080};
    RenderWindow window(VideoMode(res.x, res.y), "Feeding frenzy 2");
    Event event;
    Image icon;
    icon.loadFromFile("Sprites\\icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    short scene = 1; // 0 for start menu, 1 -> mermaid animation
    Clock clock;

    start();
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if(menu)
                menu->handle_movements(event, scene, window);
        }
        switch (scene)
        {
        case 0:
            if (!menu)
            {
                menu = new mainMenu();
                menu->load_assets(res);
            }
            menu->update_menu_scenes(window);
            break;
        case 1:
            update(dt);
            window.clear();
            draw_mermaid(window);
            window.display();
        break;
        }
    }

    return 0;
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

//MOVEMENT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void mermaidMovement(Sprite& mermaid, float speed)
{
    Vector2f pos = mermaid.getPosition();
    pos.x -= speed;
    mermaid.setPosition(pos);
}


//ANIMATION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
void mermaidAnimation(Sprite& mermaid, Texture& texture, int& frame)
{
    int frameWidth = 3011 / 10;
    int frameHeight = 202 / 2;

    int column = frame % 10;
    int row = frame / 10;
    mermaid.setTexture(texture);
    mermaid.setTextureRect(IntRect(column * frameWidth, row * frameHeight, frameWidth, frameHeight));

}



void start()
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



void update(float dt)
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

    mermaidMovement(mermaid, 0.17f);
    mermaidAnimation(mermaid, mermaidTexture, frame);
}




void draw_mermaid(RenderWindow& window)
{
    window.draw(mermaid);

    for (int i = 0; i < MAX_STARS; i++)
    {
        if (active[i])
            window.draw(stars[i]);
    }
}