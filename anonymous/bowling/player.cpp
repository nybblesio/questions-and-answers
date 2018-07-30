#include "game.h"
#include "player.h"

player::player(const std::string& name) : _name(name){
}

uint16_t player::score() const {
    if (_game == nullptr)
        return 0;

    if (_game->current_player() != this)
        return 0;

    return _game->score(10);
}

bool player::roll(uint8_t value) {
    if (_game == nullptr)
        return false;

    if (_game->current_player() != this)
        return false;

    auto frame = _game->current_frame();
    if (frame != nullptr)
        return frame->add_roll(value);
    return false;
}

std::string player::name() const {
    return _name;
}

void player::set_game(game* value) {
    _game = value;
}
