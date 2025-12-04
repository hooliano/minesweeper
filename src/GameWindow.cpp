#include <iostream>
#include <optional>
#include <cstdint>
#include <vector>
#include <random>
#include "Tile.hpp"
#include <utility>
#include "LeaderboardWindow.cpp"

std::vector<sf::Vector2f> createPositions(ConfigNames &config)
{
    float colCount = static_cast<float>(config.colCount);
    float rowCount = static_cast<float>(config.rowCount);
    std::vector<sf::Vector2f> positions;
    positions.push_back({((colCount / 2 * 32) - 32), (static_cast<float>(rowCount + 0.5) * 32.0f)});
    positions.push_back({(colCount * 32 - 304), (static_cast<float>(rowCount + 0.5) * 32.0f)});
    positions.push_back({(colCount * 32 - 240), (static_cast<float>(rowCount + 0.5) * 32.0f)});
    positions.push_back({(colCount * 32 - 240), (static_cast<float>(rowCount + 0.5) * 32.0f)});
    positions.push_back({(colCount * 32 - 176), (static_cast<float>(rowCount + 0.5) * 32.0f)});

    return positions;
}

sf::Vector2i individualDigitTexturePos(int &&digit) { return sf::Vector2i(21 * digit, 0); }

std::vector<sf::Sprite> createSprites(std::vector<sf::Texture> &textures)
{
    std::vector<sf::Sprite> sprites;

    for (size_t i = 0; i < textures.size(); i++)
    {
        sf::Sprite newSprite(textures[i]);
        sprites.push_back(newSprite);
    }

    return sprites;
}

void setMines(std::vector<std::vector<Tile>> &board, ConfigNames &config)
{
    for (size_t i = 0; i < config.numMines; i++)
    {
        std::random_device random1;
        std::mt19937 gen1(random1());
        std::uniform_int_distribution<> distrib1(0, config.rowCount - 1);

        std::random_device random2;
        std::mt19937 gen2(random2());
        std::uniform_int_distribution<> distrib2(0, config.colCount - 1);

        size_t row = distrib1(gen1);
        size_t col = distrib2(gen2);

        if (board[row][col].getMineStatus())
        {
            i--;
        }

        board[row][col].insertMine();
    }
}

void setNearbyNumbers(std::vector<std::vector<Tile>> &board)
{
    for (size_t i = 0; i < board.size(); i++)
    {
        for (size_t j = 0; j < board[i].size(); j++)
        {
            size_t nearMines = 0;
            for (int k = -1; k <= 1; k++)
            {
                if (i + k >= 0 && i + k < board.size())
                {
                    for (int n = -1; n <= 1; n++)
                    {
                        if (j + n >= 0 && j + n < board[i].size())
                        {
                            if (board[i + k][j + n].getMineStatus())
                            {
                                nearMines++;
                            }
                        }
                    }
                }
            }
            board[i][j].declareNearMines(nearMines);
        }
    }
}

void setSpriteVectorPositions(std::vector<std::vector<sf::Sprite>> &sprites)
{
    for (size_t i = 0; i < sprites.size(); i++)
    {
        for (size_t j = 0; j < sprites[i].size(); j++)
        {
            sf::Vector2f position(sprites[i][j].getPosition());
            position.x = position.x + (j * 32);
            position.y = position.y + (i * 32);
            sprites[i][j].setPosition(position);
        }
    }
}

void setButtonPositions(std::vector<sf::Vector2f> &positions, std::vector<sf::Sprite> &sprites)
{
    size_t i = 2;
    for (size_t j = 0; j < positions.size() - 1; j++)
    {
        sprites[i].setPosition(positions[j]);
        j++;
    }
}

std::pair<int, int> findTileClicked(sf::Vector2f &mousePos, std::vector<std::vector<sf::Sprite>> &visualBoard)
{
    for (size_t i = 0; i < visualBoard.size(); i++)
    {
        for (size_t j = 0; j < visualBoard[i].size(); j++)
        {
            sf::Rect spriteBounds = visualBoard[i][j].getGlobalBounds();

            if (spriteBounds.contains(mousePos))
            {
                return std::make_pair(i, j);
            }
        }
    }

    return std::make_pair(-1, -1);
}

