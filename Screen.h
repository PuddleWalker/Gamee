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
	Text scoreText;
	Vector2u size;
	Vector2u size2;
	Vector2f vSize;
	Image stImage;
	Texture stTexture;
	Sprite stSprite;
	RectangleShape fadeOverlay;
	sf::RectangleShape back;
	Button startBut;
	Button exitBut;

	sf::RectangleShape totHealthRec;
	sf::RectangleShape curHealthRec;
	float zoomPow;
	bool isStart = false;
	bool isFullScreen;
	bool isEnd = false; // Начальная непрозрачность = 0
	float fadeSpeed = 1.0f;
	int score;
	int record;
	
public:
	RenderWindow window;

	Screen(int width, int height)
	{
		font.loadFromFile("BLoq.ttf");
		text1.setFont(font);
		text2.setFont(font);
		scoreText.setFont(font);

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
		if (size2.y > 1080) size2.y = 1080;

		text1.setCharacterSize(48); // Размер шрифта
		text1.setFillColor(sf::Color::Yellow); // Цвет текста
		text2.setCharacterSize(48); // Размер шрифта
		text2.setFillColor(sf::Color::Yellow); // Цвет текста
		scoreText.setCharacterSize(48); // Размер шрифта
		scoreText.setFillColor(sf::Color::Yellow); // Цвет текста
		back.setFillColor(sf::Color(38, 29, 69));
		back.setSize(sf::Vector2f(size2.x, size2.y));

		score = 0;

		startBut.SetFont("BLoq.ttf");
		exitBut.SetFont("BLoq.ttf");
		startBut.SetText("Start");
		exitBut.SetText("Exit");
		startBut.SetTextColor(sf::Color::Yellow);
		exitBut.SetTextColor(sf::Color::Yellow);
		startBut.SetTextSize(60);
		exitBut.SetTextSize(60);
		startBut.SetSize(startBut.GetTextSize());
		exitBut.SetSize(exitBut.GetTextSize());
		startBut.SetPosition(view.getCenter().x + size.x / 2 - startBut.GetSize().x, view.getCenter().y);
		exitBut.SetPosition(view.getCenter().x + size.x/2 - startBut.GetSize().x, view.getCenter().y + startBut.GetSize().y * 1.5);
		startBut.isRecVisible = false;
		exitBut.isRecVisible = false;

		totHealthRec.setSize(sf::Vector2f(400, 40));
		curHealthRec.setSize(sf::Vector2f(400, 40));
		curHealthRec.setFillColor(sf::Color::Red);
		totHealthRec.setOutlineThickness(3);
		totHealthRec.setOutlineColor(sf::Color(99, 74, 52));

		std::ifstream file("record.txt");
		file >> record;
		file.close();
		text2.setString("Current record:" + to_string(record) + "!");
		text2.setPosition(view.getCenter().x - text2.getLocalBounds().width/2, view.getCenter().y - text2.getLocalBounds().height / 2 - size.y/3);
	}
	void ShowPlayerHP(std::pair<int, int> health)
	{
		if (health.first <= 0) end(false);
		totHealthRec.setPosition(view.getCenter().x - size.x / 2, view.getCenter().y - size.y / 2);
		curHealthRec.setPosition(view.getCenter().x - size.x / 2, view.getCenter().y - size.y / 2);
		curHealthRec.setSize(sf::Vector2f(totHealthRec.getSize().x * health.first / health.second , curHealthRec.getSize().y));
		window.draw(totHealthRec);
		window.draw(curHealthRec);
	}
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
		text1.setString("Time:" + to_string((int)time.getElapsedTime().asSeconds()));
		text1.setPosition(view.getCenter().x - size2.x / 4, view.getCenter().y - size2.y / 4);
		window.draw(text1);
	}
	void ShowBackground()
	{
		back.setPosition(view.getCenter().x- back.getSize().x/2, view.getCenter().y - back.getSize().y / 2);
		window.draw(back);
	}
	void end(bool isWin)
	{
		fadeOverlay.setSize(Vector2f(window.getSize().x *2, window.getSize().y*2));
		fadeOverlay.setPosition(view.getCenter().x - fadeOverlay.getSize().x/2, view.getCenter().y - fadeOverlay.getSize().y/2);
		text1.setCharacterSize(200);
		if (isWin)
		{
			score = time.getElapsedTime().asSeconds();
			if (score < record)
			{
				ofstream file("record.txt");
				file << score;
				file.close();
				record = score;
			}
			text1.setString("The end");
			text2.setString("Current record:" + to_string(record));
			scoreText.setString("\nYour score:" + to_string(score));
		}
		else
		{
			text1.setString("Game over");
			text2.setString("Current record:" + to_string(record));
		}
		text1.setPosition(view.getCenter().x - text1.getLocalBounds().width / 2, view.getCenter().y - text1.getLocalBounds().height - size.y / 3);
		text2.setPosition(text1.getPosition().x + text1.getLocalBounds().width / 2 - text2.getLocalBounds().width / 2, text1.getPosition().y + text1.getLocalBounds().height + text1.getLocalBounds().top);
		scoreText.setPosition(text2.getPosition().x + text2.getLocalBounds().width / 2 - scoreText.getLocalBounds().width / 2, text2.getPosition().y + text2.getLocalBounds().height - text2.getLocalBounds().top);
		startBut.SetText("Try again");
		startBut.isRecVisible = true;
		exitBut.isRecVisible = true;
		startBut.SetColor(sf::Color::Black);
		exitBut.SetColor(sf::Color::Black);
		startBut.SetOutlineColor(sf::Color::White);
		exitBut.SetOutlineColor(sf::Color::White);
		startBut.SetOutlineThickness(8);
		exitBut.SetOutlineThickness(8);
		startBut.SetColorChange(sf::Color(255, 255, 255, 20));
		exitBut.SetColorChange(sf::Color(255, 255, 255, 20));
		startBut.SetSize(startBut.GetTextSize().x * 1.2, startBut.GetTextSize().y * 1.5);
		exitBut.SetSize(startBut.GetSize());
		startBut.SetPosition(view.getCenter().x - startBut.GetSize().x * 2/3, view.getCenter().y + startBut.GetSize().y);
		exitBut.SetPosition(view.getCenter().x + startBut.GetSize().x*2/3, view.getCenter().y + startBut.GetSize().y);
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
			
			window.draw(text2);
			window.draw(text1);
			if (isWin)window.draw(scoreText);
			if (startBut.Draw(event, window))
			{
				time.restart();
				text1.setCharacterSize(48);
				break;
			}
			if (exitBut.Draw(event, window)) window.close();
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
				if (exitBut.Draw(event, window)) window.close();
				window.draw(text2);
				window.display();
			}
			if(!isFullScreen)view.setSize(size.x * 1.2, size.y * 1.2);
			else view.setSize(size2.x * 1.2 * (float(size.y) / float(size2.y)), size2.y * 1.2 * (float(size.y) / float(size2.y)));
			time.restart();
		}
	}
};

