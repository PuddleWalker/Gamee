#include <SFML\Graphics.hpp>
using namespace sf;
const int HEIGHT_MAP = 25;//размер карты высота
const int WIDTH_MAP = 50;//размер карты ширина 
sf::String TileMap[HEIGHT_MAP] = {
		"00000000000000000000000000000000000000000000000000",
		"0                           0                    0",
		"0   s                       0                    0",
		"0                                                0",
		"0                                                0",
		"0                                                0",
		"0                                                0",
		"0            0                                   0",
		"0            0                                   0",
		"0            0                                   0",
		"0            0                                   0",
		"0            0              0                    0",
		"0            0              0                    0",
		"00000000000000              0                    0",
		"0                           0                    0",
		"0                           0                    0",
		"0                           0                    0",
		"0                           0                    0",
		"0                                                0",
		"0                                                0",
		"0                                                0",
		"0                                                0",
		"0                                                0",
		"0                                                0",
		"00000000000000000000000000000000000000000000000000",
};
class Room
{
	Image map_image;
	Texture map;
	Sprite s_map;
public:
	

	void draw(RenderWindow &window)
	{
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));

				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
	}
	Room()
	{
		map_image.loadFromFile("map.png");
		map_image.createMaskFromColor(Color(47, 95, 115));
		map.loadFromImage(map_image);
		s_map.setTexture(map);
	}
};
