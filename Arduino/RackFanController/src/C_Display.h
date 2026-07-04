#ifndef _C_DISPLAY_H
#define _C_DISPLAY_H

#include "C_Arduino.h"
#include "ErrorCodes.h"

#if USE_OLED_DISPLAY
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>
#else
    #include <LiquidCrystal_I2C.h>
#endif

#define MSG_ERR_INIT F("[DISPLAY] OLED init failed")

/* ============================
   DisplayType enum
   ============================ */

enum DisplayType {
    DISPLAY_LCD,
    DISPLAY_OLED
};

/* ============================
   Constants
   ============================ */

constexpr uint8_t LCD_ADDR = 0x27;
constexpr uint8_t LCD_COLS = 16;
constexpr uint8_t LCD_ROWS = 2;
constexpr uint8_t OLED_ADDR = 0x3C;
constexpr uint8_t OLED_WIDTH = 128;
constexpr uint8_t OLED_HEIGHT = 64;
constexpr uint8_t OLED_TEXT_SIZE_SMALL = 1;
constexpr uint8_t OLED_TEXT_SIZE_MEDIUM = 2;

/* ============================
   I_Display (Unified Interface)
   ============================ */

class I_Display {
public:
    virtual ~I_Display() = default;

    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void printAt(uint8_t x, uint8_t y, const char* text) = 0;
    virtual void printAt(uint8_t x, uint8_t y, const __FlashStringHelper* text) = 0;
    // virtual void printAt(uint8_t x, uint8_t y, const String& text) = 0;
    virtual void refresh() = 0;
    virtual void setTextSize(uint8_t size) = 0;
    virtual DisplayType getDisplayType() const = 0;
};

/* ============================
   A_Display (Unified Adapter)
   ============================ */

class A_Display : public I_Display {
public:
    A_Display();

    void init() override;
    void clear() override;
    void printAt(uint8_t x, uint8_t y, const char* text) override;
    void printAt(uint8_t x, uint8_t y, const __FlashStringHelper* text) override;
    // void printAt(uint8_t x, uint8_t y, const String& text) override;
    void refresh() override;
    void setTextSize(uint8_t size) override;
    DisplayType getDisplayType() const override;

#if USE_OLED_DISPLAY
private:
    Adafruit_SSD1306 oled;
    bool oledReady = false;
#else
private:
    LiquidCrystal_I2C lcd;
#endif
};

/* ============================
   C_Display (Component)
   ============================ */

class C_Display {
public:
    C_Display();

    int setItsArduinoInterface(I_Arduino* pI_Arduino);
    int begin();

    I_Display* getInterface();
    C_Display* getComponent();

private:
    I_Arduino* pItsI_Arduino = nullptr;
    A_Display itsAdapter;
};

#endif