int findButtonPressed(sf::Vector2f &mousePos, std::vector<sf::Sprite> &buttonsList)
{
    for (size_t i = 0; i < buttonsList.size(); i++)
    {
        sf::FloatRect buttonBounds = buttonsList[i].getGlobalBounds();

        if (buttonBounds.contains(mousePos))
        {
            return i;
        }
    }
    return -1;
}

void setNearbyNumberForTile(std::vector<sf::Texture> &numbersList, Tile &tile, sf::Sprite &boardTile)
{
    size_t numberNearbyMines = tile.getNearbyMines();

    switch (numberNearbyMines)
    {
    case 1:
        boardTile.setTexture(numbersList[0]);
        break;
    case 2:
        boardTile.setTexture(numbersList[1]);
        break;
    case 3:
        boardTile.setTexture(numbersList[2]);
        break;
    case 4:
        boardTile.setTexture(numbersList[3]);
        break;
    case 5:
        boardTile.setTexture(numbersList[4]);
        break;
    case 6:
        boardTile.setTexture(numbersList[5]);
        break;
    case 7:
        boardTile.setTexture(numbersList[6]);
        break;
    case 8:
        boardTile.setTexture(numbersList[7]);
        break;
    default:
        boardTile.setTexture(numbersList[8]);
        break;
    }
}
void setCounterTexture(std::vector<sf::Texture> &counterDigitsList, int &digit, sf::Sprite &counterSprite)
{
    counterSprite.setTexture(counterDigitsList[digit]);
}
void revealAllNearbyNone(std::vector<std::vector<Tile>> &board, std::vector<std::vector<sf::Sprite>> &flagsOnBoard, std::vector<std::vector<sf::Sprite>> &visualBoard, std::vector<std::vector<sf::Sprite>> &mineBoard, std::vector<std::vector<sf::Sprite>> &numberNearbyBoard, std::vector<sf::Texture> &numbersList, int i, int j, sf::Texture &revealedTileText, int &numberOfHiddenTiles)
{
    for (int k = -1; k <= 1; k++)
    {
        if ((i + k) >= 0 && (i + k) < board.size())
        {
            for (int n = -1; n <= 1; n++)
            {
                if (j + n >= 0 && j + n < board[i].size())
                {
                    if (!(i + k == i && j + n == j))
                    {
                        if (!board[i + k][j + n].getMineStatus() && !board[i + k][j + n].getRevealStatus() && !board[i + k][j + n].getFlagStatus())
                        {
                            visualBoard[i + k][j + n].setTexture(revealedTileText);
                            mineBoard[i + k][j + n].setTexture(revealedTileText);
                            flagsOnBoard[i + k][j + n].setTexture(revealedTileText);
                            board[i + k][j + n].setRevealStatus(true);
                            numberOfHiddenTiles--;
                            setNearbyNumberForTile(numbersList, board[i + k][j + n], numberNearbyBoard[i + k][j + n]);
                            setNearbyNumberForTile(numbersList, board[i + k][j + n], flagsOnBoard[i + k][j + n]);
                            if (board[i + k][j + n].getNearbyMines() == 0)
                            {
                                revealAllNearbyNone(board, flagsOnBoard, visualBoard, mineBoard, numberNearbyBoard, numbersList, (i + k), (j + n), revealedTileText, numberOfHiddenTiles);
                            }
                        }
                    }
                }
            }
        }
    }
}

