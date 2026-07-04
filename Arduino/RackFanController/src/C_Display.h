#ifndef _C_DISPLAY_H
#define _C_DISPLAY_H

// Select which display to use
#define USE_OLED_DISPLAY   0   // 1 = OLED12864SSD1306, 0 = LCD1602I2C

#include "C_Arduino.h"
#if USE_OLED_DISPLAY
#include "C_OLED12864SSD1306.h"
#else
#include "C_LCD1602I2C.h"
#endif
#include "ErrorCodes.h"

/* ============================
   DisplayType enum
   ============================ */

enum DisplayType {
    DISPLAY_LCD1602I2C,
    DISPLAY_OLED12864SSD1306
};

/* ============================
   I_Display (Unified Interface)
   ============================ */

class I_Display {
public:
    virtual ~I_Display() = default;

    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void printAt(uint8_t x, uint8_t y, const String& text) = 0;
    virtual void refresh() = 0;
    virtual void setTextSize(uint8_t size) = 0;
    virtual DisplayType getDisplayType() const = 0;
};


#if USE_OLED_DISPLAY
/* ============================
   A_OLED12864SSD1306 (Adapter)
   ============================ */
class A_Display_OLED12864SSD1306 : public I_Display {
public:
    A_Display_OLED12864SSD1306(I_OLED12864SSD1306* oled) : p_ItsIOLED12864SSD1306(oled) {}

    void init() override {
        p_ItsIOLED12864SSD1306->init();
        p_ItsIOLED12864SSD1306->clear();
        p_ItsIOLED12864SSD1306->display();
    }

    void clear() override {
        p_ItsIOLED12864SSD1306->clear();
    }

    void printAt(uint8_t x, uint8_t y, const String& text) override {
        p_ItsIOLED12864SSD1306->printAt(x, y, text);
    }

    void refresh() override {
        p_ItsIOLED12864SSD1306->display();
    }

    void setTextSize(uint8_t size) override {
        p_ItsIOLED12864SSD1306->setTextSize(size);
    }

    DisplayType getDisplayType() const override {
        return DISPLAY_OLED12864SSD1306;
    }

private:
    I_OLED12864SSD1306* p_ItsIOLED12864SSD1306;
};
#else
/* ============================
   A_Display_LCD1602 (Adapter)
   ============================ */

class A_Display_LCD1602I2C : public I_Display {
public:
    A_Display_LCD1602I2C(I_LCD1602I2C* pILCD1602I2C) : p_ItsILCD1602I2C(pILCD1602I2C) {}

    void init() override {
        p_ItsILCD1602I2C->init();
        p_ItsILCD1602I2C->clear();
    }

    void clear() override {
        p_ItsILCD1602I2C->clear();
    }

    void printAt(uint8_t x, uint8_t y, const String& text) override {
        p_ItsILCD1602I2C->printAt(x, y, text);
    }

    void refresh() override {
        // LCD1602 does not need a refresh - NOOP
    }

    void setTextSize(uint8_t size) override {
        // LCD1602 does not support text scaling — NOOP
    }

    DisplayType getDisplayType() const override {
        return DISPLAY_LCD1602I2C;
    }

private:
    I_LCD1602I2C* p_ItsILCD1602I2C;
};
#endif


/* ============================
   C_Display (Component)
   ============================ */

class C_Display {
public:
    C_Display();

    int set_ItsIArduino(I_Arduino* pIArduino);
    int begin();

    I_Display* get_ItsIDisplay();
    C_Display* get_ItsCDisplay();

private:
    I_Arduino* p_ItsIArduino = nullptr;

#if USE_OLED_DISPLAY
    C_OLED12864SSD1306 itsCOLED12864SSD1306;
    A_Display_OLED12864SSD1306 itsAdapter;
#else
    C_LCD1602I2C itsCLCD1602I2C;
    A_Display_LCD1602I2C itsAdapter;
#endif
};

#endif
