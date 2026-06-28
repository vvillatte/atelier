#include "C_Build.h"

C_Build::C_Build()
    : its_Arduino(),
      its_dhtInternal(2),
      its_dhtExternal(4),
      its_lcd(),
      its_processor()
{}

void C_Build::begin() {
    I_Arduino* its_pArduino = its_Arduino.get_ItsIArduino();

    if (!its_pArduino) {
        Serial.println("ERR: Arduino port null");
        return;
    }

    if (its_dhtInternal.set_ItsIArduino(its_pArduino) != ERR_OK)
        Serial.println("ERR: DHT1 Arduino wiring");

    if (its_dhtExternal.set_ItsIArduino(its_pArduino) != ERR_OK)
        Serial.println("ERR: DHT2 Arduino wiring");

    if (its_lcd.set_ItsIArduino(its_pArduino) != ERR_OK)
        Serial.println("ERR: LCD Arduino wiring");

    if (its_lcd.begin() != ERR_OK) {
        Serial.println("ERR: LCD init");
        return;
    }

    its_lcd.get_ItsILCD1602()->printAt(0, 0, "Init...");
    its_lcd.get_ItsILCD1602()->printAt(0, 1, "Please wait");

    if (its_dhtInternal.begin() != ERR_OK) {
        its_lcd.get_ItsILCD1602()->clear();
        its_lcd.get_ItsILCD1602()->printAt(0, 0, "ERR: DHT1");
        return;
    }

    if (its_dhtExternal.begin() != ERR_OK) {
        its_lcd.get_ItsILCD1602()->clear();
        its_lcd.get_ItsILCD1602()->printAt(0, 0, "ERR: DHT2");
        return;
    }

    its_processor.set_ItsIInternalDHT22(its_dhtInternal.get_ItsIDHT22());
    its_processor.set_ItsIExternalDHT22(its_dhtExternal.get_ItsIDHT22());
    its_processor.set_ItsILCD(its_lcd.get_ItsILCD1602());
    its_processor.set_ItsIArduino(its_pArduino);

    if (its_processor.begin() != ERR_OK) {
        its_lcd.get_ItsILCD1602()->clear();
        its_lcd.get_ItsILCD1602()->printAt(0, 0, "ERR: Processor");
        return;
    }

    its_lcd.get_ItsILCD1602()->clear();
    its_lcd.get_ItsILCD1602()->printAt(0, 0, "System Ready");
}

C_Processor* C_Build::get_ItsCProcessor() {
    return &its_processor;
}

I_Processor* C_Build::get_ItsIProcessor() {
    return its_processor.get_ItsIProcessor();
}
