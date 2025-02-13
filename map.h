#pragma once
#include <SFML\Graphics.hpp>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>

namespace fs = std::filesystem;
using namespace sf;
using namespace std;

class Character;

class Room
{
	vector<std::pair<int, void(*)(sf::RenderWindow&)>> entitiesToDraw;
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
		std::sort(entitiesToDraw.begin(), entitiesToDraw.end(), [](const std::pair<int, void(*)(sf::RenderWindow&)>& a, const std::pair<int, void(*)(sf::RenderWindow&)>& b) {
			return a.first < b.first;
			});
		for (int i = 0; i < HEIGHT_MAP; i++)
		{
			//Здесь код отрисовки сущностей
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));

				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
		}
	}
	char GetTile(int i, int j) { return TileMap[i][j]; }
	void RemoveTile(int i, int j) { TileMap[i][j] = ' '; }
	void AddToDraw(pair<int, void(&)(sf::RenderWindow&)> ch) { entitiesToDraw.push_back(ch); }
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
			}
			file.get();
		}
	}
	//~Room()
	//{
	//	for (int i = 0; i < HEIGHT_MAP; ++i) {
	//		delete[] TileMap[i]; // Освобождаем каждую строку
	//	}
	//	delete[] TileMap;
	//}
};
