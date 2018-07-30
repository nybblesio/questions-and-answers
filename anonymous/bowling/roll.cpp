#include "roll.h"

uint8_t roll::pins() const {
    return _pins;
}

bool roll::is_open() const {
    return _pins == 0;
}

bool roll::is_all_pins() const {
    return _pins == 10;
}

void roll::pins(uint8_t value) {
    _pins = value;
}
