#include <SFML/Graphics.hpp>
#include <iostream> 
#include "map.h"
#include "view.h"
#include "entity.h"
using namespace sf;

int main()
{
	RenderWindow window(VideoMode(1080, 675), "Lesson 11. kychka-pc.ru", Style::Close | Style::Titlebar);
	view.reset(sf::FloatRect(0, 0, 1080, 675));
	view.setSize(1080, 675);
	view.zoom(1.2);

	bool fullScreen = false;
	Vector2u size;
	size = window.getSize();
	Vector2u size2;
	size2.y = VideoMode::getDesktopMode().height;
	size2.x = VideoMode::getDesktopMode().width;

	Image map_image;
	map_image.loadFromFile("map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);

	Player p(250, 250);
	NPCList n;
	n.create(245, 245, 15);
	n.create(220, 220, 15);


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
		if (event.type == Event::KeyReleased) //проверка на отпускание
		{
			if (event.key.code == Keyboard::F11) //указываешь кнопку
			{
				if (fullScreen) 
				{
					view.setSize(size.x, size.y);
					fullScreen = false;
					window.create(VideoMode(size.x, size.y), "Lesson 11. kychka-pc.ru", Style::Close | Style::Titlebar);
					view.zoom(1.2);
				}
				else
				{
					fullScreen = true;
					window.create(VideoMode(size2.x, size2.y), "Lesson 11. kychka-pc.ru", Style::Fullscreen);
					view.setSize(size2.x, size2.y);
					view.zoom(1.2*float(size.y)/float(size2.y));
				}
			}
		}

		///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
		p.movement(event);

		p.setView();
		p.update();



		window.setView(view);
		window.clear();

		/////////////////////////////Рисуем карту/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));

				s_map.setPosition(j * 32, i * 32);

				window.draw(s_map);
			}
		n.moves();
		n.update();
		n.draw(window);
		p.draw(window);
		window.display();
	}

	return 0;
}