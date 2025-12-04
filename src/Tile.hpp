#include <cstdint>

class Tile
{
public:
    Tile() : hasMine(false), revealed(false), nearbyMines(0), hasFlag(false) {}

    void insertMine() { this->hasMine = true; }
    void setRevealStatus(bool &&status) { this->revealed = status; }
    void declareNearMines(size_t numMines) { this->nearbyMines = numMines; }
    void changeFlagStatus(bool &&status) { this->hasFlag = status; }

    bool getMineStatus() { return this->hasMine; }
    bool getRevealStatus() { return this->revealed; }
    bool getFlagStatus() { return this->hasFlag; }
    size_t getNearbyMines() { return this->nearbyMines; }

private:
    bool hasMine;
    bool revealed;
    size_t nearbyMines;
    bool hasFlag;
};