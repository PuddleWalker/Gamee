#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <ctime>
class Player;
using namespace std;

class Character;
class slime;
class NPCList;
class Entity
{
public:

};


class Character
{

protected:
	float time = 0;
	float attackTime = 0;
	sf::Clock clock;
	sf::Clock attackClock;
	sf::String File;
	sf::String attFile;
	sf::Image image;
	sf::Image attImage;
	sf::Texture texture;
	sf::Texture attTexture;
	sf::Sprite sprite;
	sf::Sprite attSprite;
	float CurrentFrame = 0;
	float attackFrame = 0;
	bool isAttack = 0;
	int TotalHP = 100;
	int CurrentHP = 100;
	int MinHigh = 0;
	int MinWidth = 0;

	float x, y = 0;
	float dx = 0, dy = 0, speed = 0;
	int dir = 0;
	int direct = 2;

public:
	const sf::FloatRect getRect()
	{
		return sf::FloatRect(x, y, MinWidth, MinHigh);
	}
	const sf::FloatRect getAttRect()
	{
		if (direct == 2) return sf::FloatRect(x + MinWidth, y + MinHigh - attSprite.getTextureRect().getSize().y, attSprite.getTextureRect().getSize().x, attSprite.getTextureRect().getSize().y);
		else return sf::FloatRect(x - attSprite.getTextureRect().getSize().x, y + MinHigh - attSprite.getTextureRect().getSize().y, attSprite.getTextureRect().getSize().x, attSprite.getTextureRect().getSize().y);

	}
	const void draw(sf::RenderWindow& window)
	{
		sprite.setOrigin(0, sprite.getTextureRect().getSize().y-MinHigh);
		if (direct == 2) sprite.setOrigin(0, sprite.getOrigin().y);
		else sprite.setOrigin(-MinWidth-sprite.getTextureRect().getSize().x, sprite.getOrigin().y);
		window.draw(sprite);
		window.draw(attSprite);
	}
	void reset()
	{
		time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
	}

	float getX() { return x; }
	float getY() { return y; }
	void update(Room& r1)
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
		if(direct == 2)attSprite.setPosition(x + MinWidth, y+MinHigh - attSprite.getTextureRect().getSize().y);
		else attSprite.setPosition(x - attSprite.getTextureRect().getSize().x, y + MinHigh - attSprite.getTextureRect().getSize().y);
		interactionWithMap(r1);//вызываем функцию, отвечающую за взаимодействие с картой
	}
	private:
	virtual void interactionWithMap(Room& r1) = 0;
};

