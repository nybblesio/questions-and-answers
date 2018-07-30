#include <iostream>
#include "frame.h"

frame::frame() {
    for (auto i = 0; i < 3; i++)
        _rolls.push_back(roll());
}

bool frame::is_final() const {
    return _number == 9;
}

bool frame::is_spare() const {
    return _rolls[0].pins() + _rolls[1].pins() == 10;
}

uint16_t frame::score(
        uint8_t scoring_for_frame,
        score_type_t score_type) const {
    auto first_roll = _rolls[0];
    auto second_roll = _rolls[1];
    auto third_roll = _rolls[2];

    if (is_final()) {
        if (scoring_for_frame < 9) {
            switch (score_type) {
                case score_type_t::normal:
                case score_type_t::spare:
                    return first_roll.pins();
                case score_type_t::strike: {
                    if (scoring_for_frame == 8) {
                        return first_roll.pins() + second_roll.pins();
                    } else {
                        return first_roll.pins();
                    }
                }
            }
        }

        if (is_strike() || is_spare()) {
            return first_roll.pins() + second_roll.pins() + third_roll.pins();
        } else {
            return first_roll.pins() + second_roll.pins();
        }
    } else {
        switch (score_type) {
            case score_type_t::spare:
                return first_roll.pins();
            case score_type_t::strike:
            case score_type_t::normal:
                return first_roll.pins() + second_roll.pins();
        }
    }
}

bool frame::is_strike() const {
    return _rolls[0].is_all_pins() && _rolls[1].is_open();
}

uint8_t frame::number() const {
    return _number;
}

uint8_t frame::max_rolls() const {
    return static_cast<uint8_t>(_number == 9 ? 3 : 2);
}

void frame::number(uint8_t value) {
    _number = value;
}

bool frame::add_roll(uint8_t value) {
    auto max = max_rolls();
    if (_current_roll < max) {
        auto& roll = _rolls[_current_roll++];
        roll.pins(value);
    }
    return _current_roll < max;
}
