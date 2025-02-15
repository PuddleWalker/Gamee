#include <SFML/Graphics.hpp>
#include <iostream> 
#include "Interface.h"
#include "map.h"
#include "entity.h"
#include "Screen.h"
using namespace sf;
int roundUp(double value) {
	return (value == static_cast<int>(value)) ? static_cast<int>(value) : static_cast<int>(value) + 1;
}
sf::Color randCol() 
{
	switch (rand() % 3)
	{
	case 0:
		return(sf::Color::Blue);
		break;
	case 1:
		return(sf::Color::Red);
		break;
	case 2:
		return(sf::Color::Green);
		break;
	}
}
int main()
{
	srand(time(NULL));
	Screen screen(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);
	while (!screen.getEnd()) {
		Room r1;
		
		Player p(250, 250);
		NPCList n;
		int goals = 5;
		for (int i = 0; i < 5; i++)
		{
			n.create(r1.WIDTH_MAP, r1.HEIGHT_MAP, (rand() % 50 + 1), randCol());
		}
		while (screen.window.isOpen())
		{
			sf::Event event;

			p.reset();
			n.reset();

			while (screen.window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					screen.window.close();
			}
			screen.fullScreen(event);
			///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
			p.movement(event);
			p.update(r1);
			if (n.getCount() < 5)
			{

				n.create(r1.WIDTH_MAP, r1.HEIGHT_MAP, (rand() % 50 + 1), randCol());
				goals--;
			}
			
			
			screen.window.clear();

			/////////////////////////////Рисуем карту/////////////////////
			
			n.update(r1);
			sf::Sprite rec = p.getHit();
			float x = p.getX();
			float y = p.getY();
			n.moves(p); 
			screen.ShowBackground();
			r1.AddToDraw(std::make_pair(roundUp((y + p.getMinHigh())/(float)32)-1, [&p](sf::RenderWindow& window) { p.draw(window); }));
			for (size_t i = 0; i < n.getCount(); i++) {
				r1.AddToDraw(std::make_pair(
					roundUp((n[i].getY() + n[i].getMinHigh()) / (float)32) - 1,
					[&n, i](sf::RenderWindow& window) { n[i].draw(window); }
				));
			}
			r1.draw(screen.window);
			n.draw(screen.window);
			screen.start();
			screen.setView(rec.getPosition().x, rec.getPosition().y);
			screen.showMetrics();
			screen.ShowPlayerHP(p.getHP());
			screen.window.display();
			if (goals <= 0)
			{
				screen.end(true);
				break;
			}
		}

	}
	return 0;
}