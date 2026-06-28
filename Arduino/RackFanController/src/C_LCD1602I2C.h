#ifndef _C_LCD1602I2C_H
#define _C_LCD1602I2C_H

#include "C_Arduino.h"
#include "ErrorCodes.h"
#include <LiquidCrystal_I2C.h>

/* ============================
   I_LCD1602 (Interface)
   ============================ */

class I_LCD1602 {
public:
    virtual ~I_LCD1602() = default;
    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void printAt(uint8_t col, uint8_t row, const String& text) = 0;
};


/* ============================
   A_LCD1602 (Adapter)
   ============================ */

class A_LCD1602 : public I_LCD1602 {
public:
    A_LCD1602(uint8_t address,
              uint8_t cols,
              uint8_t rows,
              I_Arduino* its_pArduino);

    void init() override;
    void clear() override;
    void printAt(uint8_t col, uint8_t row, const String& text) override;

private:
    LiquidCrystal_I2C lcd;
    I_Arduino* its_pArduino;
};


/* ============================
   C_LCD1602I2C (Component)
   ============================ */

class C_LCD1602I2C {
public:
    C_LCD1602I2C(uint8_t address = 0x27,
                 uint8_t cols = 16,
                 uint8_t rows = 2);

    int set_ItsIArduino(I_Arduino* p);
    int begin();

    I_LCD1602* get_ItsILCD1602();
    C_LCD1602I2C* get_ItsCLCD1602();

private:
    uint8_t address;
    uint8_t cols;
    uint8_t rows;

    I_Arduino* its_pArduino = nullptr;
    A_LCD1602* adapter = nullptr;
};

#endif
