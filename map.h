#pragma once
#include <SFML\Graphics.hpp>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>5

namespace fs = std::filesystem;
using namespace sf;
using namespace std;

class Character;

class Room
{
	vector<std::pair<int, std::function<void(sf::RenderWindow&)>>> entitiesToDraw;
	Image map_image;
	Texture map;
	Sprite s_map;
	const std::string folderPath = "Rooms/";
	std::string path;

	char** TileMap;
	int HEIGHT_MAP;
	int WIDTH_MAP;
public:
	void draw(RenderWindow& window)
	{
		std::sort(entitiesToDraw.begin(), entitiesToDraw.end(), [](const std::pair<int, std::function<void(sf::RenderWindow&)>>& a, const std::pair<int, std::function<void(sf::RenderWindow&)>>& b) {
			return a.first > b.first; // Сортировка по убыванию приоритета
			});
		for (int i = 0; i < HEIGHT_MAP; i++)
		{
			
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				s_map.setTextureRect(IntRect(96, 0, 32, 32));
				if (TileMap[i][j] == '+')s_map.setTextureRect(IntRect(96, 32, 32, 32));
				if (TileMap[i][j] == '*')s_map.setTextureRect(IntRect(96, 64, 32, 32));
				if (TileMap[i][j] == '=')s_map.setTextureRect(IntRect(96, 96, 32, 32));
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if ((TileMap[i][j] == '0'))
				{
					s_map.setTextureRect(IntRect(192, 0, 32, 48));
					s_map.setPosition(j * 32, i * 32 - 16);
				}
				else s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
			while (1)
			{
				if (!entitiesToDraw.empty()) {
					if (entitiesToDraw[entitiesToDraw.size() - 1].first == i)
					{
						entitiesToDraw[entitiesToDraw.size() - 1].second(window);
						entitiesToDraw.pop_back();
					}
					else break;
				}
				else break;
			}
		}
	}
	char GetTile(int i, int j) { return TileMap[i][j]; }
	void RemoveTile(int i, int j) { TileMap[i][j] = ' '; }
	void AddToDraw(std::pair<int, std::function<void(sf::RenderWindow&)>> ch) { entitiesToDraw.push_back(ch); }
	Room()
	{
		map_image.loadFromFile("map.png");
		map_image.createMaskFromColor(Color(47, 95, 115));
		map.loadFromImage(map_image);
		s_map.setTexture(map);
		entitiesToDraw.reserve(20);
		// Считываем все файлы в директории
		std::vector<std::string> files;
		for (const auto& entry : fs::directory_iterator(folderPath))
		{
			files.push_back(entry.path().string());
		}
		// Выбираем случайный файл
		int randomIndex = std::rand() % files.size();
		path = files[randomIndex];
		std::ifstream file(path);
		file >> HEIGHT_MAP >> WIDTH_MAP;
		file.get();
		TileMap = new char* [HEIGHT_MAP];
		for (int i = 0; i < HEIGHT_MAP; ++i) {
			TileMap[i] = new char[WIDTH_MAP];
			for (int j = 0; j < WIDTH_MAP; ++j) {

				file.get(TileMap[i][j]);
				if (TileMap[i][j] == ' ')
				{
					if (rand() % 15 == 3)
					{
						switch (rand() % 3)
						{
						case 0:
							TileMap[i][j] = '+';
							break;
						case 1:
							TileMap[i][j] = '*';
							break;
						case 2:
							TileMap[i][j] = '=';
							break;
						}
					}
				}
			}
			file.get();
		}
	}
	~Room()
	{
		for (int i = 0; i < HEIGHT_MAP; ++i) {
			delete[] TileMap[i]; // Освобождаем каждую строку
		}
		delete[] TileMap;
	}
};
