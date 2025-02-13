#pragma once
#include <SFML/Graphics.hpp>

class Button
{
	sf::RectangleShape rec;
	sf::RectangleShape recOver;
	sf::Font font;
	sf::Text text;
	sf::Text textOver;
	void (*func)();
public:
	bool isRecVisible = true;
	Button(void (*funcPtr)()) : func(funcPtr) 
	{
		rec.setFillColor(sf::Color(255, 255, 255));
	}
	Button()
	{
		func = nullptr;
		rec.setFillColor(sf::Color(255, 255, 255));
	}
	void SetFunc(void (*funcPtr)()) { func = funcPtr; }
	void SetPosition(sf::Vector2f& pos) 
	{ 
		rec.setPosition(pos);
		recOver.setPosition(pos);
		text.setPosition(pos.x + rec.getSize().x / 2 - text.getLocalBounds().width, pos.y + rec.getSize().y / 2 - text.getLocalBounds().height);
		textOver.setPosition(pos.x + rec.getSize().x / 2 - text.getLocalBounds().width, pos.y + rec.getSize().y / 2 - text.getLocalBounds().height);
	}
	void setPosition(float x, float y) { rec.setPosition(sf::Vector2f(x, y));recOver.setPosition(sf::Vector2f(x, y));}
	sf::Vector2f getPosition() { return rec.getPosition(); }
	void SetSize(float x, float y) { rec.setSize(sf::Vector2f(x, y)); recOver.setSize(sf::Vector2f(x, y));}
	void SetSize(sf::Vector2f& size){rec.setSize(size);recOver.setSize(size);}
	sf::Vector2f GetSize() { return rec.getSize(); }
	sf::Vector2f GetTextSize() { return text.getLocalBounds().getSize(); }
	void SetText(std::string str) 
	{
		if (text.getFont() == nullptr)throw std::runtime_error("Font is not set. Please set the font before setting the text.");
		text.setString(str); 
		textOver.setString(str); 
	}
	void SetTextSize(int size) { text.setCharacterSize(size); textOver.setCharacterSize(size);}
	void SetFont(std::string str) { font.loadFromFile(str); text.setFont(font);textOver.setFont(font); }
	void SetFont(sf::Font& font) { text.setFont(font); textOver.setFont(font);}
	void SetColor(sf::Color& color) { rec.setFillColor(color); }
	void SetTextColor(sf::Color& color) { text.setFillColor(color); }
	void SetOutlineColor(sf::Color& color) { rec.setOutlineColor(color); }
	void SetOutlineThickness(int thick) { rec.setOutlineThickness(thick); }
	void SetColorChange(sf::Color& color) { recOver.setFillColor(color); textOver.setFillColor(color); }
	bool Draw(sf::Event& event, sf::RenderWindow& window )
	{
		if (isRecVisible)window.draw(rec);
		if (!text.getString().isEmpty()) window.draw(text);
		if (rec.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
		{
			if (event.type == sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				
			}
			else if (isRecVisible == true)window.draw(recOver);
			else if(!text.getString().isEmpty()) window.draw(textOver);
			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if(func != nullptr)func;
					return true;
				}
			}
		}
		return false;
	}
};

