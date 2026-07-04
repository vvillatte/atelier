#include "C_Display.h"

C_Display::C_Display()
#if USE_OLED_DISPLAY
    : itsCOLED12864SSD1306(0x3C),
      itsAdapter(nullptr)
#else
    : itsCLCD1602I2C(),
      itsAdapter(nullptr)
#endif
{}


int C_Display::set_ItsIArduino(I_Arduino* pIArduino) {
    if (!pIArduino) return ERR_NULL_POINTER;

    p_ItsIArduino = pIArduino;

#if USE_OLED_DISPLAY
    if (itsCOLED12864SSD1306.set_ItsIArduino(pIArduino) != ERR_OK)
        return ERR_INVALID_STATE;

    itsAdapter =
        A_Display_OLED12864SSD1306(itsCOLED12864SSD1306.get_ItsIOLED12864SSD1306());
#else
    if (itsCLCD1602I2C.set_ItsIArduino(pIArduino) != ERR_OK)
        return ERR_INVALID_STATE;

    itsAdapter =
        A_Display_LCD1602I2C(itsCLCD1602I2C.get_ItsILCD1602());
#endif

    return ERR_OK;
}


int C_Display::begin() {
    if (!p_ItsIArduino) return ERR_INVALID_STATE;

#if USE_OLED_DISPLAY
    if (itsCOLED12864SSD1306.begin() != ERR_OK)
        return ERR_LCD_INIT_FAILED;

    itsAdapter.init();
#else
    if (itsCLCD1602I2C.begin() != ERR_OK)
        return ERR_LCD_INIT_FAILED;

    itsAdapter.init();
#endif

return ERR_OK;
}


I_Display* C_Display::get_ItsIDisplay() {
    return &itsAdapter;
}

C_Display* C_Display::get_ItsCDisplay() {
    return this;
}