class slime : public Character
{
	friend class Player;
	friend class NPCList;
	sf::Clock timer;
	float rWait;
	float pX1, pY1;
	bool isDamaged;
public:
	slime(float X, float Y, int HP) {
		TotalHP = HP; CurrentHP = HP; MinHigh = 34; MinWidth = 49;
		isDamaged = false;
		rWait = rand() % 3 + 3;
		File = "slime.png";
		image.loadFromFile(File);
		image.createMaskFromColor(sf::Color(47, 95, 115));
		image.createMaskFromColor(sf::Color(0, 0, 0));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		pX1 = rand() % 200 - 100 + x;
		pY1 = rand() % 200 - 100 + y;

		attFile = "pAttack.png";
		attImage.loadFromFile(attFile);
		attImage.createMaskFromColor(sf::Color(47, 95, 115));
		attTexture.loadFromImage(attImage);
		attSprite.setTexture(attTexture);
		attSprite.setTextureRect(sf::IntRect(0, 0, 60, 100));
	}
	void Attacked(sf::FloatRect attHit, int dam)
	{
		if (attHit.intersects(getRect()))
		{
			CurrentHP -= dam;
			isDamaged = true;
		}
	}
private:
	void interactionWithMap(Room& r1) override//ф-ция взаимодействия с картой
	{
		for (int i = y / 32; i < (y + MinHigh) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; j < (x + MinWidth) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
			{
				if (r1.GetTile(i, j) == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
				{
					if (pY1 - y > 0)//если мы шли вниз,
					{
						if (r1.GetTile(i - 1, j) != '0')y = i * 32 - MinHigh;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					}
					if (pY1 - y < 0)
					{
						if (r1.GetTile(i + 1, j) != '0')y = i * 32 + 33;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					}
					if (pX1 - x > 0)
					{
						if (r1.GetTile(i, j - 1) != '0')x = j * 32 - MinWidth;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (pX1 - x < 0)
					{
						if (r1.GetTile(i, j + 1) != '0')x = j * 32 + 32;//аналогично идем влево
					}
				}
			}
	}
	void moves(float px, float py, sf::IntRect pHit)
	{
		if(isDamaged)
		{
			CurrentFrame += 0.005 * time;
			if (direct == 2)
			{
				sprite.setTextureRect(sf::IntRect(92 * int(CurrentFrame), 103, 93, 37));
			}
			else
			{
				sprite.setTextureRect(sf::IntRect(92 * (int(CurrentFrame) + 1) + 1, 103, -93, 37));
			}
			if (CurrentFrame > 6) isDamaged = false;
		}
		else
		{
			float pX, pY;
			pX = px - sprite.getTextureRect().getSize().x;
			pY = py;
			if (px < x)pX = px + pHit.getSize().x;

			if (py < y) pY = py + pHit.getSize().y - sprite.getTextureRect().getSize().y;
			if (sqrt((x - pX) * (x - pX) + (y - pY) * (y - pY)) < 350 && sqrt((x - pX) * (x - pX) + (y - pY) * (y - pY)) > 10)
			{
				float distance = sqrt((pX - x) * (pX - x) + (pY - y) * (pY - y));//считаем дистанцию (длину от точки А до точки Б). формула длины вектора
				if (px < x)direct = 1;
				else direct = 2;
				if (distance > 2) {//этим условием убираем дергание во время конечной позиции спрайта

					x += 0.075 * time * (pX - x) / distance;//идем по иксу с помощью вектора нормали
					y += 0.075 * time * (pY - y) / distance;//идем по игреку так же
				}
				if (direct == 2)
				{
					CurrentFrame += 0.005 * time;
					if (CurrentFrame > 8) CurrentFrame -= 8;
					sprite.setTextureRect(sf::IntRect(55 * int(CurrentFrame), 34, 56, 35));
				}
				else
				{
					CurrentFrame += 0.005 * time;
					if (CurrentFrame > 8) CurrentFrame -= 8;
					sprite.setTextureRect(sf::IntRect(55 * (int(CurrentFrame) + 1) + 1, 34, -56, 35));
				}
				pX1 = pX;
				pY1 = pY;
				timer.restart();
			}
			else
			{
				bool isGo = false;
				if (pX1 - x >= 10 || pY1 - y >= 10) isGo = true;
				if (timer.getElapsedTime().asSeconds() >= rWait && !isGo)
				{
					timer.restart();
					rWait = rand() % 3 + 3;
					pX1 = rand() % 200 - 100 + x;
					if (pX1 > x) direct = 2;
					else direct = 1;
					pY1 = rand() % 200 - 100 + y;
				}
				if (timer.getElapsedTime().asSeconds() >= rWait)
				{
					float distance = sqrt((pX1 - x) * (pX1 - x) + (pY1 - y) * (pY1 - y));//считаем дистанцию (длину от точки А до точки Б). формула длины вектора
					if (distance > 2) {//этим условием убираем дергание во время конечной позиции спрайта

						x += 0.075 * time * (pX1 - x) / distance;//идем по иксу с помощью вектора нормали
						y += 0.075 * time * (pY1 - y) / distance;//идем по игреку так же
					}
					if (direct == 2)
					{
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > 8) CurrentFrame -= 8;
						sprite.setTextureRect(sf::IntRect(55 * int(CurrentFrame), 34, 56, 35));
					}
					else
					{
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > 8) CurrentFrame -= 8;
						sprite.setTextureRect(sf::IntRect(55 * (int(CurrentFrame) + 1) + 1, 34, -56, 35));
					}
				}
				else
				{
					if (direct == 2)
					{
						CurrentFrame += 0.007 * time;
						if (CurrentFrame > 8) CurrentFrame -= 8;
						sprite.setTextureRect(sf::IntRect(48 * int(CurrentFrame), 0, 49, 34));
					}
					else
					{
						CurrentFrame += 0.007 * time;
						if (CurrentFrame > 8) CurrentFrame -= 8;
						sprite.setTextureRect(sf::IntRect(48 * int(CurrentFrame + 1) + 1, 0, -49, 34));
					}
				}
			}
		}
	}
};

class NPCList
{
	friend class Player;
	static list<slime> NPCs;
	static list<slime>::iterator it;
	static int NPCCount;
public:
	NPCList() { NPCCount = 0; }
	void create(float X, float Y, int HP)
	{
		NPCs.emplace_back(X, Y, HP);
		NPCCount++;
	}
	slime& operator[](int num)
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

	void moves(float x, float y, sf::IntRect rec)
	{
		for (it = NPCs.begin(); it != NPCs.end(); it++)
		{
			(*it).moves(x, y, rec);
		}
	}

	void update(Room& r1) {
		for (it = NPCs.begin(); it != NPCs.end(); ) {
			if ((*it).CurrentHP <= 0) {
				it = NPCs.erase(it); // Удаляем и обновляем итератор
				NPCCount -= 1;
			}
			else {
				(*it).update(r1);
				++it; // Переходим к следующему элементу
			}
		}
	}

