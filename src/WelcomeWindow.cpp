#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <optional>
#include <cstdint>
#include <fstream>
#include <string>
#include "readConfig.hpp"

void setText(sf::Text &text, float x, float y)
{
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                    textRect.position.y + textRect.size.y / 2.0f});
    text.setPosition(sf::Vector2f(x, y));
}

sf::String bootWelcomeWindow()
{
    ConfigNames config = readConfig("files/config.cfg");
    sf::Vector2u windowDimensions((config.colCount * 32), (config.rowCount * 32 + 100));

    sf::RenderWindow startWindow(sf::VideoMode(windowDimensions), "Minesweeper", sf::Style::Close);

    sf::RectangleShape rectangle(static_cast<sf::Vector2f>(windowDimensions));
    rectangle.setFillColor(sf::Color::Blue);

    sf::Font font("files/font.ttf");

    sf::Text welcomeMessage(font);
    welcomeMessage.setString("WELCOME TO MINESWEEPER!");
    welcomeMessage.setCharacterSize(24);
    welcomeMessage.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcomeMessage, (config.colCount * 32) / 2, (config.rowCount * 32 + 100) / 2 - 150);

    sf::Text enterName(font);
    enterName.setString("Enter your name:");
    enterName.setCharacterSize(20);
    enterName.setStyle(sf::Text::Bold);
    setText(enterName, (config.colCount * 32) / 2, (config.rowCount * 32 + 100) / 2 - 75);

    sf::Text userTypedName(font);
    sf::String userInput("");
    userTypedName.setCharacterSize(24);
    userTypedName.setFillColor(sf::Color::Yellow);
    userTypedName.setStyle(sf::Text::Bold | sf::Text::Underlined);
    userTypedName.setString("|");
    setText(userTypedName, (config.colCount * 32) / 2, (config.rowCount * 32 + 100) / 2 - 45);

    while (startWindow.isOpen())
    {
        while (const std::optional event = startWindow.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                sf::String emptyString = "";
                startWindow.close();
                return emptyString;
            }
            if (const auto *textEntered = event->getIf<sf::Event::TextEntered>())
            {
                if (std::isalpha(textEntered->unicode))
                {
                    if (userInput.getSize() < 10)
                    {
                        char currChar = static_cast<char>(textEntered->unicode);
                        if (userInput.getSize() == 0)
                        {
                            currChar = std::toupper(currChar);
                        }
                        else
                        {
                            currChar = std::tolower(currChar);
                        }
                        userInput += currChar;
                    }
                }
                else if (textEntered->unicode == '\b' && userInput.getSize() != 0)
                {
                    userInput.erase(userInput.getSize() - 1);
                }
                userTypedName.setString(userInput + "|");
                setText(userTypedName, (config.colCount * 32) / 2, (config.rowCount * 32 + 100) / 2 - 45);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && userInput.getSize() > 0)
            {
                return userInput;
            }
        }

        startWindow.clear();
        startWindow.draw(rectangle);
        startWindow.draw(welcomeMessage);
        startWindow.draw(enterName);
        startWindow.draw(userTypedName);
        startWindow.display();
    }

    return userInput;
}