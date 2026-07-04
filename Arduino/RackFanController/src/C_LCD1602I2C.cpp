#include "C_LCD1602I2C.h"

/* ============================
   A_LCD1602 (Adapter)
   ============================ */

A_LCD1602I2C::A_LCD1602I2C(uint8_t address,
                     uint8_t cols,
                     uint8_t rows,
                     I_Arduino* its_pArduino)
    : lcd(address, cols, rows),
      its_pArduino(its_pArduino) {}

void A_LCD1602I2C::init() {
    lcd.init();
    lcd.backlight();
}

void A_LCD1602I2C::clear() {
    lcd.clear();
}

void A_LCD1602I2C::printAt(uint8_t col, uint8_t row, const String& text) {
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
      p_ItsIArduino(nullptr),
      its_ALCD1602I2C(address, cols, rows, nullptr) {}

int C_LCD1602I2C::set_ItsIArduino(I_Arduino* p) {
    if (!p) return ERR_NULL_POINTER;

    p_ItsIArduino = p;

    // LCD does not require pin reservation (I2C uses fixed pins)
    its_ALCD1602I2C = A_LCD1602I2C(address, cols, rows, p_ItsIArduino);

    return ERR_OK;
}

int C_LCD1602I2C::begin() {
    if (!p_ItsIArduino) return ERR_INVALID_STATE;

    its_ALCD1602I2C.init();
    its_ALCD1602I2C.clear();

    return ERR_OK;
}

I_LCD1602I2C* C_LCD1602I2C::get_ItsILCD1602() {
    return &its_ALCD1602I2C;
}

C_LCD1602I2C* C_LCD1602I2C::get_ItsCLCD1602() {
    return this;
}
