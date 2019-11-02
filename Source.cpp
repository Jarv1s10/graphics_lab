#include <iostream>
#include <vector>
#include <set>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "FortuneAlgorithm.h"
using namespace mygal;


const int RADIUS = 0.005, WIDTH = 800, HEIGHT = 800;
bool LOCK_FLAG = false;
constexpr float Offset = 1.0f;

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
		LOCK_FLAG = true;
		return diagram;
	}

	void drawPoints(sf::RenderWindow& window, const Diagram<double>& diagram)
	{
		for (const auto& site : diagram.getSites())
			drawPoint(window, site.point, sf::Color(100, 250, 50));
	}

	void drawLines(sf::RenderWindow& window, Diagram<double>& diagram)
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
					drawEdge(window, origin, destination, sf::Color::Red);
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
				drawEdge(window, origin, destination, sf::Color::Green);
			}
		}
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
	void drawPoint(sf::RenderWindow& window, Vector2<double> point, sf::Color color)
	{
		auto shape = sf::CircleShape(RADIUS);
		shape.setPosition(sf::Vector2f(point.x - RADIUS, point.y - RADIUS));
		shape.setFillColor(color);
		window.draw(shape);
	}
};


int main()
{
	MainWindow mw;
	auto settings = sf::ContextSettings();
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Voronoi", sf::Style::Default, settings);
	window.setView(sf::View(sf::FloatRect(0.f, 0.f, 1.f, 1.f)));
	window.setFramerateLimit(60);
	auto showTriangulation = false;
	auto diagram = mw.onV();
	auto triangulation = diagram.computeTriangulation();
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
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::V)
				{
					diagram = mw.onV();
					triangulation = diagram.computeTriangulation();
				}
				else if (event.key.code == sf::Keyboard::T)
				{
					showTriangulation = !showTriangulation;
				}
			}
		}

		window.clear();
		mw.drawPoints(window, diagram);
		mw.drawLines(window, diagram);
		if (showTriangulation)
			mw.drawTriangulation(window, diagram, triangulation);
		window.display();
	}

	return 0;
}