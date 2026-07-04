#include "C_Display.h"

/* ============================
   A_Display (Unified Adapter)
   ============================ */

A_Display::A_Display()
#if USE_OLED_DISPLAY
    : oled(OLED_WIDTH, OLED_HEIGHT, &Wire)
#else
    : lcd(LCD_ADDR, LCD_COLS, LCD_ROWS)
#endif
{}

void A_Display::init() {
#if USE_OLED_DISPLAY
    Wire.begin();
    if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(MSG_ERR_INIT);
        return;
    }
    oledReady = true;
    oled.clearDisplay();
    oled.setTextSize(OLED_TEXT_SIZE_SMALL);
    oled.setTextColor(SSD1306_WHITE);
#else
    lcd.init();
    lcd.backlight();
#endif
}

void A_Display::clear() {
#if USE_OLED_DISPLAY
    oled.clearDisplay();
#else
    lcd.clear();
#endif
}

void A_Display::printAt(uint8_t x, uint8_t y, const char* text) {
#if USE_OLED_DISPLAY
    oled.setCursor(x, y);
    oled.print(text);
#else
    lcd.setCursor(x, y);
    lcd.print(text);
#endif
}

void A_Display::printAt(uint8_t x, uint8_t y, const __FlashStringHelper* text) {
#if USE_OLED_DISPLAY
    oled.setCursor(x, y);
    oled.print(text);
#else
    lcd.setCursor(x, y);
    lcd.print(text);
#endif
}

// void A_Display::printAt(uint8_t x, uint8_t y, const String& text) {
// #if USE_OLED_DISPLAY
//     oled.setCursor(x, y);
//     oled.print(text);
// #else
//     lcd.setCursor(x, y);
//     lcd.print(text);
// #endif
// }

void A_Display::refresh() {
#if USE_OLED_DISPLAY
    oled.display();
#else
    // LCD does not require refresh
#endif
}

void A_Display::setTextSize(uint8_t size) {
#if USE_OLED_DISPLAY
    oled.setTextSize(size);
#else
    // LCD does not support text scaling
#endif
}

DisplayType A_Display::getDisplayType() const {
#if USE_OLED_DISPLAY
    return DISPLAY_OLED;
#else
    return DISPLAY_LCD;
#endif
}

/* ============================
   C_Display (Component)
   ============================ */

C_Display::C_Display()
    : itsAdapter()
{}

int C_Display::setItsArduinoInterface(I_Arduino* pI_Arduino) {
    if (!pI_Arduino) return ERR_NULL_POINTER;
    pItsI_Arduino = pI_Arduino;
    return ERR_OK;
}

int C_Display::begin() {
    if (!pItsI_Arduino) return ERR_INVALID_STATE;

    itsAdapter.init();
    itsAdapter.clear();
    itsAdapter.refresh();

    return ERR_OK;
}

I_Display* C_Display::getInterface() {
    return &itsAdapter;
}

C_Display* C_Display::getComponent() {
    return this;
}
