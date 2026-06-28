#include "C_LCD1602I2C.h"

A_LCD1602::A_LCD1602(I_Arduino* arduino)
    : lcd(0x27, 16, 2), arduino(arduino) {}

void A_LCD1602::init() {
    lcd.init();
    lcd.backlight();
}

void A_LCD1602::clear() {
    lcd.clear();
}

void A_LCD1602::printAt(uint8_t col, uint8_t row, const String& text) {
    lcd.setCursor(col, row);
    lcd.print(text);
}

C_LCD1602I2C::C_LCD1602I2C()
    : arduino(nullptr), adapter(nullptr) {}

int C_LCD1602I2C::set_ItsIArduino(I_Arduino* p) {
    if (!p) return ERR_NULL_POINTER;

    arduino = p;
    adapter = new A_LCD1602(arduino);

    return ERR_OK;
}

int C_LCD1602I2C::begin() {
    if (!adapter) return ERR_INVALID_STATE;

    adapter->init();
    adapter->clear();

    return ERR_OK;
}

I_LCD1602* C_LCD1602I2C::get_ItsILCD1602() {
    return adapter;
}

C_LCD1602I2C* C_LCD1602I2C::get_ItsCLCD1602() {
    return this;
}
