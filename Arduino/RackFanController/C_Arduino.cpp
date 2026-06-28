#include "C_Arduino.h"

A_Arduino::A_Arduino() {}

void A_Arduino::pinMode(uint8_t pin, uint8_t mode) {
    ::pinMode(pin, mode);
}

void A_Arduino::digitalWrite(uint8_t pin, uint8_t value) {
    ::digitalWrite(pin, value);
}

unsigned long A_Arduino::millis() {
    return ::millis();
}

C_Arduino::C_Arduino()
    : adapter(), pinMask(0ULL) {}

bool C_Arduino::isValidPin(uint8_t pin) const {
    // AVR UNO: pins 0–19 exist (0–13 digital, 14–19 analog)
    return pin < 20;
}

PinCapability C_Arduino::getPinCapability(uint8_t pin) const {
    if (pin <= 13) {
        // PWM pins on UNO: 3, 5, 6, 9, 10, 11
        if (pin == 3 || pin == 5 || pin == 6 || pin == 9 || pin == 10 || pin == 11)
            return PIN_PWM;
        return PIN_DIGITAL;
    }

    if (pin >= 14 && pin <= 19)
        return PIN_ANALOG;

    return PIN_SPECIAL;
}

bool C_Arduino::reservePin(uint8_t pin) {
    if (!isValidPin(pin)) {
        LOG("Invalid pin requested");
        return false;
    }

    uint64_t mask = (1ULL << pin);

    if (pinMask & mask) {
        LOG("Pin already reserved");
        return false;
    }

    pinMask |= mask;

    LOG(String("Reserved pin ") + pin);
    return true;
}

void C_Arduino::freePin(uint8_t pin) {
    if (!isValidPin(pin)) {
        LOG("Invalid pin released");
        return;
    }

    uint64_t mask = (1ULL << pin);
    pinMask &= ~mask;

    LOG(String("Released pin ") + pin);
}

int C_Arduino::requestPin(uint8_t pin) {
    if (!reservePin(pin)) {
        return ERR_PIN_ALREADY_IN_USE;
    }
    return ERR_OK;
}

int C_Arduino::releasePin(uint8_t pin) {
    freePin(pin);
    return ERR_OK;
}

I_Arduino* C_Arduino::get_ItsIArduino() {
    return &adapter;
}

C_Arduino* C_Arduino::get_ItsCArduino() {
    return this;
}
