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

struct mainMenu{
    short selected =0, menu_scene =0 ;
    Texture main_menu_background_text, logo_texture, button1_texture[2], button2_texture[2], reft,
    button3_texture[2], button4_texture[2], button5_texture[2], button6_texture[2],tool_tip_texture,tube_texture, ment;
    Sprite main_menu_background_sprite, logo_sprite, button1_sprite[2], button2_sprite[2],button3_sprite[2],
    button4_sprite[2], button5_sprite[2], button6_sprite[2],tool_tip_sprite, tube_sprite, mens, refs;
    Vector2f Res;
    SoundBuffer click_buffer, main_theme_buffer;
    Sound click_sound, main_theme_sound;
    void load_assets(Vector2f res)
    {
        Res = res;
        Vector2f buttons_scale;
        if (!main_menu_background_text.loadFromFile("Sprites\\2\\Menu assets\\mainMenu.jpg")) cout << "main menu background is not found" << endl;
        main_menu_background_sprite.setTexture(main_menu_background_text);
        main_menu_background_sprite.setScale(res.x/main_menu_background_text.getSize().x, res.y/main_menu_background_text.getSize().y);
        
        if (!button1_texture[0].loadFromFile("Sprites\\2\\Menu assets\\newbutton1.png")) cout << "button 1's texture is not found" << endl;
        if (!button1_texture[1].loadFromFile("Sprites\\2\\Menu assets\\newbutton1glow.png")) cout << "glowing button 1's texture is not found" << endl;
        for(int i = 0; i < 2; i++)
        {
        button1_sprite[i].setTexture(button1_texture[i]);
        button1_sprite[i].setPosition(res.x * 0.55501f, res.y * 0.08f);
        button1_sprite[i].setRotation(2.f);
        buttons_scale= {(res.x * 0.27f) / button1_texture[i].getSize().x,(res.y * 0.14f) / button1_texture[i].getSize().y};
        button1_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
        }

        if (!button2_texture[0].loadFromFile("Sprites\\2\\Menu assets\\newbutton2.png")) cout << "button 2's texture is not found" << endl;
        if (!button2_texture[1].loadFromFile("Sprites\\2\\Menu assets\\newbutton2glow.png")) cout << "glowing button 2 texture is not found" << endl;
        for(int i = 0; i < 2; i++)
        {
        button2_sprite[i].setTexture(button2_texture[i]);
        button2_sprite[i].setPosition(res.x * 0.56001f, res.y * 0.21f);
        button2_sprite[i].setRotation(-1.3f);
        buttons_scale= {(res.x * 0.25f) / button2_texture[i].getSize().x,(res.y * 0.13f) / button2_texture[i].getSize().y};
        button2_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
        }
        
        if (!button3_texture[0].loadFromFile("Sprites\\2\\Menu assets\\newbutton3.png")) cout << "button 3's texture is not found" << endl;
        if (!button3_texture[1].loadFromFile("Sprites\\2\\Menu assets\\newbutton3glow.png")) cout << "glowing button 3 texture is not found" << endl;
        for(int i = 0; i < 2; i++)
        {
        button3_sprite[i].setTexture(button3_texture[i]);
        button3_sprite[i].setPosition(res.x * 0.56501f, res.y * 0.32f);
        button3_sprite[i].setRotation(1.65f);
        buttons_scale= {(res.x * 0.255f) / button3_texture[i].getSize().x,(res.y * 0.10f) / button3_texture[i].getSize().y};
        button3_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
        }
        if (!button4_texture[0].loadFromFile("Sprites\\2\\Menu assets\\newbutton4.png")) cout << "button 4's texture is not found" << endl;
        if (!button4_texture[1].loadFromFile("Sprites\\2\\Menu assets\\newbutton4glow.png")) cout << "glowing button 4 texture is not found" << endl;
        for(int i = 0; i < 2; i++)
        {
            button4_sprite[i].setTexture(button4_texture[i]);
            button4_sprite[i].setPosition(res.x * 0.56401f, res.y * 0.414f);
            button4_sprite[i].setRotation(-1.05f);
            buttons_scale= {(res.x * 0.255f) / button4_texture[i].getSize().x,(res.y * 0.10f) / button4_texture[i].getSize().y};
            button4_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
        }
        if (!button5_texture[0].loadFromFile("Sprites\\2\\Menu assets\\newbutton5.png")) cout << "button 5's texture is not found" << endl;
        if (!button5_texture[1].loadFromFile("Sprites\\2\\Menu assets\\newbutton5glow.png")) cout << "glowing button 5 texture is not found" << endl;
        for(int i = 0; i < 2; i++)
        {
            button5_sprite[i].setTexture(button5_texture[i]);
            button5_sprite[i].setPosition(res.x * 0.56401f, res.y * 0.495f);
            button5_sprite[i].setRotation(-1.05f);
            buttons_scale= {(res.x * 0.255f) / button5_texture[i].getSize().x,(res.y * 0.10f) / button5_texture[i].getSize().y};
            button5_sprite[i].setScale(buttons_scale.x, buttons_scale.y);
        }
        if (!button6_texture[0].loadFromFile("Sprites\\2\\Menu assets\\newbutton6.png")) cout << "button 6's texture is not found" << endl;
        if (!button6_texture[1].loadFromFile("Sprites\\2\\Menu assets\\newbutton6glow.png")) cout << "glowing button 6 texture is not found" << endl;
        for(int i = 0; i < 2; i++)
        {
            button6_sprite[i].setTexture(button6_texture[i]);
            button6_sprite[i].setPosition(res.x * 0.56401f, res.y * 0.585f);
            button6_sprite[i].setRotation(-1.45f);
            buttons_scale= {(res.x * 0.257f) / button6_texture[i].getSize().x,(res.y * 0.095f) / button6_texture[i].getSize().y};
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
        // click to start refrence "hab2a ashelha ba3deen"
        if (!reft.loadFromFile("ref1.png")) cout << "ref's texture is not found" << endl;
        refs.setTexture(reft);
        refs.setScale(res.x/reft.getSize().x, res.y/reft.getSize().y);

    };
    void handle_movements(Event& event, short &scene)
    {
        if (event.type == Event::KeyPressed)
    {
        if(menu_scene == 0)
        {
            menu_scene++;
            logo_sprite.setScale((Res.x * 0.365f) / logo_texture.getSize().x, (Res.y * 0.37f) / logo_texture.getSize().y);
            logo_sprite.setPosition(Res.x * 0.1665f, Res.y * 0.05f);
        }
        else if(menu_scene == 1)
        {
            if (event.key.code == Keyboard::Down)
                selected ++, selected%=6;//, cout << selected;
            if (event.key.code == Keyboard::Up)
                selected = (selected - 1) + 6 , selected%=6;//, cout << selected;
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
        }
    }
    void draw_starting_menu(RenderWindow& window)
    {
        window.clear();
//        window.draw(refs);
        window.draw(main_menu_background_sprite);
        window.draw(logo_sprite);
        window.draw(tube_sprite);
        window.display();
    }
    
    void draw_main_menu(RenderWindow& window)
    {
        window.clear();
        window.draw(main_menu_background_sprite);
//        window.draw(mens); // de al main menu 2l original "bazabat 3aleha al 7aga"
        window.draw(logo_sprite);
        window.draw(tube_sprite);
        window.draw(tool_tip_sprite);
        window.draw(button6_sprite[selected == 5 ? 1 : 0]);
        window.draw(button5_sprite[selected == 4 ? 1 : 0]);
        window.draw(button4_sprite[selected == 3 ? 1 : 0]);
        window.draw(button3_sprite[selected == 2 ? 1 : 0]);
        window.draw(button2_sprite[selected == 1 ? 1 : 0]);
        window.draw(button1_sprite[selected == 0 ? 1 : 0]);
        window.display();
    };
};

int main()
{
    mainMenu* menu = nullptr;
    Vector2f res = {1920,1080};
    RenderWindow window(VideoMode(res.x, res.y), "Feeding frenzy 2");
    Event event;
    Image icon;
    icon.loadFromFile("Sprites\\icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    short scene = 0; // 0 for start menu,

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if(menu)
                menu-> handle_movements(event, scene);
        }
        switch (scene)
        {
        case 0:
            if (!menu)
        {
            menu = new mainMenu();
            menu->load_assets(res);
        }
        menu-> update_menu_scenes(window);
        break;
        case 1:
        
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