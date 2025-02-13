#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Screen
{
	View view;
	Clock time;
	Font font;
	Text text1;
	Text text2;
	Vector2u size;
	Vector2u size2;
	Vector2f vSize;
	Image stImage;
	Texture stTexture;
	Sprite stSprite;
	RectangleShape fadeOverlay;
	Button startBut;
	float zoomPow;
	bool isStart = false;
	bool isFullScreen;
	bool isEnd = false; // Начальная непрозрачность = 0
	float fadeSpeed = 1.0f;
	int score;
	int timer;
public:
	RenderWindow window;

	Screen(int width, int height)
	{
		font.loadFromFile("HarryPotterKudosEN-en.ttf");
		text1.setFont(font);
		text2.setFont(font);

		isFullScreen = false;
		stImage.loadFromFile("background.png");
		stImage.createMaskFromColor(Color(47, 95, 115));
		stTexture.loadFromImage(stImage);
		stSprite.setTexture(stTexture);
		stSprite.setTextureRect(IntRect(0, 0, 907, 510));

		fadeOverlay.setFillColor(sf::Color(0, 0, 0, 0));

		window.create(VideoMode(1080, 675), "Reimu's jorney", Style::Close | Style::Titlebar);
		view.reset(sf::FloatRect(0, 0, 1080, 675));
		view.setSize(1080, 675);
		size = window.getSize();
		size2.y = VideoMode::getDesktopMode().height;
		size2.x = VideoMode::getDesktopMode().width;
		isFullScreen = false;
		if (size2.x > 1920) size2.x = 1920;
		if (size2.y > 1080) size2.x = 1080;

		text1.setCharacterSize(48); // Размер шрифта
		text1.setFillColor(sf::Color::Yellow); // Цвет текста
		text2.setCharacterSize(48); // Размер шрифта
		text2.setFillColor(sf::Color::Yellow); // Цвет текста

		score = 0;
		timer = 0;
		startBut.setPosition(32, 32);
		startBut.SetSize(32, 32);
		startBut.isRecVisible = false;
	}

	void setTimer(int timer) { this->timer = timer; }
	void setView(float x, float y) { //функция для считывания координат игрока
		float tempX = x; float tempY = y;//считываем коорд игрока и проверяем их, чтобы убрать края

		if (x < 320) tempX = 320;//убираем из вида левую сторону
		if (y < 240) tempY = 240;//верхнюю сторону
		if (y > 554) tempY = 554;//нижнюю сторону	
		view.setCenter(tempX + 50, tempY + 50); //следим за игроком, передавая его координаты. 
		window.setView(view);
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
	void increseScore() { score++; }
	void showMetrics()
	{
		text1.setString("TiME:" + to_string(timer - (int)time.getElapsedTime().asSeconds()));
		text1.setPosition(view.getCenter().x - size2.x / 4, view.getCenter().y - size2.y / 4);
		window.draw(text1);

		text2.setString("SCORE:" + to_string(score));
		text2.setPosition(view.getCenter().x - size2.x / 4, view.getCenter().y - size2.y / 4 + text1.getCharacterSize());
		window.draw(text2);
		if (timer - (int)time.getElapsedTime().asSeconds() <= 0) { end(); }
	}

	void end()
	{
		fadeOverlay.setSize(Vector2f(window.getSize().x *2, window.getSize().y*2));
		fadeOverlay.setPosition(view.getCenter().x - fadeOverlay.getSize().x/2, view.getCenter().y - fadeOverlay.getSize().y/2);
		Color color;
		while (window.isOpen())
		{
			color = fadeOverlay.getFillColor();
			if (color.a < 255) {
				color.a += fadeSpeed; // Увеличиваем альфа-канал
				fadeOverlay.setFillColor(color);
			}
			
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}
			fullScreen(event);
			window.setView(view);
			window.draw(fadeOverlay);
			text1.setString("the end");
			text1.setCharacterSize(100);
			text1.setPosition(view.getCenter().x - text1.getLocalBounds().width / 2, view.getCenter().y- text1.getLocalBounds().height);
			text2.setPosition(view.getCenter().x-text2.getLocalBounds().width/2, view.getCenter().y + text1.getCharacterSize() - text2.getLocalBounds().height);
			window.draw(text2);
			window.draw(text1);
			window.display();
		}
	}
	
	bool getEnd()
	{
		return isEnd;
	}
	void start()
	{
		 
		if (!isStart) 
		{
			stSprite.setScale(float(window.getSize().y) / float(stSprite.getTextureRect().getSize().y), float(window.getSize().y) / float(stSprite.getTextureRect().getSize().y)); 
			stSprite.setPosition(view.getCenter().x - stSprite.getTextureRect().getSize().x / 2 * stSprite.getScale().y, view.getCenter().y - stSprite.getTextureRect().getSize().y / 2 * stSprite.getScale().y);
			startBut.setPosition(view.getCenter().x, view.getCenter().y);
			while (window.isOpen() and !isStart)
			{

				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();
				}
				
				fullScreen(event);
				window.setView(view);
				window.clear();
				window.draw(stSprite);
				if (startBut.Draw(event, window)) //проверка на отпускание
				{
					stSprite.setTextureRect(IntRect(0, 0, 1, 1));
					isStart = true;
				}
				window.display();
			}
			if(!isFullScreen)view.setSize(size.x * 1.2, size.y * 1.2);
			else view.setSize(size2.x * 1.2 * (float(size.y) / float(size2.y)), size2.y * 1.2 * (float(size.y) / float(size2.y)));
			time.restart();
		}
	}
};

