#include <SFML/Graphics.hpp>
#include <iostream> 
#include "Interface.h"
#include "map.h"
#include "view.h"
#include "entity.h"
#include "Screen.h"
using namespace sf;

int main()
{
	srand(time(NULL));
	Screen screen(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);
	while (!screen.getEnd()) {
		Room r1;
		Player p(250, 250);
		NPCList n;
		
		for (int i = 0; i < 3; i++)
		{
			n.create((rand() % 46 + 2) * 32, (rand() % 21 + 2) * 32, (rand() % 50 + 1));
		}
		screen.setTimer(120);
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
			if (n.getCount() < 3)
			{
				n.create((rand() % 46 + 2) * 32, (rand() % 21 + 2) * 32, (rand() % 50 + 1));
				screen.increseScore();
			}
			
			
			screen.window.clear();

			/////////////////////////////Рисуем карту/////////////////////
			
			n.update(r1);
			IntRect rec = p.getHit();
			float x = p.getX();
			float y = p.getY();
			n.moves(x, y, rec);
			r1.draw(screen.window);
			n.draw(screen.window);
			p.draw(screen.window);
			screen.start();
			screen.setView(x, y);
			screen.showMetrics();
			screen.window.display();
		}

	}
	return 0;
}