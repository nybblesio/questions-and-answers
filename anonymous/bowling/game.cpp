
#include "game.h"
#include "player.h"

uint16_t game::score(uint8_t frame) {
    auto p = current_player();
    if (p == nullptr)
        return 0;
    uint16_t score = 0;
    for (uint8_t frame_number = 0;
            frame_number < frame + 1;
            frame_number++) {
        auto frame_ptr = frame_for_player(p, frame_number);
        if (frame_ptr == nullptr)
            break;
        score += frame_ptr->score(frame_number, score_type_t::normal);
        if (frame_ptr->is_strike()) {
            score_result_t first_score_result {};
            score_for_player_frame(
                p,
                first_score_result,
                frame_number,
                static_cast<uint8_t>(frame_number + 1),
                score_type_t::strike);
            score += first_score_result.score;
            if (first_score_result.strike) {
                score_result_t second_score_result {};
                score_for_player_frame(
                    p,
                    second_score_result,
                    frame_number,
                    static_cast<uint8_t>(frame_number + 2),
                    score_type_t::strike);
                score += second_score_result.score;
            }
        } else if (frame_ptr->is_spare()) {
            score_result_t score_result {};
            score_for_player_frame(
                p,
                score_result,
                frame_number,
                static_cast<uint8_t>(frame_number + 1),
                score_type_t::spare);
            score += score_result.score;
        }
    }
    return score;
}

bool game::move_to_next_frame() {
    auto frame = current_frame();
    if (_current_frame < 10) {
        _current_frame++;
    }
    if (_frame_changed_callable != nullptr)
        _frame_changed_callable(frame);
    return _current_frame < 10;
}

frame* game::current_frame() {
    auto p = current_player();
    if (p == nullptr)
        return nullptr;
    return frame_for_player(p, _current_frame);
}

void game::add_player(player* value) {
    if (value == nullptr)
        return;
    value->set_game(this);
    _players.push_back(value);
    auto it = _frames.insert(std::make_pair(value, std::vector<frame> {}));
    auto& frames = it.first->second;
    frames.resize(10);
    for (auto i = 0; i < 10; i++)
        frames[i].number(i);
}

player* game::current_player() const {
    if (_players.empty())
        return nullptr;
    if (_players.size() == 1)
        return _players.back();
    return _current_player;
}

void game::switch_players(player* value) {
    if (_players.size() <= 1)
        return;
    _current_player = value;
}

const player_list_t& game::players() const {
    return _players;
}

frame* game::frame_for_player(player* p, uint8_t number) {
    if (number > 9)
        return nullptr;
    const auto it = _frames.find(p);
    if (it != _frames.end()) {
        return &it->second[number];
    }
    return nullptr;
}

void game::score_for_player_frame(
        player* p,
        score_result_t& result,
        uint8_t scoring_for_frame,
        uint8_t target_frame,
        score_type_t score_type) {
    auto next_frame = frame_for_player(p, target_frame);
    if (next_frame != nullptr) {
        result.spare = next_frame->is_spare();
        result.strike = next_frame->is_strike();
        result.score = next_frame->score(scoring_for_frame, score_type);
    }
}

void game::on_frame_changed(const frame_changed_callable& callable) {
    _frame_changed_callable = callable;
}
