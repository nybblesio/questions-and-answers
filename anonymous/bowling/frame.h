#pragma once

#include <vector>
#include "roll.h"

enum class score_type_t {
    normal,
    spare,
    strike
};

class frame {
public:
    frame();

    bool is_spare() const;

    bool is_strike() const;

    uint8_t number() const;

    bool is_final() const;

    uint8_t max_rolls() const;

    void number(uint8_t value);

    bool add_roll(uint8_t value);

    uint16_t score(
        uint8_t scoring_for_frame,
        score_type_t score_type) const;

private:
    uint8_t _number;
    uint8_t _current_roll = 0;
    std::vector<roll> _rolls {};
};
