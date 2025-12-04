#include <fstream>
#include <vector>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "constants.hpp"

// #define BORK_IT

#ifndef BORK_IT
#include "WelcomeWindow.cpp"
#endif

std::vector<Player> fetchLeaderBoardPlayers()
{
    std::fstream leaderBoardStream;
    leaderBoardStream.open("files/leaderboard.txt", std::ios::in | std::ios::out);

    std::vector<Player> players;

    std::string score;

    while (std::getline(leaderBoardStream, score))
    {
        int minutes = std::stoi(score.substr(0, 2));
        int seconds = std::stoi(score.substr(3, 2));
        std::string name = score.substr(7);
        Player player(name, minutes, seconds);
        players.push_back(player);
    }

    return players;
}

std::string textToWriteLeaderboard(std::vector<Player> &players, size_t &playerIndex)
{
    std::string ranks;
    for (size_t i = 0; i < players.size(); i++)
    {
        std::string playerMinutes;
        std::string playerSeconds;

        if (players[i].getMinutes() < 10)
        {
            playerMinutes = "0" + std::to_string(players[i].getMinutes());
        }
        else
        {
            playerMinutes = std::to_string(players[i].getMinutes());
        }
        if (players[i].getSeconds() < 10)
        {
            playerSeconds = "0" + std::to_string(players[i].getSeconds());
        }
        else
        {
            playerSeconds = std::to_string(players[i].getSeconds());
        }
        std::string playerScore;
        if (i == playerIndex)
        {
            std::string playerScore = std::to_string(i + 1) + ".\t" + playerMinutes + ":" + playerSeconds + "\t" + players[i].getName() + "*\n\n";
        }
        else
        {
            std::string playerScore = std::to_string(i + 1) + ".\t" + playerMinutes + ":" + playerSeconds + "\t" + players[i].getName() + "\n\n";
        }

        ranks.append(playerScore);
    }
    return ranks;
}

size_t getPlayerIndex(std::vector<Player> &players, Player &player)
{
    bool compByMinutes = false;
    bool compBySeconds = false;
    for (size_t i = 0; i < players.size(); i++)
    {
        if (!compByMinutes)
        {
            if (player.getMinutes() <= players[i].getMinutes())
            {
                compByMinutes = true;
            }
        }
        if (compByMinutes && !compBySeconds)
        {
            if (player.getSeconds() <= players[i].getSeconds() && player.getMinutes() == players[i].getMinutes())
            {
                compBySeconds = true;
                return i;
            }
            else if (player.getMinutes() != players[i].getMinutes())
            {
                return i;
            }
        }
    }
    return players.size();
}

size_t updatePlayersVector(std::vector<Player> &players, Player &player)
{
    size_t playerIndex = getPlayerIndex(players, player);

    if (playerIndex == players.size() || players.size() == 0)
    {
        players.push_back(player);
    }
    else
    {
        players.push_back(players[players.size() - 1]);

        for (size_t i = players.size() - 2; i > playerIndex; i--)
        {
            players[i] = players[i - 1];
        }

        players[playerIndex] = player;
    }

    if (players.size() > 5)
    {
        std::vector<Player> newPlayers;

        for (size_t i = 0; i < 5; i++)
        {
            newPlayers.push_back(players[i]);
        }

        players = newPlayers;
    }
    if (playerIndex < 5)
    {
        return playerIndex;
    }
    return 0;
}

void updateLeaderBoard(std::vector<Player> &players)
{
    std::ofstream updater("files/leaderboard.txt", std::ios::trunc);
    for (size_t i = 0; i < players.size(); i++)
    {
        std::string playerMinutes;
        std::string playerSeconds;

        if (players[i].getMinutes() < 10)
        {
            playerMinutes = "0" + std::to_string(players[i].getMinutes());
        }
        else
        {
            playerMinutes = std::to_string(players[i].getMinutes());
        }
        if (players[i].getSeconds() < 10)
        {
            playerSeconds = "0" + std::to_string(players[i].getSeconds());
        }
        else
        {
            playerSeconds = std::to_string(players[i].getSeconds());
        }
        std::string playerScore = playerMinutes + ":" + playerSeconds + ", " + players[i].getName();
        updater << playerScore << std::endl;
    }
}

#ifndef BORK_IT
void renderLeaderBoard(sf::RenderWindow &gameWindow, std::vector<std::vector<sf::Sprite>> &pauseScreen, bool pause, size_t &playerIndex)
{
    ConfigNames config = readConfig("files/config.cfg");
    sf::Vector2u windowDimensions((config.colCount * 16), (config.rowCount * 16 + 50));

    sf::RenderWindow leaderboardWindow(sf::VideoMode(windowDimensions), "Minesweeper", sf::Style::Close);

    sf::Font font(FONT_FILE);

    std::vector<Player> players = fetchLeaderBoardPlayers();

    sf::RectangleShape background(static_cast<sf::Vector2f>(windowDimensions));
    background.setFillColor(sf::Color::Blue);

    std::string ranks;

    for (size_t i = 0; i < players.size(); i++)
    {

        std::string playerMinutes;
        std::string playerSeconds;

        if (players[i].getMinutes() < 10)
        {
            playerMinutes = "0" + std::to_string(players[i].getMinutes());
        }
        else
        {
            playerMinutes = std::to_string(players[i].getMinutes());
        }
        if (players[i].getSeconds() < 10)
        {
            playerSeconds = "0" + std::to_string(players[i].getSeconds());
        }
        else
        {
            playerSeconds = std::to_string(players[i].getSeconds());
        }
        std::string playerScore;
        if (i == playerIndex)
        {
            playerScore = std::to_string(i + 1) + ".\t" + playerMinutes + ":" + playerSeconds + "\t" + players[i].getName() + "*\n\n";
        }
        else
        {
            playerScore = std::to_string(i + 1) + ".\t" + playerMinutes + ":" + playerSeconds + "\t" + players[i].getName() + "\n\n";
        }

        ranks.append(playerScore);
    }
    std::string header = "LEADERBOARD";

    sf::Text leaderboardMessage(font);
    leaderboardMessage.setString(header);
    leaderboardMessage.setCharacterSize(20);
    leaderboardMessage.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(leaderboardMessage, (config.colCount * 16) / 2, (config.rowCount * 16 + 50) / 2 - 120);

    sf::Text displayLeaders(font);
    displayLeaders.setString(ranks);
    displayLeaders.setCharacterSize(18);
    displayLeaders.setStyle(sf::Text::Bold);
    setText(displayLeaders, (windowDimensions.x / 2), (windowDimensions.y / 2) + 20);

    while (leaderboardWindow.isOpen())
    {
        while (const std::optional event = leaderboardWindow.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                leaderboardWindow.close();
            }
        }

        leaderboardWindow.clear();
        leaderboardWindow.draw(background);
        leaderboardWindow.draw(leaderboardMessage);
        leaderboardWindow.draw(displayLeaders);
        leaderboardWindow.display();
        if (pause)
        {
            for (std::vector<sf::Sprite> a : pauseScreen)
            {
                for (sf::Sprite b : a)
                {
                    gameWindow.draw(b);
                }
            }
            gameWindow.display();
        }
    }
}

#endif

#ifdef BORK_IT
int main()
{
    std::vector<Player> players = fetchLeaderBoardPlayers();
    std::string leaderInfo = textToWriteLeaderboard(players);
    std::cout << leaderInfo;
}
#endif