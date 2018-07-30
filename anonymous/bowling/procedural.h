#pragma once

#include <cstdint>
#include <string>

static constexpr uint8_t number_strike_pins = 10;
static constexpr uint8_t number_of_frames = 10;
static constexpr uint8_t number_of_rolls = 21;

struct bowling_game_t {
    std::string player_name {};
    uint8_t rolls[number_of_rolls];
    uint8_t rolls_made = 0;
    uint16_t score = 0;
    uint8_t frame = 0;
    uint16_t frame_score[number_of_frames];
};

void score_game(bowling_game_t& game);