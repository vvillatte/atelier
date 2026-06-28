#include "C_LCD1602I2C.h"

/* ============================
   A_LCD1602 (Adapter)
   ============================ */

A_LCD1602::A_LCD1602(uint8_t address,
                     uint8_t cols,
                     uint8_t rows,
                     I_Arduino* its_pArduino)
    : lcd(address, cols, rows),
      its_pArduino(its_pArduino) {}

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


/* ============================
   C_LCD1602I2C (Component)
   ============================ */

C_LCD1602I2C::C_LCD1602I2C(uint8_t address,
                           uint8_t cols,
                           uint8_t rows)
    : address(address),
      cols(cols),
      rows(rows),
      its_pArduino(nullptr),
      adapter(nullptr) {}

int C_LCD1602I2C::set_ItsIArduino(I_Arduino* p) {
    if (!p) return ERR_NULL_POINTER;

    its_pArduino = p;

    // LCD does not require pin reservation (I2C uses fixed pins)
    adapter = new A_LCD1602(address, cols, rows, its_pArduino);

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
