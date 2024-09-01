#include <SFML/Graphics.hpp>
#include <iostream> 
#include "map.h"
#include "view.h"
#include "entity.h"
#include "Screen.h"
using namespace sf;

int main()
{
	srand(time(NULL));
	Room r1;
	Screen screen(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);
	

	Player p(250, 250);
	NPCList n;
	n.create(500, 245, 15);
	n.create(220, 220, 15);
	n.create(748, 693, 15);


	while (window.isOpen())
	{
		sf::Event event;

		p.reset();
		n.reset();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		screen.fullScreen(event);
		///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
		p.movement(event);

		p.setView();
		p.update();



		window.setView(view);
		window.clear();

		/////////////////////////////Рисуем карту/////////////////////
		
		n.update();
		n.moves();
		r1.draw(window);
		n.draw(window);
		p.draw(window);
		screen.start(window);
		window.display();
	}

	return 0;
}