#include "C_LCD.h"

/* ============================
   A_LCD (Adapter)
   ============================ */

A_LCD::A_LCD(uint8_t address,
                     uint8_t cols,
                     uint8_t rows,
                     I_Arduino* pItsI_Arduino)
    : lcd(address, cols, rows),
      pItsI_Arduino(pItsI_Arduino) {}

void A_LCD::init() {
    lcd.init();
    lcd.backlight();
}

void A_LCD::clear() {
    lcd.clear();
}

void A_LCD::printAt(uint8_t col, uint8_t row, const String& text) {
    lcd.setCursor(col, row);
    lcd.print(text);
}

void A_LCD::display() {
    return;
}

void A_LCD::setTextSize(uint8_t size) {
    return;
}

/* ============================
   C_LCD (Component)
   ============================ */

C_LCD::C_LCD(uint8_t address,
                           uint8_t cols,
                           uint8_t rows)
    : address(address),
      cols(cols),
      rows(rows),
      pItsI_Arduino(nullptr),
      itsAdapter(address, cols, rows, nullptr) {}

int C_LCD::setItsArduinoInterface(I_Arduino* pI_Arduino) {
    if (!pI_Arduino) return ERR_NULL_POINTER;

    pItsI_Arduino = pI_Arduino;

    // LCD does not require pin reservation (I2C uses fixed pins)
    itsAdapter = A_LCD(address, cols, rows, pItsI_Arduino);

    return ERR_OK;
}

int C_LCD::begin() {
    if (!pItsI_Arduino) return ERR_INVALID_STATE;

    itsAdapter.init();
    itsAdapter.clear();

    return ERR_OK;
}

I_LCD* C_LCD::getInterface() {
    return &itsAdapter;
}

C_LCD* C_LCD::getComponent() {
    return this;
}
