#ifndef _C_LCD_H
#define _C_LCD_H

#include "C_Arduino.h"
#include "ErrorCodes.h"
#include <LiquidCrystal_I2C.h>

/* ============================
   I_LCD (Interface)
   ============================ */

class I_LCD {
public:
    virtual ~I_LCD() = default;
    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void printAt(uint8_t col, uint8_t row, const String& text) = 0;
    virtual void display() = 0;
    virtual void setTextSize(uint8_t size) = 0;
};


/* ============================
   A_LCD (Adapter)
   ============================ */

class A_LCD : public I_LCD {
public:
    A_LCD(uint8_t address,
                 uint8_t cols,
                 uint8_t rows,
                 I_Arduino* its_pArduino);

    void init() override;
    void clear() override;
    void printAt(uint8_t col, uint8_t row, const String& text) override;
    void display() override;
    void setTextSize(uint8_t size) override;
private:
    LiquidCrystal_I2C lcd;
    I_Arduino* pItsI_Arduino;
};


/* ============================
   C_LCD (C_LCD)
   ============================ */

class C_LCD {
public:
    C_LCD(uint8_t address = 0x27,
                 uint8_t cols = 16,
                 uint8_t rows = 2);

    int setItsArduinoInterface(I_Arduino* pI_Arduino);
    int begin();

    I_LCD* getInterface();
    C_LCD* getComponent();

private:
    uint8_t address;
    uint8_t cols;
    uint8_t rows;

    I_Arduino* pItsI_Arduino = nullptr;
    A_LCD itsAdapter;
};

#endif