void revealAllMines(std::vector<std::vector<Tile>> &board, std::vector<std::vector<sf::Sprite>> &flagsOnBoard, std::vector<std::vector<sf::Sprite>> &visualBoard, std::vector<std::vector<sf::Sprite>> &mineBoard, std::vector<std::vector<sf::Sprite>> &numberNearbyBoard, sf::Texture &revealedTileText, sf::Texture &mineText, sf::Texture &flagText)
{
    for (size_t i = 0; i < board.size(); i++)
    {
        for (size_t j = 0; j < board[i].size(); j++)
        {
            if (board[i][j].getMineStatus())
            {
                mineBoard[i][j].setTexture(mineText);
                if (board[i][j].getFlagStatus())
                {
                    numberNearbyBoard[i][j].setTexture(flagText);
                    flagsOnBoard[i][j].setTexture(mineText);
                }
                else
                {
                    numberNearbyBoard[i][j].setTexture(mineText);
                    flagsOnBoard[i][j].setTexture(mineText);
                }

                board[i][j].setRevealStatus(true);
            }
        }
    }
}

void flagAllMines(std::vector<std::vector<Tile>> &board, std::vector<std::vector<sf::Sprite>> &flagsOnBoard, sf::Texture &flagText, int &numFlagsLeft)
{
    for (size_t i = 0; i < board.size(); i++)
    {
        for (size_t j = 0; j < board[i].size(); j++)
        {
            if (board[i][j].getMineStatus() && !board[i][j].getFlagStatus())
            {
                flagsOnBoard[i][j].setTexture(flagText);
                board[i][j].changeFlagStatus(true);
                numFlagsLeft--;
            }
        }
    }
}
void startGame(std::string &name)
{
    ConfigNames config = readConfig("files/config.cfg");

    sf::Texture hiddenTileText("files/images/tile_hidden.png");
    sf::Texture revealedTileText("files/images/tile_revealed.png");
    sf::Texture happyFaceButtonText("files/images/face_happy.png");
    sf::Texture debugButtonText("files/images/debug.png");
    sf::Texture pauseButtonText("files/images/pause.png");
    sf::Texture playButtonText("files/images/play.png");
    sf::Texture winFaceText("files/images/face_win.png");
    sf::Texture loseFaceText("files/images/face_lose.png");
    sf::Texture leaderBoardButtonText("files/images/leaderboard.png");
    sf::Texture counterTexture("files/images/digits.png");
    sf::Texture mineText("files/images/mine.png");
    sf::Texture flagText("files/images/flag.png");
    sf::Texture number1Text("files/images/number_1.png");
    sf::Texture number2Text("files/images/number_2.png");
    sf::Texture number3Text("files/images/number_3.png");
    sf::Texture number4Text("files/images/number_4.png");
    sf::Texture number5Text("files/images/number_5.png");
    sf::Texture number6Text("files/images/number_6.png");
    sf::Texture number7Text("files/images/number_7.png");
    sf::Texture number8Text("files/images/number_8.png");

    sf::Vector2i rectPos0 = individualDigitTexturePos(0);
    sf::Vector2i rectPos1 = individualDigitTexturePos(1);
    sf::Vector2i rectPos2 = individualDigitTexturePos(2);
    sf::Vector2i rectPos3 = individualDigitTexturePos(3);
    sf::Vector2i rectPos4 = individualDigitTexturePos(4);
    sf::Vector2i rectPos5 = individualDigitTexturePos(5);
    sf::Vector2i rectPos6 = individualDigitTexturePos(6);
    sf::Vector2i rectPos7 = individualDigitTexturePos(7);
    sf::Vector2i rectPos8 = individualDigitTexturePos(8);
    sf::Vector2i rectPos9 = individualDigitTexturePos(9);
    sf::Vector2i rectPosNeg = individualDigitTexturePos(10);

    sf::Vector2i rectSize(21, 32);

    sf::IntRect digit0(rectPos0, rectSize);
    sf::IntRect digit1(rectPos1, rectSize);
    sf::IntRect digit2(rectPos2, rectSize);
    sf::IntRect digit3(rectPos3, rectSize);
    sf::IntRect digit4(rectPos4, rectSize);
    sf::IntRect digit5(rectPos5, rectSize);
    sf::IntRect digit6(rectPos6, rectSize);
    sf::IntRect digit7(rectPos7, rectSize);
    sf::IntRect digit8(rectPos8, rectSize);
    sf::IntRect digit9(rectPos9, rectSize);
    sf::IntRect digitNeg(rectPosNeg, rectSize);

    sf::Texture counterDigit0("files/images/digits.png", false, digit0);
    sf::Texture counterDigit1("files/images/digits.png", false, digit1);
    sf::Texture counterDigit2("files/images/digits.png", false, digit2);
    sf::Texture counterDigit3("files/images/digits.png", false, digit3);
    sf::Texture counterDigit4("files/images/digits.png", false, digit4);
    sf::Texture counterDigit5("files/images/digits.png", false, digit5);
    sf::Texture counterDigit6("files/images/digits.png", false, digit6);
    sf::Texture counterDigit7("files/images/digits.png", false, digit7);
    sf::Texture counterDigit8("files/images/digits.png", false, digit8);
    sf::Texture counterDigit9("files/images/digits.png", false, digit9);
    sf::Texture counterDigitNeg("files/images/digits.png", false, digitNeg);

    sf::Sprite hiddenTile(hiddenTileText);
    sf::Sprite revealedTile(revealedTileText);
    sf::Sprite happyFaceButton(happyFaceButtonText);
    sf::Sprite debugButton(debugButtonText);
    sf::Sprite pauseButton(pauseButtonText);
    sf::Sprite leaderBoardButton(leaderBoardButtonText);
    sf::Sprite counter(counterTexture);
    sf::Sprite number1(number1Text);
    sf::Sprite number2(number2Text);
    sf::Sprite number3(number3Text);
    sf::Sprite number4(number4Text);
    sf::Sprite number5(number5Text);
    sf::Sprite number6(number6Text);
    sf::Sprite number7(number7Text);
    sf::Sprite number8(number8Text);
    sf::Sprite minutesTen(counterDigit0);
    sf::Sprite minutesOne(counterDigit0);
    sf::Sprite secondsTen(counterDigit0);
    sf::Sprite secondsOne(counterDigit0);
    sf::Sprite flagsHundred(counterDigit0);
    sf::Sprite flagsTen(counterDigit0);
    sf::Sprite flagsOne(counterDigit0);

    float colCount = static_cast<float>(config.colCount);
    float rowCount = static_cast<float>(config.rowCount);
    int numberOfHiddenTiles = colCount * rowCount;

    bool debugOnOrOff = false;
    bool gamePaused = false;
    bool gameOver = false;
    bool gameOverRegistered = false;
    bool leaderboardOn = false;
    int numFlagsLeft = config.numMines;
    size_t playerIndex = -1;

    happyFaceButton.setPosition({((colCount / 2 * 32) - 32), (static_cast<float>(rowCount + 0.5) * 32.0f)});
    debugButton.setPosition({(colCount * 32 - 304), (static_cast<float>(rowCount + 0.5) * 32.0f)});
    pauseButton.setPosition({(colCount * 32 - 240), (static_cast<float>(rowCount + 0.5) * 32.0f)});
    leaderBoardButton.setPosition({(colCount * 32 - 176), (static_cast<float>(rowCount + 0.5) * 32.0f)});
    minutesTen.setPosition({(colCount * 32) - 97, (static_cast<float>(rowCount + 0.5) * 32.0f + 16)});
    minutesOne.setPosition({(colCount * 32) - 76, (static_cast<float>(rowCount + 0.5) * 32.0f + 16)});
    secondsTen.setPosition({(colCount * 32) - 54, (static_cast<float>(rowCount + 0.5) * 32.0f + 16)});
    secondsOne.setPosition({(colCount * 32) - 33, (static_cast<float>(rowCount + 0.5) * 32.0f + 16)});
    flagsHundred.setPosition({33, 32 * static_cast<float>(rowCount + 0.5) + 16});
    flagsTen.setPosition({54, 32 * static_cast<float>(rowCount + 0.5) + 16});
    flagsOne.setPosition({75, 32 * static_cast<float>(rowCount + 0.5) + 16});

    std::vector<std::vector<sf::Sprite>> visualBoard(config.rowCount, std::vector<sf::Sprite>(config.colCount, hiddenTile));
    setSpriteVectorPositions(visualBoard);

    std::vector<std::vector<Tile>> board(config.rowCount, std::vector<Tile>(config.colCount));
    setMines(board, config);
    setNearbyNumbers(board);

    std::vector<std::vector<sf::Sprite>> mineBoard(config.rowCount, std::vector<sf::Sprite>(config.colCount, hiddenTile));
    setSpriteVectorPositions(mineBoard);

    std::vector<std::vector<sf::Sprite>> numberNearbyBoard(config.rowCount, std::vector<sf::Sprite>(config.colCount, hiddenTile));
    setSpriteVectorPositions(numberNearbyBoard);

    std::vector<std::vector<sf::Sprite>> flagsOnBoard(config.rowCount, std::vector<sf::Sprite>(config.colCount, hiddenTile));
    setSpriteVectorPositions(flagsOnBoard);

    std::vector<std::vector<sf::Sprite>> pauseScreen(config.rowCount, std::vector<sf::Sprite>(config.colCount, revealedTile));
    setSpriteVectorPositions(pauseScreen);

    std::vector<sf::Texture> numbersList = {number1Text, number2Text, number3Text, number4Text, number5Text, number6Text, number7Text, number8Text, revealedTileText};

    std::vector<sf::Sprite> buttonsList = {happyFaceButton, debugButton, pauseButton, leaderBoardButton};

    std::vector<sf::Texture> counterDigitsList = {counterDigit0, counterDigit1, counterDigit2, counterDigit3, counterDigit4, counterDigit5, counterDigit6, counterDigit7, counterDigit8, counterDigit9, counterDigitNeg};

    sf::Vector2u windowDimensions((config.colCount * 32), (config.rowCount * 32 + 100));

    sf::RenderWindow gameWindow(sf::VideoMode(windowDimensions), "Minesweeper", sf::Style::Close);

    sf::RectangleShape background(static_cast<sf::Vector2f>(windowDimensions));
    background.setFillColor(sf::Color::White);

    sf::Clock clock;
    clock.start();

    while (gameWindow.isOpen())
    {
        if (!leaderboardOn)
        {
            gamePaused = false;
        }
        int numMinutes = 0;
        sf::Time elapsed = clock.getElapsedTime();
        float elapsedTime = elapsed.asSeconds();
        while (elapsedTime >= 60)
        {
            elapsedTime -= 60;
            numMinutes++;
        }
        if (numberOfHiddenTiles == config.numMines && !gameOverRegistered)
        {
            clock.stop();
            gameOver = true;
            happyFaceButton.setTexture(winFaceText);
            flagAllMines(board, flagsOnBoard, flagText, numFlagsLeft);
            int seconds = static_cast<int>(elapsedTime);
            int minesLeft = 0;
            setCounterTexture(counterDigitsList, minesLeft, flagsHundred);
            setCounterTexture(counterDigitsList, minesLeft, flagsTen);
            setCounterTexture(counterDigitsList, minesLeft, flagsOne);
            std::vector<Player> players = fetchLeaderBoardPlayers();
            Player newPlayer(name, numMinutes, seconds);
            playerIndex = updatePlayersVector(players, newPlayer);
            updateLeaderBoard(players);
            renderLeaderBoard(gameWindow, pauseScreen, 0, playerIndex);
            gameOverRegistered = true;
        }

        int firstSecondDigit = static_cast<int>(elapsedTime / 10);
        int secondSecondDigit = static_cast<int>(elapsedTime) % 10;
        int firstMinuteDigit = (numMinutes / 10);
        int secondMinuteDigit = numMinutes % 10;

        setCounterTexture(counterDigitsList, firstSecondDigit, secondsTen);
        setCounterTexture(counterDigitsList, secondSecondDigit, secondsOne);
        setCounterTexture(counterDigitsList, firstMinuteDigit, minutesTen);
        setCounterTexture(counterDigitsList, secondMinuteDigit, minutesOne);

        if (numFlagsLeft >= 0)
        {
            int flagsLeftFirstDigit = numFlagsLeft / 100;
            int flagsLeftSecondDigit = (numFlagsLeft - (flagsLeftFirstDigit * 100)) / 10;
            int flagsLeftThirdDigit = (numFlagsLeft - (flagsLeftFirstDigit * 100)) % 10;

            setCounterTexture(counterDigitsList, flagsLeftFirstDigit, flagsHundred);
            setCounterTexture(counterDigitsList, flagsLeftSecondDigit, flagsTen);
            setCounterTexture(counterDigitsList, flagsLeftThirdDigit, flagsOne);
        }
        else
        {
            flagsHundred.setTexture(counterDigitNeg);
            int flagsLeftFirstDigit = (numFlagsLeft / 10) * -1;
            int flagsLeftSecondDigit = (numFlagsLeft % 10) * -1;

            setCounterTexture(counterDigitsList, flagsLeftFirstDigit, flagsTen);
            setCounterTexture(counterDigitsList, flagsLeftSecondDigit, flagsOne);
        }

        while (const std::optional event = gameWindow.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                gameWindow.close();
            }

            if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i iMousePos = mouseButtonPressed->position;
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(iMousePos);

                    std::pair<int, int> boardPos = findTileClicked(mousePos, visualBoard);

                    if (boardPos.first >= 0)
                    {
                        if (!board[boardPos.first][boardPos.second].getRevealStatus() && !board[boardPos.first][boardPos.second].getFlagStatus() && !gamePaused && !gameOver)
                        {
                            if (board[boardPos.first][boardPos.second].getMineStatus())
                            {
                                visualBoard[boardPos.first][boardPos.second].setTexture(revealedTileText);
                                mineBoard[boardPos.first][boardPos.second].setTexture(mineText);
                                numberNearbyBoard[boardPos.first][boardPos.second].setTexture(flagText);
                                flagsOnBoard[boardPos.first][boardPos.second].setTexture(mineText);
                                board[boardPos.first][boardPos.second].setRevealStatus(true);
                                clock.stop();
                                happyFaceButton.setTexture(loseFaceText);
                                gameOver = true;
                                revealAllMines(board, flagsOnBoard, visualBoard, mineBoard, numberNearbyBoard, revealedTileText, mineText, flagText);
                            }
                            else
                            {
                                visualBoard[boardPos.first][boardPos.second].setTexture(revealedTileText);
                                mineBoard[boardPos.first][boardPos.second].setTexture(revealedTileText);
                                setNearbyNumberForTile(numbersList, board[boardPos.first][boardPos.second], flagsOnBoard[boardPos.first][boardPos.second]);
                                setNearbyNumberForTile(numbersList, board[boardPos.first][boardPos.second], numberNearbyBoard[boardPos.first][boardPos.second]);
                                board[boardPos.first][boardPos.second].setRevealStatus(true);
                                if (board[boardPos.first][boardPos.second].getNearbyMines() == 0)
                                {
                                    revealAllNearbyNone(board, flagsOnBoard, visualBoard, mineBoard, numberNearbyBoard, numbersList, boardPos.first, boardPos.second, revealedTileText, numberOfHiddenTiles);
                                }
                                numberOfHiddenTiles--;
                            }
                        }
                    }
                    else
                    {
                        size_t button = findButtonPressed(mousePos, buttonsList);
                        switch (button)
                        {
                        case 0:
                            gameWindow.close();
                            startGame(name);
                            break;
                        case 1:
                            if (!debugOnOrOff && !gamePaused && !gameOver)
                            {
                                for (size_t i = 0; i < visualBoard.size(); i++)
                                {
                                    for (size_t j = 0; j < visualBoard[i].size(); j++)
                                    {
                                        if (board[i][j].getMineStatus())
                                        {
                                            visualBoard[i][j].setTexture(revealedTileText);
                                            mineBoard[i][j].setTexture(mineText);
                                            numberNearbyBoard[i][j].setTexture(mineText);
                                            flagsOnBoard[i][j].setTexture(mineText);
                                            board[i][j].changeFlagStatus(false);
                                        }
                                        else
                                        {
                                            visualBoard[i][j].setTexture(revealedTileText);
                                            mineBoard[i][j].setTexture(revealedTileText);
                                            setNearbyNumberForTile(numbersList, board[i][j], flagsOnBoard[i][j]);
                                            setNearbyNumberForTile(numbersList, board[i][j], numberNearbyBoard[i][j]);
                                            board[i][j].changeFlagStatus(false);
                                        }
                                    }
                                }
                                numFlagsLeft = config.numMines;
                                debugOnOrOff = true;
                            }
                            else if (debugOnOrOff && !gamePaused && !gameOver)
                            {
                                for (size_t i = 0; i < visualBoard.size(); i++)
                                {
                                    for (size_t j = 0; j < visualBoard[i].size(); j++)
                                    {
                                        visualBoard[i][j].setTexture(hiddenTileText);
                                        mineBoard[i][j].setTexture(hiddenTileText);
                                        numberNearbyBoard[i][j].setTexture(hiddenTileText);
                                        flagsOnBoard[i][j].setTexture(hiddenTileText);
                                        board[i][j].setRevealStatus(false);
                                    }
                                }
                                debugOnOrOff = false;
                            }
                            break;
                        case 2:
                            if (!gamePaused && !gameOver)
                            {
                                clock.stop();
                                pauseButton.setTexture(playButtonText);
                                gamePaused = true;
                            }
                            else if (gamePaused && !gameOver)
                            {
                                clock.start();
                                pauseButton.setTexture(pauseButtonText);
                                gamePaused = false;
                            }
                            break;
                        case 3:
                            if (!leaderboardOn)
                            {
                                gamePaused = true;
                                clock.stop();
                                renderLeaderBoard(gameWindow, pauseScreen, 1, playerIndex);
                                clock.start();
                            }
                            break;
                        }
                    }
                }
                else if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    sf::Vector2i iMousePos = mouseButtonPressed->position;
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(iMousePos);

                    std::pair<int, int> boardPos = findTileClicked(mousePos, visualBoard);

                    if (boardPos.first >= 0)
                    {
                        if (!board[boardPos.first][boardPos.second].getRevealStatus() && !gamePaused && !gameOver)
                        {
                            if (board[boardPos.first][boardPos.second].getFlagStatus())
                            {
                                flagsOnBoard[boardPos.first][boardPos.second].setTexture(hiddenTileText);
                                board[boardPos.first][boardPos.second].changeFlagStatus(false);
                                numFlagsLeft++;
                            }
                            else
                            {
                                flagsOnBoard[boardPos.first][boardPos.second].setTexture(flagText);
                                board[boardPos.first][boardPos.second].changeFlagStatus(true);
                                numFlagsLeft--;
                            }
                        }
                    }
                }
            }

            gameWindow.clear();
            gameWindow.draw(background);

            if (gamePaused)
            {
                for (std::vector<sf::Sprite> a : pauseScreen)
                {
                    for (sf::Sprite b : a)
                    {
                        gameWindow.draw(b);
                    }
                }
            }
            else
            {
                for (std::vector<sf::Sprite> a : visualBoard)
                {
                    for (sf::Sprite b : a)
                    {
                        gameWindow.draw(b);
                    }
                }
                for (std::vector<sf::Sprite> a : mineBoard)
                {
                    for (sf::Sprite b : a)
                    {
                        gameWindow.draw(b);
                    }
                }
                for (std::vector<sf::Sprite> a : numberNearbyBoard)
                {
                    for (sf::Sprite b : a)
                    {
                        gameWindow.draw(b);
                    }
                }
                for (std::vector<sf::Sprite> a : flagsOnBoard)
                {
                    for (sf::Sprite b : a)
                    {
                        gameWindow.draw(b);
                    }
                }
            }

            gameWindow.draw(happyFaceButton);
            gameWindow.draw(debugButton);
            gameWindow.draw(pauseButton);
            gameWindow.draw(leaderBoardButton);
            gameWindow.draw(minutesTen);
            gameWindow.draw(minutesOne);
            gameWindow.draw(secondsTen);
            gameWindow.draw(secondsOne);
            gameWindow.draw(flagsHundred);
            gameWindow.draw(flagsTen);
            gameWindow.draw(flagsOne);
            gameWindow.display();
        }
    }
}