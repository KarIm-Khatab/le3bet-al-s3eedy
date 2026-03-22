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

int main()
{
    RenderWindow window = { VideoMode(1440,900),"sfml works" };
    Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
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