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
    PINCAP_DIGITAL,
    PINCAP_ANALOG,
    PINCAP_PWM,
    PINCAP_I2C,
    PINCAP_SPI,
    PINCAP_UART,
    PINCAP_SPECIAL
};

/* ============================
   I_Arduino (Interface)
   ============================ */

class I_Arduino {
public:
    virtual ~I_Arduino() = default;

    virtual int requestPin(uint8_t pin) = 0;
    virtual int releasePin(uint8_t pin) = 0;

    virtual void pinMode(uint8_t pin, uint8_t mode) = 0;
    virtual void digitalWrite(uint8_t pin, uint8_t value) = 0;
    virtual unsigned long millis() = 0;

    virtual void serialPrint(const String& s) = 0;
    virtual void serialPrintLn(const String& s) = 0;
};


/* ============================
   A_Arduino (Adapter)
   ============================ */

class A_Arduino : public I_Arduino {
public:
    A_Arduino();

    // Pin ownership API
    int requestPin(uint8_t pin) override;
    int releasePin(uint8_t pin) override;

    // Arduino API passthrough
    void pinMode(uint8_t pin, uint8_t mode) override;
    void digitalWrite(uint8_t pin, uint8_t value) override;
    unsigned long millis() override;
    
    void serialPrint(const String& s) override;
    void serialPrintLn(const String& s) override;

private:
    // Pin ownership bitmask (supports up to 64 pins)
    uint64_t pinMask = 0ULL;

    // Private helpers
    bool reservePin(uint8_t pin);
    void freePin(uint8_t pin);

    bool isValidPin(uint8_t pin) const;
    PinCapability getPinCapability(uint8_t pin) const;
};


/* ============================
   C_Arduino (Component)
   ============================ */

class C_Arduino {
public:
    C_Arduino();

    I_Arduino* get_ItsIArduino();
    C_Arduino* get_ItsCArduino();

private:
    A_Arduino adapter;
};

#endif
