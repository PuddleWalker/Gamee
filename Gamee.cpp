#include <SFML/Graphics.hpp>
#include <iostream> 
#include "map.h"
#include "view.h"
using namespace sf;



////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player {
private: float x, y;
public:
	float w, h, dx, dy, speed;
	int dir;
	int direct = 2;

	float CurrentFrame = 0;
	float dodgeFrame = 0;

	int isDodge = 0;

	float time;
	float dodgeTime;
	Clock clock;
	Clock dodgeClock;

	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Player(String F, float X, float Y, float W, float H) {
		dx = 0; dy = 0; speed = 0; dir = 0;
		File = F;
		w = W; h = H;
		image.loadFromFile(File);
		image.createMaskFromColor(Color(47, 95, 115));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;

	}
	void update()
	{
		switch (dir)
		{
		case 0: dx = speed; dy = 0; break;
		case 1: dx = -speed; dy = 0; break;
		case 2: dx = 0; dy = speed; break;
		case 3: dx = 0; dy = -speed; break;
		}

		x += dx * time;
		y += dy * time;

		speed = 0;
		sprite.setPosition(x, y);
		interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой
	}

	void reset()
	{
		time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		dodgeTime = dodgeClock.getElapsedTime().asMicroseconds();
		if (isDodge) dodgeClock.restart();
		dodgeTime = dodgeTime / 800;
	}

	void interactionWithMap()//ф-ция взаимодействия с картой
	{

		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; j < (x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
			{
				if (TileMap[i][j] == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
				{
					if (dy > 0)//если мы шли вниз,
					{
						y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					}
					if (dy < 0)
					{
						y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					}
					if (dx > 0)
					{
						x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//аналогично идем влево
					}
				}

				if (TileMap[i][j] == 's') { //если символ равен 's' (камень)
					x = 300; y = 300;//какое то действие... например телепортация героя
					TileMap[i][j] = ' ';//убираем камень, типа взяли бонус. можем и не убирать, кстати.
				}
			}
	}
	void movement(Event event) {
		if (event.type == Event::MouseButtonPressed)
		{

			if (Mouse::isButtonPressed(Mouse::Left)) {}
			if (Mouse::isButtonPressed(Mouse::Right)) {
				if (dodgeClock.getElapsedTime().asMicroseconds() > 200000)
				{
					if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A) or
						Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W) or
						Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D) or
						Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) isDodge = 2;
					else isDodge = 1;
					dodgeClock.restart();
					dodgeTime = dodgeTime / 800;
				}
			}
		}
		if (isDodge == 2)
		{
			if (dodgeFrame < 6)
			{
				speed = 0.35;
				dodgeFrame += 0.0125 * dodgeTime;
			}
			else dodgeFrame += 0.009 * dodgeTime;
			if (dodgeFrame > 9)
			{
				dodgeFrame -= 9;
				isDodge = 0;
				dodgeClock.restart();
			}
			if (direct == 2) {
				sprite.setTextureRect(IntRect(107 * int(dodgeFrame), 324, 108, 124));
			}
			else {
				sprite.setTextureRect(IntRect(107 * (int(dodgeFrame) + 1) + 1, 324, -108, 124));
			}
		}
		else
		{
			if (isDodge == 1) {
				if (dodgeFrame < 5)
				{
					speed = 0.35;
					dodgeFrame += 0.0125 * dodgeTime;
				}
				else dodgeFrame += 0.009 * dodgeTime;
				if (dodgeFrame > 8)
				{
					dodgeFrame -= 8;
					isDodge = 0;
					dodgeClock.restart();
				}
				if (direct == 2) {
					dir = 1;
					sprite.setTextureRect(IntRect(149 * int(dodgeFrame), 213, 150, 112));
				}
				else {
					dir = 0;
					sprite.setTextureRect(IntRect(149 * (int(dodgeFrame) + 1) + 1, 213, -150, 112));
				}
			}

			else
			{
				if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
					dir = 1; speed = 0.1;
					CurrentFrame += 0.005 * time;
					if (CurrentFrame > 8) CurrentFrame -= 8;
					sprite.setTextureRect(IntRect(117 * (int(CurrentFrame) + 1) + 1, 105, -118, 109));
					direct = 1;
				}

				if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
					dir = 0; speed = 0.1;
					CurrentFrame += 0.005 * time;
					if (CurrentFrame > 8) CurrentFrame -= 8;
					sprite.setTextureRect(IntRect(117 * int(CurrentFrame), 105, 118, 109));
					direct = 2;
				}

				if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
					dir = 3; speed = 0.1;
					CurrentFrame += 0.005 * time;
					if (CurrentFrame > 8) CurrentFrame -= 8;
					if (direct == 1)sprite.setTextureRect(IntRect(117 * (int(CurrentFrame) + 1) + 1, 104, -118, 110));
					else sprite.setTextureRect(IntRect(117 * int(CurrentFrame), 104, 118, 110));
				}

				if ((Keyboard::isKeyPressed(Keyboard::S) or (Keyboard::isKeyPressed(Keyboard::Down)))) {
					dir = 2; speed = 0.1;
					CurrentFrame += 0.005 * time;
					if (CurrentFrame > 8) CurrentFrame -= 8;
					if (direct == 1)sprite.setTextureRect(IntRect(117 * (int(CurrentFrame) + 1) + 1, 104, -118, 110));
					else sprite.setTextureRect(IntRect(117 * int(CurrentFrame), 104, 118, 110));
				}

				if (speed == 0)
				{
					CurrentFrame += 0.007 * time;
					if (CurrentFrame > 10) CurrentFrame -= 10;
					if (direct == 1)sprite.setTextureRect(IntRect(102 * (int(CurrentFrame) + 1) + 1, 0, -103, 105));
					else sprite.setTextureRect(IntRect(102 * int(CurrentFrame), 0, 103, 105));
				}
			}
		}
	}

	float getplayercoordinateX() {
		return x;
	}
	float getplayercoordinateY() {
		return y;
	}

};



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

	Player p("sprites.png", 250, 250, 96.0, 96.0);


	while (window.isOpen())
	{
		sf::Event event;

		p.reset();

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
		float coordinatePlayerX, coordinatePlayerY = 0;
		coordinatePlayerX = p.getplayercoordinateX();
		coordinatePlayerY = p.getplayercoordinateY();

		///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
		p.movement(event);

		getplayercoordinateforview(coordinatePlayerX, coordinatePlayerY);
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


		window.draw(p.sprite);
		window.display();
	}

	return 0;
}