#include "C_OLED12864_SSD1306.h"

/* ============================
   A_OLED12864_SSD1306 (Adapter)
   ============================ */

A_OLED12864_SSD1306::A_OLED12864_SSD1306(uint8_t address,
                                         I_Arduino* its_pArduino)
    : oled(128, 64, &Wire, -1),
      its_pArduino(its_pArduino)
{
    Wire.begin();   // I2C init
}

void A_OLED12864_SSD1306::init() {
    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        // No LCD error code exists for OLED, so use generic sensor init failure
        return;
    }

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
}

void A_OLED12864_SSD1306::clear() {
    oled.clearDisplay();
}

void A_OLED12864_SSD1306::printAt(uint8_t x, uint8_t y, const String& text) {
    oled.setCursor(x, y);
    oled.print(text);
}

void A_OLED12864_SSD1306::display() {
    oled.display();
}


/* ============================
   C_OLED12864_SSD1306 (Component)
   ============================ */

C_OLED12864_SSD1306::C_OLED12864_SSD1306(uint8_t address)
    : address(address),
      its_pArduino(nullptr),
      adapter(nullptr)
{}

int C_OLED12864_SSD1306::set_ItsIArduino(I_Arduino* p) {
    if (!p) return ERR_NULL_POINTER;

    its_pArduino = p;

    adapter = new A_OLED12864_SSD1306(address, its_pArduino);

    return ERR_OK;
}

int C_OLED12864_SSD1306::begin() {
    if (!adapter) return ERR_INVALID_STATE;

    adapter->init();
    adapter->clear();
    adapter->display();

    return ERR_OK;
}

I_OLED12864_SSD1306* C_OLED12864_SSD1306::get_ItsIOLED12864() {
    return adapter;
}

C_OLED12864_SSD1306* C_OLED12864_SSD1306::get_ItsCOLED12864() {
    return this;
}
