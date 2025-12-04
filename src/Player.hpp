#include <string>
#include <iostream>

class Player
{
public:
    Player(std::string &name_, int &minutes_, int &seconds_) : name(name_), minutes(minutes_), seconds(seconds_) {}

    void setName(std::string &name_) { this->name = name_; }
    void setMinutes(int &minutes_) { this->minutes = minutes_; }
    void setSeconds(int &seconds_) { this->seconds = seconds_; }

    std::string getName() { return this->name; }
    int getMinutes() { return this->minutes; }
    int getSeconds() { return this->seconds; }

private:
    std::string name;
    int minutes;
    int seconds;
};