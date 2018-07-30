#include "procedural.h"

static bool is_frame_a_spare(
        const bowling_game_t& game,
        size_t roll) {
    return game.rolls[roll] + game.rolls[roll + 1] == 10
        && game.rolls[roll + 1] != 0;
}

static bool is_frame_a_strike(
        const bowling_game_t& game,
        size_t roll) {
    if (roll >= 18 && game.rolls[roll] == 10) {
        return true;
    }
    return game.rolls[roll] == 10 && game.rolls[roll + 1] == 0;
}

uint16_t score_strikes_and_spares(
        const bowling_game_t& game,
        size_t roll) {
    uint16_t score = 0;

    auto roll_count = 2;
    size_t next_roll = roll;

    if (game.frame == 9) {
        next_roll = 20;
        roll_count = 1;
    }

    if (is_frame_a_spare(game, next_roll)) {
        roll_count = 1;
    }

    while (next_roll < number_of_rolls && roll_count > 0) {
        if (is_frame_a_strike(game, next_roll)) {
            score += 10;
            next_roll += game.frame == 9 ? 1 : 2;
            roll_count--;
        } else {
            score += game.rolls[next_roll++];
            roll_count--;
            if (roll_count > 0) {
                score += game.rolls[next_roll++];
                roll_count--;
            }
        }
    }

    return score;
}

void score_game(bowling_game_t& game) {
    size_t roll = 0;
    uint16_t frame_score = 0;
    game.score = 0;
    game.frame = 0;

    while (roll < game.rolls_made) {
        uint8_t roll_score = game.rolls[roll];
        frame_score += roll_score;
        if ((roll + 1) % 2 == 0) {
            auto extra_score = 0;
            if (is_frame_a_strike(game, roll - 1)) {
                extra_score = score_strikes_and_spares(game, roll - 1);
            } else if (is_frame_a_spare(game, roll - 1)) {
                extra_score = score_strikes_and_spares(game, roll - 1);
            }
            frame_score += extra_score;
            game.score += frame_score;
            game.frame_score[game.frame++] = game.score;
            frame_score = 0;
        }
        ++roll;
    }
}
