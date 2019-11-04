#include <iostream>
#include <vector>
#include <set>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "FortuneAlgorithm.h"
#include "ConvexHull.h"
using namespace mygal;


const double RADIUS = 0.005;
const int WIDTH = 800, HEIGHT = 800;

class MainWindow
{
public:

	void onClick(double x, double y)
	{
		points.push_back(Vector2<double>{x / double(WIDTH), y / double(HEIGHT)});
	}

	Diagram<double> onV()
	{
		auto algorithm = FortuneAlgorithm<double>(points);
		algorithm.construct();
		algorithm.bound(Box<double>{-0.5, -0.5, 1.5, 1.5});
		auto diagram = algorithm.getDiagram();
		return diagram;
	}

	std::vector<Vector2<double>> onOne()
	{
		auto hull = QuickHull().gethull(points);
		return hull;
	}

	std::vector<Vector2<double>> onTwo()
	{
		auto hull = Graham().gethull(points);
		return hull;
	}

	std::vector<Vector2<double>> onThree()
	{
		auto hull = Andrew().gethull(points);
		return hull;
	}

	void drawDiagram(sf::RenderWindow& window, Diagram<double>& diagram)
	{
		for (const auto& site : diagram.getSites())
		{
			auto center = site.point;
			auto face = site.face;
			auto halfEdge = face->outerComponent;
			if (halfEdge == nullptr)
				continue;
			while (halfEdge->prev != nullptr)
			{
				halfEdge = halfEdge->prev;
				if (halfEdge == face->outerComponent)
					break;
			}
			auto start = halfEdge;
			while (halfEdge != nullptr)
			{
				if (halfEdge->origin != nullptr && halfEdge->destination != nullptr)
				{
					auto origin = halfEdge->origin->point;
					auto destination = halfEdge->destination->point;
					drawEdge(window, origin, destination, sf::Color::Blue);
				}
				halfEdge = halfEdge->next;
				if (halfEdge == start)
					break;
			}
		}
	}

	void drawTriangulation(sf::RenderWindow& window, const Diagram<double>& diagram, const Triangulation& triangulation)
	{
		for (auto i = std::size_t(0); i < diagram.getNbSites(); ++i)
		{
			auto origin = diagram.getSite(i)->point;
			for (const auto& j : triangulation.getNeighbors(i))
			{
				auto destination = diagram.getSite(j)->point;
				drawEdge(window, origin, destination, sf::Color::Red);
			}
		}
	}

	void drawConvexhull(sf::RenderWindow& window, std::vector<Vector2<double>> hull, sf::Color color)
	{
		for (int i = 0; i < hull.size() - 1; i++)
		{
			drawEdge(window, hull[i], hull[i + 1], color);
		}
		drawEdge(window, hull[0], hull[hull.size()-1], color);
	}

private:

	std::vector<Vector2<double>> points;

	void drawEdge(sf::RenderWindow& window, Vector2<double> origin, Vector2<double> destination, sf::Color color)
	{
		auto line = std::array<sf::Vertex, 2>
		{
			sf::Vertex(sf::Vector2f(origin.x, origin.y), color),
			sf::Vertex(sf::Vector2f(destination.x, destination.y), color)
		};
		window.draw(line.data(), 2, sf::Lines);
	}
};


int main()
{
	MainWindow mw;

	auto settings = sf::ContextSettings();
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Voronoi / Convex hull", sf::Style::Default, settings);
	window.setView(sf::View(sf::FloatRect(0.f, 0.f, 1.f, 1.f)));
	window.setFramerateLimit(60);
	std::vector<sf::CircleShape> dots;


	auto showTriangulation = false, showDiagram = false, showHull = false;
	sf::Color color;
	auto diagram = mw.onV();
	auto triangulation = diagram.computeTriangulation();
	auto hull = mw.onOne();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
			{
				mw.onClick(event.mouseButton.x, event.mouseButton.y);
				auto dot = sf::CircleShape(RADIUS);
				dot.setFillColor(sf::Color::Yellow);
				dot.setPosition(event.mouseButton.x / double(WIDTH) - RADIUS, event.mouseButton.y / double(HEIGHT) - RADIUS);
				dots.push_back(dot);
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::V)
				{
					diagram = mw.onV();
					triangulation = diagram.computeTriangulation();
					showDiagram = !showDiagram;
				}
				else if (event.key.code == sf::Keyboard::T)
				{
					showTriangulation = !showTriangulation;
				}
				else if (event.key.code == sf::Keyboard::Num1)
				{
					hull = mw.onOne();
					showHull = !showHull;
					color = sf::Color::Green;
				}
				else if (event.key.code == sf::Keyboard::Num2)
				{
					auto hull = mw.onTwo();
					showHull = !showHull;
					color = sf::Color::Magenta;
				}
				else if (event.key.code == sf::Keyboard::Num3)
				{
					hull = mw.onThree();
					showHull = !showHull;
					color = sf::Color::Yellow;
				}
			}
		}

		window.clear();
		for (const auto& d : dots)
		{
			window.draw(d);
		}
		if(showDiagram)
			mw.drawDiagram(window, diagram);
		if (showTriangulation)
			mw.drawTriangulation(window, diagram, triangulation);
		if (showHull)
			mw.drawConvexhull(window, hull, color);
		window.display();
	}

	return 0;
}