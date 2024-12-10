#include "Dice.h"

Dice::Dice() : gen(rd()), dis(1, 6), lastRoll1(0), lastRoll2(0), doublesCount(0) {}

std::pair<int, int> Dice::roll() {
    lastRoll1 = dis(gen);
    lastRoll2 = dis(gen);
    if (lastRoll1 == lastRoll2) {
        doublesCount++;
    } else {
        doublesCount = 0;
    }
    return {lastRoll1, lastRoll2};
}

bool Dice::isDoubles() const {
    return lastRoll1 == lastRoll2;
}

int Dice::getTotal() const {
    return lastRoll1 + lastRoll2;
}

void Dice::resetDoubles() {
    doublesCount = 0;
}

int Dice::getDoublesCount() const {
    return doublesCount;
}