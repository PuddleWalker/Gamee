#pragma once
#include <list>

using namespace std;
class Entity
{
public:

};


class Character
{

protected:
	float time;
	float attackTime;
	Clock clock;
	Clock attackClock;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	float CurrentFrame = 0;
	float attackFrame = 0;
	bool isAttack = 0;

	float x, y;
	float w, h, dx, dy, speed;
	int dir;
	int direct = 2;

public:
	
	const void draw(RenderWindow& window) {
		window.draw(sprite);
	}
	void reset()
	{
		time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
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
};

class NPC : public Character
{
	friend class NPCList;
public:
	NPC(String F, float X, float Y, float W, float H) {
		dx = 0; dy = 0; speed = 0; dir = 0;
		File = F;
		w = W; h = H;
		image.loadFromFile(File);
		image.createMaskFromColor(Color(47, 95, 115));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
	}
	void moves()
	{
		CurrentFrame += 0.007 * time;
		if (CurrentFrame > 8) CurrentFrame -= 8;
		sprite.setTextureRect(IntRect(48 * int(CurrentFrame), 0, 49, 34));
	}
};

class NPCList
{
	static list<NPC> NPCs;
	static list<NPC>::iterator it;
public:
	void create(String F, float X, float Y, float W, float H)
	{
		NPCs.emplace_back(F, X, Y, W, H);
	}
	NPC& operator[](int num)
	{
		it = NPCs.begin();
		advance(it, num);
		return *it;
	}
	void reset()
	{
		for (it = NPCs.begin(); it != NPCs.end(); it++)
		{
			(*it).reset();
		}
	}

	void moves()
	{
		for (it = NPCs.begin(); it != NPCs.end(); it++)
		{
			(*it).moves();
		}
	}

	void update()
	{
		for (it = NPCs.begin(); it != NPCs.end(); it++)
		{
			(*it).update();
		}
	}

	void draw(RenderWindow& window)
	{
		for (it = NPCs.begin(); it != NPCs.end(); it++)
		{
			(*it).draw(window);
		}
	}
};
list<NPC>::iterator NPCList::it;
list<NPC> NPCList::NPCs;

class Player : public Character
{

private: 
	   float dodgeFrame = 0;

	   int attackCount = 1;
	   int isDodge = 0;
	   float dodgeTime;
	   Clock dodgeClock;
public:
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

	void reset()
	{
		time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		dodgeTime = dodgeClock.getElapsedTime().asMicroseconds();
		if (isDodge) dodgeClock.restart();
		dodgeTime = dodgeTime / 800;
		attackTime = attackClock.getElapsedTime().asMicroseconds();
		if(isAttack)attackClock.restart();
		attackTime = attackTime / 800;
	}

	void movement(Event event) {
		if (event.type == Event::MouseButtonPressed)
		{

			if (Mouse::isButtonPressed(Mouse::Left)) 
			{
				isAttack = true;
				if (attackTime > 313) attackCount = 1;
				if (isAttack and attackTime > 313)
				{
					attackClock.restart();
					attackTime = attackClock.getElapsedTime().asMicroseconds();
					attackTime = attackTime / 800;
				}
			}
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
					isAttack = false;
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
			if (dodgeFrame <= 1)attackClock.restart();
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
				if(isAttack)
				{
					int a, b, c, d, e; float f;
					switch (attackCount)
					{
					case 1:
						b = 447; c = 102; d = 118; e = 7; f = 0.012;
						break;
					case 2:
						b = 564; c = 136; d = 96; e = 5; f = 0.01;
						break;
					case 3:
						b = 659; c = 152; d = 96; e = 11; f = 0.015;
						break;
					case 4:
						b = 754; c = 126; d = 100; e = 10; f = 0.014;
						break;
					case 5:
						b = 853; c = 123; d = 111; e = 8; f = 0.012;
						break;
					case 6:
						b = 963; c = 116; d = 100; e = 10; f = 0.014;
						break;
					}
					a = c - 1;
					attackFrame += f * attackTime;
					if (attackFrame > e)
					{
						attackFrame -= e;
						if (attackCount < 6)attackCount++;
						else attackCount = 1;
						isAttack = false;
					}
					if (direct == 2) {
						sprite.setTextureRect(IntRect(a * int(attackFrame), b, c, d));
					}
					else {
						sprite.setTextureRect(IntRect(a * (int(attackFrame) + 1) + 1, b, -c, d));
					}
					
				}
				else {
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
	}
	void setView()
	{
		getplayercoordinateforview(x, y);
	}
};
