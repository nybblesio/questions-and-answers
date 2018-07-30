#include <cstdint>
#include <iostream>
#include "game.h"
#include "player.h"
#include "procedural.h"

void run_oop_version() {
//    uint8_t rolls_to_make[] = {
//        4,   5,    // frame #1
//        2,   4,    // frame #2
//        1,   6,    // frame #3
//        4,   3,    // frame #4
//        1,   1,    // frame #5
//        9,   1,    // frame #6
//        10,  0,    // frame #7
//        10,  0,    // frame #8
//        10,  0,    // frame #9
//        10, 10, 10,// frame #10
//    };

    uint8_t rolls_to_make[] = {
        4,   5,    // frame #1
        2,   8,    // frame #2
        9,   1,    // frame #3
        9,   1,    // frame #4
        10,  0,    // frame #5
        6,   2,    // frame #6
        9,   1,    // frame #7
        10,  0,    // frame #8
        9,   1,    // frame #9
        9,   1, 10,// frame #10
    };

    game bowling_game {};
    player single_player("jeff");

    bowling_game.add_player(&single_player);
    bowling_game.on_frame_changed([&](frame* new_frame) {
        std::cout << "player: " << single_player.name() << " ";
        for (uint8_t f = 0; f < 10; f++ ) {
            std::cout << (int)f << ":" << bowling_game.score(f) << " ";
        }
        std::cout << "\n" << std::endl;
    });

    for (uint8_t i = 0; i < 21; i++) {
        if (!single_player.roll(rolls_to_make[i])) {
            bowling_game.move_to_next_frame();
        }
    }

    std::cout << "\n" << std::endl;

}

void run_procedural_version() {
    uint8_t rolls_to_make[] = {
        8,   0,    // frame #1
        7,   0,    // frame #2
        5,   3,    // frame #3
        9,   1,    // frame #4
        9,   1,    // frame #5
        10,  0,    // frame #6
        8,   0,    // frame #7
        5,   1,    // frame #8
        3,   7,    // frame #9
        9,   0,  0,// frame #10
    };

//    uint8_t rolls_to_make[] = {
//        4,   5,    // frame #1
//        2,   4,    // frame #2
//        1,   6,    // frame #3
//        4,   3,    // frame #4
//        1,   1,    // frame #5
//        9,   1,    // frame #6
//        10,  0,    // frame #7
//        10,  0,    // frame #8
//        10,  0,    // frame #9
//        10, 10,  10,// frame #10
//    };

//    uint8_t rolls_to_make[] = {
//        4,   5,    // frame #1
//        2,   8,    // frame #2
//        9,   1,    // frame #3
//        9,   1,    // frame #4
//        10,  0,    // frame #5
//        6,   2,    // frame #6
//        9,   1,    // frame #7
//        10,  0,    // frame #8
//        9,   1,    // frame #9
//        9,   1, 10,// frame #10
//    };

    bowling_game_t game {
        .player_name = "jeff"
    };

    for (uint8_t i = 0; i < number_of_rolls; i++) {
        game.rolls[i] = rolls_to_make[i];
        game.rolls_made = i;
    }

    score_game(game);
    std::cout << "player: " << game.player_name << " ";
    for (const auto& frame_score : game.frame_score) {
        std::cout << frame_score << " ";
    }
    std::cout << "\n" << std::endl;
}

int main() {
#if 0
    run_oop_version();
#else
    run_procedural_version();
#endif
    return 0;
}
