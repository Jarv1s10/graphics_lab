#include <iostream>
#include <vector>
#include <set>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "FortuneAlgorithm.h"
using namespace std;

class MainWindow
{
public:
	int RADIUS = 5, width = 800, hight = 600;
	bool LOCK_FLAG = false;


	void onClick(double x, double y, sf::CircleShape& circle)
	{;
		circle.setFillColor(sf::Color::Green);
		circle.setPosition(x, y);
	}

	void onV(vector<sf::Vertex>& lines)
	{
		LOCK_FLAG = true;
	}
	
};


int main()
{
	MainWindow mw;
	sf::RenderWindow window(sf::VideoMode(mw.width, mw.hight), "Voronoi");
	vector<sf::CircleShape> dots;
	vector<sf::Vertex> lines;
	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && !mw.LOCK_FLAG)
			{
				sf::CircleShape circle(mw.RADIUS);
				mw.onClick(event.mouseButton.x - mw.RADIUS, event.mouseButton.y - mw.RADIUS, circle);
				dots.push_back(circle);
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::V)
				{
					if (!mw.LOCK_FLAG)
					{
						mw.onV(lines);
						cout << "voronoi calculated" << endl;
					}
				}
			}
		}

		window.clear();

		for (const auto& r : dots)
		{
			window.draw(r);
		}
		window.display();
	}
	
	return 0;
}