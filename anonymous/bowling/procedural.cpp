#include "procedural.h"

bool is_frame_a_strike(const bowling_game_t& game, size_t roll) {
    if (game.frame == number_of_frames) {
        return game.rolls[roll] == 10;
    } else {
        return game.rolls[roll - 1] == 10 && game.rolls[roll] == 0;
    }
}

void score_game(bowling_game_t& game) {
    size_t roll = 0;
    uint16_t frame_score = 0;
    uint16_t rolling_frame_score = 0;
    game.score = 0;
    game.frame = 0;

    auto update_scoring = [&]() {
        rolling_frame_score += frame_score;
        game.score += rolling_frame_score;
        game.frame_score[game.frame++] = rolling_frame_score;
    };

    while (roll < game.rolls_made) {
        uint8_t roll_score = game.rolls[roll];
        frame_score += roll_score;
        if (game.frame == number_of_frames - 1) {
            if (frame_score == number_strike_pins) {
                frame_score += game.rolls[roll];
                frame_score += game.rolls[roll + 1];
            } else {
                frame_score += game.rolls[roll + 1];
            }
            update_scoring();
            break;
        } else if ((roll + 1) % 2 == 0) {
            if (frame_score == number_strike_pins) {
                if (is_frame_a_strike(game, roll)) {
                    frame_score += game.rolls[roll + 1];
                    frame_score += game.rolls[roll + 3];
                } else
                    frame_score += game.rolls[roll + 1];
            }
            update_scoring();
            frame_score = 0;
        }

        ++roll;
    }
}
