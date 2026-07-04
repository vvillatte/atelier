#include "C_Arduino.h"

/* ============================
   A_Arduino (Adapter)
   ============================ */

A_Arduino::A_Arduino() {
}

/* ---- Pin Ownership ---- */

bool A_Arduino::isValidPin(uint8_t pin) const {
    // AVR UNO: pins 0–19 exist (0–13 digital, 14–19 analog)
    return pin < 20;
}

PinCapability A_Arduino::getPinCapability(uint8_t pin) const {
    if (pin <= 13) {
        // PWM pins on UNO: 3, 5, 6, 9, 10, 11
        if (pin == 3 || pin == 5 || pin == 6 ||
            pin == 9 || pin == 10 || pin == 11)
        {
            return PINCAP_PWM;
        }
        return PINCAP_DIGITAL;
    }

    if (pin >= 14 && pin <= 19)
        return PINCAP_ANALOG;

    return PINCAP_SPECIAL;
}

bool A_Arduino::reservePin(uint8_t pin) {
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

void A_Arduino::freePin(uint8_t pin) {
    if (!isValidPin(pin)) {
        LOG("Invalid pin released");
        return;
    }

    uint64_t mask = (1ULL << pin);
    pinMask &= ~mask;

    LOG(String("Released pin ") + pin);
}

int A_Arduino::requestPin(uint8_t pin) {
    return reservePin(pin) ? ERR_OK : ERR_PIN_ALREADY_IN_USE;
}

int A_Arduino::releasePin(uint8_t pin) {
    freePin(pin);
    return ERR_OK;
}

/* ---- Arduino API passthrough ---- */

void A_Arduino::pinMode(uint8_t pin, uint8_t mode) {
    ::pinMode(pin, mode);
}

void A_Arduino::digitalWrite(uint8_t pin, uint8_t value) {
    ::digitalWrite(pin, value);
}

unsigned long A_Arduino::millis() {
    return ::millis();
}

/* ---- Serial passthrough ---- */

void A_Arduino::serialPrint(const String& s) {
    Serial.print(s);
}

void A_Arduino::serialPrintLn(const String& s) {
    Serial.println(s);
}

/* ============================
   C_Arduino (Component)
   ============================ */

C_Arduino::C_Arduino()
    : adapter() {}

I_Arduino* C_Arduino::get_ItsIArduino() {
    return &adapter;
}

C_Arduino* C_Arduino::get_ItsCArduino() {
    return this;
}
