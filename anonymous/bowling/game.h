#pragma once

#include <map>
#include <vector>
#include <functional>
#include "frame.h"

struct score_result_t {
    bool strike = false;
    bool spare = false;
    uint16_t score = 0;
};

class player;

using player_list_t = std::vector<player*>;
using frame_changed_callable = std::function<void (frame*)>;

class game {
public:
    frame* current_frame();

    bool move_to_next_frame();

    uint16_t score(uint8_t frame);

    void add_player(player* value);

    player* current_player() const;

    void switch_players(player* value);

    const player_list_t& players() const;

    void on_frame_changed(const frame_changed_callable& callable);

private:
    void score_for_player_frame(
        player* p,
        score_result_t& result,
        uint8_t scoring_for_frame,
        uint8_t target_frame,
        score_type_t score_type = score_type_t::normal);

    frame* frame_for_player(player* p, uint8_t number);

private:
    player_list_t _players {};
    uint8_t _current_frame = 0;
    player* _current_player = nullptr;
    std::map<player*, std::vector<frame>> _frames {};
    frame_changed_callable _frame_changed_callable {};
};

