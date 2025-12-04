#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include "GameWindow.cpp"
#include <iostream>

int main()
{
    sf::String userNameSF = bootWelcomeWindow();
    std::string userName = userNameSF.toAnsiString();

    if (userName.size() > 0)
    {
        startGame(userName);
    }
}