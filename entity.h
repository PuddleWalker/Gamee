#pragma once
#include <list>

using namespace std;
class NPCList;
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
	String attFile;
	Image image;
	Image attImage;
	Texture texture;
	Texture attTexture;
	Sprite sprite;
	Sprite attSprite;
	float CurrentFrame = 0;
	float attackFrame = 0;
	bool isAttack = 0;
	int TotalHP = 100;
	int CurrentHP = 100;
	int MinHigh;
	int MinWidth;

	float x, y;
	float dx = 0, dy = 0, speed = 0;
	int dir = 0;
	int direct = 2;

public:
	FloatRect getRect()
	{
		return FloatRect(x, y, MinWidth, MinHigh);
	}
	FloatRect getAttRect()
	{
		if (direct == 2) return FloatRect(x + MinWidth, y + MinHigh - attSprite.getTextureRect().getSize().y, attSprite.getTextureRect().getSize().x, attSprite.getTextureRect().getSize().y);
		else return FloatRect(x - 2 * MinWidth, y + MinHigh - attSprite.getTextureRect().getSize().y, attSprite.getTextureRect().getSize().x, attSprite.getTextureRect().getSize().y);

	}
	const void draw(RenderWindow& window) 
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
		if(direct == 2)attSprite.setPosition(x + MinWidth, y+MinHigh - attSprite.getTextureRect().getSize().y);
		else attSprite.setPosition(x - 2 * MinWidth, y + MinHigh - attSprite.getTextureRect().getSize().y);
		interactionWithMap();//�������� �������, ���������� �� �������������� � ������
	}

	void interactionWithMap()//�-��� �������������� � ������
	{
		for (int i = y / 32; i < (y + MinHigh) / 32; i++)//���������� �� ��������, �������������� � �������,, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
			for (int j = x / 32; j < (x + MinWidth) / 32; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
			{
				if (TileMap[i][j] == '0')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
				{
					if (dy > 0)//���� �� ��� ����,
					{
						y = i * 32 - MinHigh;//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
					}
					if (dy < 0)
					{
						y = i * 32 + 32;//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
					}
					if (dx > 0)
					{
						x = j * 32 - MinWidth;//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
					}
					if (dx < 0)
					{
						x = j * 32 + 32;//���������� ���� �����
					}
				}

				if (TileMap[i][j] == 's') { //���� ������ ����� 's' (������)
					x = 300; y = 300;//����� �� ��������... �������� ������������ �����
					TileMap[i][j] = ' ';//������� ������, ���� ����� �����. ����� � �� �������, ������.
				}
			}
	}
};

class slime : public Character
{
	friend class Player;
	friend class NPCList;
public:
	slime(float X, float Y, int HP) {
		TotalHP = HP; CurrentHP = HP; MinHigh = 34; MinWidth = 49;
		File = "slime.png";
		image.loadFromFile(File);
		image.createMaskFromColor(Color(47, 95, 115));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;

		attFile = "pAttack.png";
		attImage.loadFromFile(attFile);
		attImage.createMaskFromColor(Color(47, 95, 115));
		attTexture.loadFromImage(attImage);
		attSprite.setTexture(attTexture);
		attSprite.setTextureRect(IntRect(0, 0, 100, 100));
	}
private:
	void moves()
	{
		CurrentFrame += 0.007 * time;
		if (CurrentFrame > 8) CurrentFrame -= 8;
		sprite.setTextureRect(IntRect(48 * int(CurrentFrame), 0, 49, 34));
	}
};

class NPCList
{
	friend class Player;
	static list<slime> NPCs;
	static list<slime>::iterator it;
public:
	void create(float X, float Y, int HP)
	{
		NPCs.emplace_back(X, Y, HP);
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

	void moves()
	{
		for (it = NPCs.begin(); it != NPCs.end(); it++)
		{
			(*it).moves();
		}
	}

	void update() {
		for (it = NPCs.begin(); it != NPCs.end(); ) {
			if ((*it).CurrentHP <= 0) {
				it = NPCs.erase(it); // ������� � ��������� ��������
			}
			else {
				(*it).update();
				++it; // ��������� � ���������� ��������
			}
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
list<slime>::iterator NPCList::it;
list<slime> NPCList::NPCs;

class Player : public Character
{

private: 
	float dodgeFrame = 0;
	int attackCount = 1;
	int isDodge = 0;
	float dodgeTime;
	Clock dodgeClock;
	
public:
	Player(float X, float Y) {
		MinHigh = 96; MinWidth = 55;
		File = "sprites.png";
		image.loadFromFile(File);
		image.createMaskFromColor(Color(47, 95, 115));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;

		attFile = "pAttack.png";
		attImage.loadFromFile(attFile);
		attImage.createMaskFromColor(Color(47, 95, 115));
		attTexture.loadFromImage(attImage);
		attSprite.setTexture(attTexture);
		attSprite.setTextureRect(IntRect(0, 0, 100, 100));
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
						for (auto& it : NPCList::NPCs)
						{
							if (getAttRect().intersects(it.getRect()))
							{
								it.CurrentHP -= 15;
								cout << getAttRect().getSize().x << "\t" << getAttRect().getSize().x << "\t";
								cout << getAttRect().getPosition().x << "\t" << getAttRect().getPosition().y << "\t";
								cout << x << "\t" << y << endl;
								cout << getRect().getSize().x << "\t" << getRect().getSize().x << "\t";
								cout << getRect().getPosition().x << "\t" << getRect().getPosition().y << endl;
							}
						}
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
