#pragma once

#include <cstdint>

class roll {
public:
    roll() = default;

    uint8_t pins() const;

    bool is_open() const;

    void pins(uint8_t value);

    bool is_all_pins() const;

private:
    uint8_t _pins {};
};
