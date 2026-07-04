#ifndef _C_OLED12864SSD1306_H
#define _C_OLED12864SSD1306_H

#include "C_Arduino.h"
#include "ErrorCodes.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* ============================
   I_OLED12864SSD1306 (Interface)
   ============================ */

class I_OLED12864SSD1306 {
public:
    virtual ~I_OLED12864SSD1306() = default;

    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void printAt(uint8_t x, uint8_t y, const String& text) = 0;
    virtual void display() = 0;
    virtual void setTextSize(uint8_t size) = 0;
};


/* ============================
   A_OLED12864SSD1306 (Adapter)
   ============================ */

class A_OLED12864SSD1306 : public I_OLED12864SSD1306 {
public:
    A_OLED12864SSD1306(uint8_t address,
                        I_Arduino* its_pArduino);

    void init() override;
    void clear() override;
    void printAt(uint8_t x, uint8_t y, const String& text) override;
    void display() override;
    void setTextSize(uint8_t size) override;

private:
    Adafruit_SSD1306 oled;
    I_Arduino* p_ItsIArduino;
};


/* ============================
   C_OLED12864SSD1306 (Component)
   ============================ */

class C_OLED12864SSD1306 {
public:
    C_OLED12864SSD1306(uint8_t address = 0x3C);

    int set_ItsIArduino(I_Arduino* pIArduino);
    int begin();

    I_OLED12864SSD1306* get_ItsIOLED12864SSD1306();
    C_OLED12864SSD1306* get_ItsCOLED12864SSD1306();

private:
    uint8_t address;
    I_Arduino* p_ItsIArduino = nullptr;
    A_OLED12864SSD1306 its_AOLED12864SSD1306;
};

#endif
