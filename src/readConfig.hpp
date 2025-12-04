#include <cstdint>
#include <fstream>
#include <stdexcept>

struct ConfigNames
{
    size_t colCount;
    size_t rowCount;
    size_t numMines;
};

ConfigNames readConfig(std::string filePath)
{
    ConfigNames config;
    std::ifstream fileReader(filePath);
    if (fileReader)
    {
        std::string coluCount, rowCounter, numbMines;

        std::getline(fileReader, coluCount);
        std::getline(fileReader, rowCounter);
        std::getline(fileReader, numbMines);

        config.colCount = std::stoi(coluCount);
        config.rowCount = std::stoi(rowCounter);
        config.numMines = std::stoi(numbMines);

        return config;
    }
    else
    {
        throw std::runtime_error("File not found.");
    }
}