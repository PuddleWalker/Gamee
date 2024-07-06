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
	void update(float time)
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


	float getplayercoordinateX() {
		return x;
	}
	float getplayercoordinateY() {
		return y;
	}

};



int main()
{
	int direct = 2;
	Clock dodgeClock;
	RenderWindow window(sf::VideoMode(640, 480), "Lesson 11. kychka-pc.ru");


	view.reset(sf::FloatRect(0, 0, 640, 480));

	Image map_image;
	map_image.loadFromFile("map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);


	Player p("sprites.png", 250, 250, 96.0, 96.0);

	float CurrentFrame = 0;
	float dodgeFrame = 0;
	Clock clock;
	bool isDodge = false;

	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		float dodgeTime = dodgeClock.getElapsedTime().asMicroseconds();
		if(isDodge) dodgeClock.restart();
		dodgeTime = dodgeTime / 800;


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float coordinatePlayerX, coordinatePlayerY = 0;
		coordinatePlayerX = p.getplayercoordinateX();
		coordinatePlayerY = p.getplayercoordinateY();

		///////////////////////////////////////////Управление персонажем с анимацией////////////////////////////////////////////////////////////////////////
		if (event.type == Event::MouseButtonPressed)
		{

			if (Mouse::isButtonPressed(Mouse::Left)) {}
			if (Mouse::isButtonPressed(Mouse::Right)) {
				if (dodgeClock.getElapsedTime().asMicroseconds() > 200000)
				{
					isDodge = true;
					dodgeClock.restart();
					dodgeTime = dodgeTime / 800;
				}
			}
		}
		if (isDodge)
		{
			dodgeFrame += 0.0075 * dodgeTime;
			if (dodgeFrame > 3)
			{
				dodgeFrame -= 3;
				isDodge = false;
				dodgeClock.restart();
			}
			if (direct == 2) {
				p.dir = 1; p.speed = 0.25;
				p.sprite.setTextureRect(IntRect(123 * int(dodgeFrame), 214, 124, 100));
			}
			else {
				p.dir = 0; p.speed = 0.25;
				p.sprite.setTextureRect(IntRect(123 * (int(dodgeFrame)+1)+1, 214, -124, 100));
			}
		}
		else
		{
			if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
				p.dir = 1; p.speed = 0.1;
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 8) CurrentFrame -= 8;
				p.sprite.setTextureRect(IntRect(117 * (int(CurrentFrame) + 1) + 1, 105, -118, 109));
				direct = 1;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
				p.dir = 0; p.speed = 0.1;
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 8) CurrentFrame -= 8;
				p.sprite.setTextureRect(IntRect(117 * int(CurrentFrame), 105, 118, 109));
				direct = 2;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
				p.dir = 3; p.speed = 0.1;
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 8) CurrentFrame -= 8;
				if (direct == 1)p.sprite.setTextureRect(IntRect(117 * (int(CurrentFrame) + 1) + 1, 104, -118, 110));
				else p.sprite.setTextureRect(IntRect(117 * int(CurrentFrame), 104, 118, 110));
			}

			if ((Keyboard::isKeyPressed(Keyboard::S) or (Keyboard::isKeyPressed(Keyboard::Down)))) {
				p.dir = 2; p.speed = 0.1;
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 8) CurrentFrame -= 8;
				if (direct == 1)p.sprite.setTextureRect(IntRect(117 * (int(CurrentFrame) + 1) + 1, 104, -118, 110));
				else p.sprite.setTextureRect(IntRect(117 * int(CurrentFrame), 104, 118, 110));
			}

			if (p.speed == 0)
			{
				CurrentFrame += 0.007 * time;
				if (CurrentFrame > 10) CurrentFrame -= 10;
				if (direct == 1)p.sprite.setTextureRect(IntRect(102 * (int(CurrentFrame) + 1) + 1, 0, -103, 105));
				else p.sprite.setTextureRect(IntRect(102 * int(CurrentFrame), 0, 103, 105));
			}
		}

		getplayercoordinateforview(coordinatePlayerX, coordinatePlayerY);
		p.update(time);



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