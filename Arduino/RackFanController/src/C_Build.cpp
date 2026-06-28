#include "C_Build.h"

C_Build::C_Build()
    : arduino(),
      dhtInternal(2),
      dhtExternal(4),
      lcd(),
      processor()
{}

void C_Build::begin() {
    I_Arduino* arduinoPort = arduino.get_ItsIArduino();

    if (!arduinoPort) {
        Serial.println("ERR: Arduino port null");
        return;
    }

    if (dhtInternal.set_ItsIArduino(arduinoPort) != ERR_OK)
        Serial.println("ERR: DHT1 Arduino wiring");

    if (dhtExternal.set_ItsIArduino(arduinoPort) != ERR_OK)
        Serial.println("ERR: DHT2 Arduino wiring");

    if (lcd.set_ItsIArduino(arduinoPort) != ERR_OK)
        Serial.println("ERR: LCD Arduino wiring");

    if (lcd.begin() != ERR_OK) {
        Serial.println("ERR: LCD init");
        return;
    }

    lcd.get_ItsILCD1602()->printAt(0, 0, "Init...");
    lcd.get_ItsILCD1602()->printAt(0, 1, "Please wait");

    if (dhtInternal.begin() != ERR_OK) {
        lcd.get_ItsILCD1602()->clear();
        lcd.get_ItsILCD1602()->printAt(0, 0, "ERR: DHT1");
        return;
    }

    if (dhtExternal.begin() != ERR_OK) {
        lcd.get_ItsILCD1602()->clear();
        lcd.get_ItsILCD1602()->printAt(0, 0, "ERR: DHT2");
        return;
    }

    processor.set_ItsIInternalDHT22(dhtInternal.get_ItsIDHT22());
    processor.set_ItsIExternalDHT22(dhtExternal.get_ItsIDHT22());
    processor.set_ItsILCD(lcd.get_ItsILCD1602());

    lcd.get_ItsILCD1602()->clear();
    lcd.get_ItsILCD1602()->printAt(0, 0, "System Ready");
}

C_Processor* C_Build::get_ItsCProcessor() {
    return &processor;
}

I_Processor* C_Build::get_ItsIProcessor() {
    return &processor;
}
