#pragma once

#include <string>

class game;

class player {
public:
    explicit player(const std::string& name);

    uint16_t score() const;

    bool roll(uint8_t value);

    std::string name() const;

    void set_game(game* value);

private:
    std::string _name;
    game* _game = nullptr;
};