	void draw(sf::RenderWindow& window)
	{
		for (it = NPCs.begin(); it != NPCs.end(); it++)
		{
			(*it).draw(window);
		}
	}
	static int getCount()
	{
		return NPCCount;
	}
};
list<slime>::iterator NPCList::it;
list<slime> NPCList::NPCs;
int NPCList::NPCCount = 1;

class Player : public Character
{

private: 
	float dodgeFrame = 0;
	int attackCount = 1;
	int isDodge = 0;
	float dodgeTime = 0;
	sf::Clock dodgeClock;
	
public:
	Player(float X, float Y) {
		MinHigh = 96; MinWidth = 55;
		File = "sprites.png";
		image.loadFromFile(File);
		image.createMaskFromColor(sf::Color(47, 95, 115));
		image.createMaskFromColor(sf::Color(1, 0, 0));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;

		attFile = "pAttack.png";
		attImage.loadFromFile(attFile);
		attImage.createMaskFromColor(sf::Color(47, 95, 115));
		attTexture.loadFromImage(attImage);
		attSprite.setTexture(attTexture);
		attSprite.setTextureRect(sf::IntRect(0, 0, 60, 100));
	}
	sf::IntRect getHit() { return sprite.getTextureRect(); }
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

	void movement(sf::Event event) {
		if (event.type == sf::Event::MouseButtonPressed)
		{

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
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
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				if (dodgeClock.getElapsedTime().asMicroseconds() > 200000)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) or
						sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) or
						sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) or
						sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) isDodge = 2;
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
				sprite.setTextureRect(sf::IntRect(107 * int(dodgeFrame), 324, 108, 124));
			}
			else {
				sprite.setTextureRect(sf::IntRect(107 * (int(dodgeFrame) + 1) + 1, 324, -108, 124));
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
					sprite.setTextureRect(sf::IntRect(149 * int(dodgeFrame), 213, 150, 112));
				}
				else {
					dir = 0;
					sprite.setTextureRect(sf::IntRect(149 * (int(dodgeFrame) + 1) + 1, 213, -150, 112));
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
						for (auto& it : NPCList::NPCs)
						{
							it.Attacked(getAttRect(), 15);
						}
						attackFrame -= e;
						if (attackCount < 6)attackCount++;
						else attackCount = 1;
						isAttack = false;

					}
					if (direct == 2) {
						sprite.setTextureRect(sf::IntRect(a * int(attackFrame), b, c, d));
					}
					else {
						sprite.setTextureRect(sf::IntRect(a * (int(attackFrame) + 1) + 1, b, -c, d));
					}
					
				}
				else {
					if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)))) {
						dir = 1; speed = 0.1;
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > 8) CurrentFrame -= 8;
						sprite.setTextureRect(sf::IntRect(117 * (int(CurrentFrame) + 1) + 1, 105, -118, 109));
						direct = 1;
					}

					if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)))) {
						dir = 0; speed = 0.1;
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > 8) CurrentFrame -= 8;
						sprite.setTextureRect(sf::IntRect(117 * int(CurrentFrame), 105, 118, 109));
						direct = 2;
					}

					if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W)))) {
						dir = 3; speed = 0.1;
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > 8) CurrentFrame -= 8;
						if (direct == 1)sprite.setTextureRect(sf::IntRect(117 * (int(CurrentFrame) + 1) + 1, 104, -118, 110));
						else sprite.setTextureRect(sf::IntRect(117 * int(CurrentFrame), 104, 118, 110));
					}

					if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))) {
						dir = 2; speed = 0.1;
						CurrentFrame += 0.005 * time;
						if (CurrentFrame > 8) CurrentFrame -= 8;
						if (direct == 1)sprite.setTextureRect(sf::IntRect(117 * (int(CurrentFrame) + 1) + 1, 104, -118, 110));
						else sprite.setTextureRect(sf::IntRect(117 * int(CurrentFrame), 104, 118, 110));
					}

					if (speed == 0)
					{
						CurrentFrame += 0.007 * time;
						if (CurrentFrame > 10) CurrentFrame -= 10;
						if (direct == 1)sprite.setTextureRect(sf::IntRect(102 * (int(CurrentFrame) + 1) + 1, 0, -103, 105));
						else sprite.setTextureRect(sf::IntRect(102 * int(CurrentFrame), 0, 103, 105));
					}
				}
			}
		}
	}
	void interactionWithMap(Room& r1) override//ф-ция взаимодействия с картой
	{
		for (int i = y / 32; i < (y + MinHigh) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 32; j < (x + MinWidth) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
			{
				if (r1.GetTile(i, j) == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
				{
					if (dy > 0)//если мы шли вниз,
					{
						y = i * 32 - MinHigh;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					}
					if (dy < 0)
					{
						y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					}
					if (dx > 0)
					{
						x = j * 32 - MinWidth;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//аналогично идем влево
					}
				}

				if (r1.GetTile(i, j) == 's') { //если символ равен 's' (камень)
					x = 300; y = 300;//какое то действие... например телепортация героя
					r1.RemoveTile(i, j);//убираем камень, типа взяли бонус. можем и не убирать, кстати.
				}
			}
	}
};
