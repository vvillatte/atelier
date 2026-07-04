#ifndef _C_OLED_H
#define _C_OLED_H

#include "C_Arduino.h"
#include "ErrorCodes.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* ============================
   I_OLED (Interface)
   ============================ */

class I_OLED {
public:
    virtual ~I_OLED() = default;

    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void printAt(uint8_t x, uint8_t y, const String& text) = 0;
    virtual void display() = 0;
    virtual void setTextSize(uint8_t size) = 0;
};


/* ============================
   A_OLED (Adapter)
   ============================ */

class A_OLED : public I_OLED {
public:
    A_OLED(uint8_t address,
                      I_Arduino* its_pArduino);

    void init() override;
    void clear() override;
    void printAt(uint8_t x, uint8_t y, const String& text) override;
    void display() override;
    void setTextSize(uint8_t size) override;

private:
    Adafruit_SSD1306 oled;
    I_Arduino* pItsArduinoInterface = nullptr;
};


/* ============================
   C_OLED (Component)
   ============================ */

class C_OLED {
public:
    C_OLED(uint8_t address = 0x3C);

    int set_ItsIArduino(I_Arduino* pIArduino);
    int begin();

    I_OLED* get_ItsIOLED();
    C_OLED* get_ItsCOLED();

private:
    uint8_t address;
    I_Arduino* pItsArduinoInterface = nullptr;
    A_OLED itsAdapter;
};

#endif
