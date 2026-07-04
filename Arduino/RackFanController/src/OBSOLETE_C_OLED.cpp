#include "C_OLED.h"

/* ============================
   Adapter (Adapter)
   ============================ */

A_OLED::A_OLED(uint8_t address, I_Arduino* pI_Arduino)
    : oled(128, 64, &Wire, -1),
      pItsArduinoInterface(pI_Arduino)
{
    Wire.begin();   // I2C init
}

void A_OLED::init() {
    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        // No LCD error code exists for OLED, so use generic sensor init failure
        return;
    }

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
}

void A_OLED::clear() {
    oled.clearDisplay();
}

void A_OLED::printAt(uint8_t x, uint8_t y, const String& text) {
    oled.setCursor(x, y);
    oled.print(text);
}

void A_OLED::display() {
    oled.display();
}

void A_OLED::setTextSize(uint8_t size) {
    oled.setTextSize(size);
}

/* ============================
   C_OLED (Component)
   ============================ */

C_OLED::C_OLED(uint8_t address)
    : address(address),
      pItsArduinoInterface(nullptr),
      itsAdapter(address, nullptr)
{}

int C_OLED::set_ItsIArduino(I_Arduino* pIArduino) {
    if (!pIArduino) return ERR_NULL_POINTER;

    pItsArduinoInterface = pIArduino;
    itsAdapter = A_OLED(address, pItsArduinoInterface);

    return ERR_OK;
}

int C_OLED::begin() {
    if (!pItsArduinoInterface) return ERR_INVALID_STATE;

    itsAdapter.init();
    itsAdapter.clear();
    itsAdapter.display();

    return ERR_OK;
}

I_OLED* C_OLED::get_ItsIOLED() {
    return &itsAdapter;
}

C_OLED* C_OLED::get_ItsCOLED() {
    return this;
}
