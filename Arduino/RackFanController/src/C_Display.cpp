#include "C_Display.h"

C_Display::C_Display()
#if USE_OLED_DISPLAY
    : its_oled(0x3C)
#else
    : its_lcd()
#endif
{}


int C_Display::set_ItsIArduino(I_Arduino* p) {
    if (!p) return ERR_NULL_POINTER;

    its_pArduino = p;

#if USE_OLED_DISPLAY
    if (its_oled.set_ItsIArduino(p) != ERR_OK)
        return ERR_INVALID_STATE;

    adapter = new A_Display_OLED12864(its_oled.get_ItsIOLED12864());
#else
    if (its_lcd.set_ItsIArduino(p) != ERR_OK)
        return ERR_INVALID_STATE;

    adapter = new A_Display_LCD1602(its_lcd.get_ItsILCD1602());
#endif

    return ERR_OK;
}


int C_Display::begin() {
    if (!adapter) return ERR_INVALID_STATE;

#if USE_OLED_DISPLAY
    if (its_oled.begin() != ERR_OK)
        return ERR_LCD_INIT_FAILED;
#else
    if (its_lcd.begin() != ERR_OK)
        return ERR_LCD_INIT_FAILED;
#endif

    adapter->init();
    return ERR_OK;
}


I_Display* C_Display::get_ItsIDisplay() {
    return adapter;
}

C_Display* C_Display::get_ItsCDisplay() {
    return this;
}
