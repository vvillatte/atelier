#include "C_Build.h"

C_Build::C_Build()
    : its_Arduino(),
      its_dhtInternal(2),
      its_dhtExternal(4),
      its_display(),
      its_processor()
{}

void C_Build::begin() {
    I_Arduino* its_pArduino = its_Arduino.get_ItsIArduino();

    if (!its_pArduino) {
        Serial.println("ERR: Arduino port null");
        return;
    }

    // --- Wire sensors ---
    if (its_dhtInternal.set_ItsIArduino(its_pArduino) != ERR_OK)
        Serial.println("ERR: DHT1 Arduino wiring");

    if (its_dhtExternal.set_ItsIArduino(its_pArduino) != ERR_OK)
        Serial.println("ERR: DHT2 Arduino wiring");

    // --- Wire display ---
    if (its_display.set_ItsIArduino(its_pArduino) != ERR_OK) {
        Serial.println("ERR: Display Arduino wiring");
        return;
    }

    if (its_display.begin() != ERR_OK) {
        Serial.println("ERR: Display init");
        return;
    }

    I_Display* disp = its_display.get_ItsIDisplay();

    // --- Initial screen ---
    disp->clear();
    disp->printAt(0, 0, "Init...");
    disp->printAt(0, 10, "Please wait");
    disp->refresh();

    // --- Begin sensors ---
    if (its_dhtInternal.begin() != ERR_OK) {
        disp->clear();
        disp->printAt(0, 0, "ERR: DHT1");
        disp->refresh();
        return;
    }

    if (its_dhtExternal.begin() != ERR_OK) {
        disp->clear();
        disp->printAt(0, 0, "ERR: DHT2");
        disp->refresh();
        return;
    }

    // --- Wire processor ---
    its_processor.set_ItsIInternalDHT22(its_dhtInternal.get_ItsIDHT22());
    its_processor.set_ItsIExternalDHT22(its_dhtExternal.get_ItsIDHT22());
    its_processor.set_ItsIArduino(its_pArduino);
    its_processor.set_ItsIDisplay(disp);

    if (its_processor.begin() != ERR_OK) {
        disp->clear();
        disp->printAt(0, 0, "ERR: Processor");
        disp->refresh();
        return;
    }

    // --- Ready ---
    disp->clear();
    disp->printAt(0, 0, "System Ready");
    disp->refresh();
}

C_Processor* C_Build::get_ItsCProcessor() {
    return &its_processor;
}

I_Processor* C_Build::get_ItsIProcessor() {
    return its_processor.get_ItsIProcessor();
}
