#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

RenderWindow window;

class Screen
{
	Vector2u size;
	Vector2u size2;
	Vector2f vSize;
	Image stImage;
	Texture stTexture;
	Sprite stSprite;
	float zoomPow;
	bool isStart = false;
	bool isFullScreen;
public:
	Screen(int width, int height)
	{
		isFullScreen = false;
		stImage.loadFromFile("background.png");
		stImage.createMaskFromColor(Color(47, 95, 115));
		stTexture.loadFromImage(stImage);
		stSprite.setTexture(stTexture);
		stSprite.setTextureRect(IntRect(0, 0, 907, 510));
		

		window.create(VideoMode(1080, 675), "Reimu's jorney", Style::Close | Style::Titlebar);
		view.reset(sf::FloatRect(0, 0, 1080, 675));
		view.setSize(1080, 675);
		size = window.getSize();
		size2.y = VideoMode::getDesktopMode().height;
		size2.x = VideoMode::getDesktopMode().width;
		isFullScreen = false;
		if (size2.x > 1920) size2.x = 1920;
		if (size2.y > 1080) size2.x = 1080;
	}
	void fullScreen(Event& event) 
	{
		if (event.type == Event::KeyReleased) //проверка на отпускание
		{
			if (event.key.code == Keyboard::F11) //указываешь кнопку
			{
				if (isFullScreen)
				{
					isFullScreen = false;
					window.create(VideoMode(size.x, size.y), "Reimu's jorney", Style::Close | Style::Titlebar);
					if (isStart)view.setSize(size.x * 1.2, size.y * 1.2);
					else view.setSize(size.x, size.y);
				}
				else
				{
					isFullScreen = true;
					window.create(VideoMode(size2.x, size2.y), "Reimu's jorney", Style::Fullscreen);
					if(isStart)view.setSize(size2.x * 1.2 * (float(size.y) / float(size2.y)), size2.y * 1.2 * (float(size.y) / float(size2.y)));
					else view.setSize(size2.x * (float(size.y) / float(size2.y)), size2.y * (float(size.y) / float(size2.y)));
				}
			}
		}

	}
	void end()
	{

	}
	void start(RenderWindow& window)
	{
		 
		if (!isStart) 
		{
			stSprite.setScale(float(window.getSize().y) / float(stSprite.getTextureRect().getSize().y), float(window.getSize().y) / float(stSprite.getTextureRect().getSize().y)); 
			stSprite.setPosition(view.getCenter().x - stSprite.getTextureRect().getSize().x / 2 * stSprite.getScale().y, view.getCenter().y - stSprite.getTextureRect().getSize().y / 2 * stSprite.getScale().y);
			while (window.isOpen() and !isStart)
			{

				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();
				}
				if (Keyboard::isKeyPressed(Keyboard::Enter)) //проверка на отпускание
				{
					stSprite.setTextureRect(IntRect(0, 0, 1, 1));
					isStart = true;
				}
				fullScreen(event);
				window.setView(view);
				window.clear();
				window.draw(stSprite);
				window.display();
			}
			if(!isFullScreen)view.setSize(size.x * 1.2, size.y * 1.2);
			else view.setSize(size2.x * 1.2 * (float(size.y) / float(size2.y)), size2.y * 1.2 * (float(size.y) / float(size2.y)));
		}
	}
};

