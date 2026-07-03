#ifndef _C_DISPLAY_H
#define _C_DISPLAY_H

#include "C_Arduino.h"
#include "C_LCD1602I2C.h"
#include "C_OLED12864_SSD1306.h"
#include "ErrorCodes.h"

// Select which display to use
#define USE_OLED_DISPLAY   1   // 1 = OLED12864_SSD1306, 0 = LCD1602I2C

/* ============================
   I_Display (Unified Interface)
   ============================ */

class I_Display {
public:
    virtual ~I_Display() = default;

    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void printAt(uint8_t x, uint8_t y, const String& text) = 0;
    virtual void refresh() = 0;   // LCD1602: no-op, OLED: calls display()
};


/* ============================
   A_Display_LCD1602 (Adapter)
   ============================ */

class A_Display_LCD1602 : public I_Display {
public:
    A_Display_LCD1602(I_LCD1602* lcd) : lcd(lcd) {}

    void init() override {
        lcd->init();
        lcd->clear();
    }

    void clear() override {
        lcd->clear();
    }

    void printAt(uint8_t x, uint8_t y, const String& text) override {
        lcd->printAt(x, y, text);
    }

    void refresh() override {
        // LCD1602 does not need a refresh
    }

private:
    I_LCD1602* lcd;
};


/* ============================
   A_Display_OLED12864 (Adapter)
   ============================ */

class A_Display_OLED12864 : public I_Display {
public:
    A_Display_OLED12864(I_OLED12864_SSD1306* oled) : oled(oled) {}

    void init() override {
        oled->init();
        oled->clear();
        oled->display();
    }

    void clear() override {
        oled->clear();
    }

    void printAt(uint8_t x, uint8_t y, const String& text) override {
        oled->printAt(x, y, text);
    }

    void refresh() override {
        oled->display();
    }

private:
    I_OLED12864_SSD1306* oled;
};


/* ============================
   C_Display (Component)
   ============================ */

class C_Display {
public:
    C_Display();

    int set_ItsIArduino(I_Arduino* p);
    int begin();

    I_Display* get_ItsIDisplay();
    C_Display* get_ItsCDisplay();

private:
    I_Arduino* its_pArduino = nullptr;

#if USE_OLED_DISPLAY
    C_OLED12864_SSD1306 its_oled;
    A_Display_OLED12864* adapter = nullptr;
#else
    C_LCD1602I2C its_lcd;
    A_Display_LCD1602* adapter = nullptr;
#endif
};

#endif
