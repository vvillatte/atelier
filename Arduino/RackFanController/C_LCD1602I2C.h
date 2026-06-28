#ifndef _C_LCD1602I2C_H
#define _C_LCD1602I2C_H

#include "C_Arduino.h"
#include "ErrorCodes.h"
#include <LiquidCrystal_I2C.h>

class I_LCD1602 {
public:
    virtual ~I_LCD1602() = default;
    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void printAt(uint8_t col, uint8_t row, const String& text) = 0;
};

class A_LCD1602 : public I_LCD1602 {
public:
    A_LCD1602(I_Arduino* arduino);
    void init() override;
    void clear() override;
    void printAt(uint8_t col, uint8_t row, const String& text) override;

private:
    LiquidCrystal_I2C lcd;
    I_Arduino* arduino;
};

class C_LCD1602I2C {
public:
    C_LCD1602I2C();

    int set_ItsIArduino(I_Arduino* p);
    int begin();

    I_LCD1602* get_ItsILCD1602();
    C_LCD1602I2C* get_ItsCLCD1602();

private:
    I_Arduino* arduino = nullptr;
    A_LCD1602* adapter = nullptr;
};

#endif
