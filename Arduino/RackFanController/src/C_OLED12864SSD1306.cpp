#include "C_OLED12864SSD1306.h"

/* ============================
   A_OLED12864SSD1306 (Adapter)
   ============================ */

A_OLED12864SSD1306::A_OLED12864SSD1306(uint8_t address,
                                         I_Arduino* its_pArduino)
    : oled(128, 64, &Wire, -1),
      p_ItsIArduino(its_pArduino)
{
    Wire.begin();   // I2C init
}

void A_OLED12864SSD1306::init() {
    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        // No LCD error code exists for OLED, so use generic sensor init failure
        return;
    }

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
}

void A_OLED12864SSD1306::clear() {
    oled.clearDisplay();
}

void A_OLED12864SSD1306::printAt(uint8_t x, uint8_t y, const String& text) {
    oled.setCursor(x, y);
    oled.print(text);
}

void A_OLED12864SSD1306::display() {
    oled.display();
}

void A_OLED12864SSD1306::setTextSize(uint8_t size) {
    oled.setTextSize(size);
}

/* ============================
   C_OLED12864SSD1306 (Component)
   ============================ */

C_OLED12864SSD1306::C_OLED12864SSD1306(uint8_t address)
    : address(address),
      p_ItsIArduino(nullptr),
      its_AOLED12864SSD1306(address, nullptr)
{}

int C_OLED12864SSD1306::set_ItsIArduino(I_Arduino* pIArduino) {
    if (!pIArduino) return ERR_NULL_POINTER;

    p_ItsIArduino = pIArduino;
    its_AOLED12864SSD1306 = A_OLED12864SSD1306(address, p_ItsIArduino);

    return ERR_OK;
}

int C_OLED12864SSD1306::begin() {
    if (!p_ItsIArduino) return ERR_INVALID_STATE;

    its_AOLED12864SSD1306.init();
    its_AOLED12864SSD1306.clear();
    its_AOLED12864SSD1306.display();

    return ERR_OK;
}

I_OLED12864SSD1306* C_OLED12864SSD1306::get_ItsIOLED12864SSD1306() {
    return &its_AOLED12864SSD1306;
}

C_OLED12864SSD1306* C_OLED12864SSD1306::get_ItsCOLED12864SSD1306() {
    return this;
}
