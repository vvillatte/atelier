#ifndef _C_ARDUINO_H
#define _C_ARDUINO_H

#include <Arduino.h>
#include "ErrorCodes.h"

// Optional logging macro (disabled by default)
#define ARDUINO_LOGGING 0

#if ARDUINO_LOGGING
    #define LOG(x) Serial.println(x)
#else
    #define LOG(x)
#endif

// Pin capability categories
enum PinCapability {
    PIN_DIGITAL,
    PIN_ANALOG,
    PIN_PWM,
    PIN_I2C,
    PIN_SPI,
    PIN_UART,
    PIN_SPECIAL
};

class I_Arduino {
public:
    virtual ~I_Arduino() = default;

    virtual void pinMode(uint8_t pin, uint8_t mode) = 0;
    virtual void digitalWrite(uint8_t pin, uint8_t value) = 0;
    virtual unsigned long millis() = 0;
};

class A_Arduino : public I_Arduino {
public:
    A_Arduino();
    void pinMode(uint8_t pin, uint8_t mode) override;
    void digitalWrite(uint8_t pin, uint8_t value) override;
    unsigned long millis() override;
};

class C_Arduino {
public:
    C_Arduino();

    int requestPin(uint8_t pin);
    int releasePin(uint8_t pin);

    I_Arduino* get_ItsIArduino();
    C_Arduino* get_ItsCArduino();

private:
    A_Arduino adapter;

    // Bitmask for pin ownership (supports up to 64 pins)
    uint64_t pinMask;

    // Private helpers
    bool reservePin(uint8_t pin);
    void freePin(uint8_t pin);

    // Capability validation
    bool isValidPin(uint8_t pin) const;
    PinCapability getPinCapability(uint8_t pin) const;
};

#endif
