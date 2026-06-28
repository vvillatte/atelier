#include "C_Processor.h"

/* ============================
   A_Processor (Adapter)
   ============================ */

A_Processor::A_Processor(I_DHT22* its_pInternal,
                         I_DHT22* its_pExternal,
                         I_LCD1602* its_pLCD,
                         I_Arduino* its_pArduino)
    : its_pInternal(its_pInternal),
      its_pExternal(its_pExternal),
      its_pLCD(its_pLCD),
      its_pArduino(its_pArduino),
      lastSample(0) {}

void A_Processor::loop() {
    if (!its_pInternal || !its_pExternal || !its_pLCD || !its_pArduino) {
        its_pLCD->clear();
        its_pLCD->printAt(0, 0, "ERR: Ports");
        its_pLCD->printAt(0, 1, "not wired");
        return;
    }

    unsigned long now = its_pArduino->millis();   // Correct source of millis()
    if (now - lastSample < 10000) return;

    lastSample = now;

    float t1 = its_pInternal->readTemperature();
    float h1 = its_pInternal->readHumidity();
    float t2 = its_pExternal->readTemperature();
    float h2 = its_pExternal->readHumidity();

    if (isnan(t1) || isnan(h1) || isnan(t2) || isnan(h2)) {
        its_pLCD->clear();
        its_pLCD->printAt(0, 0, "ERR: Sensor");
        its_pLCD->printAt(0, 1, "read fail");
        return;
    }

    // --- Format humidity safely ---
    auto fmtHum = [](float h) {
        String s = String(h, 1);   // e.g. "100.0"
        if (s == "100.0") s = "100.";  // trim to avoid overflow
        return s + "%";
    };

    // --- Format temperature safely ---
    auto fmtTemp = [](float t) {
        return String(t, 1) + "C";   // degree symbol unsupported on UNO R4
    };

    its_pLCD->clear();
    its_pLCD->printAt(0, 0, "T: " + fmtTemp(t1) + " / " + fmtTemp(t2));
    its_pLCD->printAt(0, 1, "H: " + fmtHum(h1) + " / " + fmtHum(h2));
}


/* ============================
   C_Processor (Component)
   ============================ */

C_Processor::C_Processor()
    : its_pInternal(nullptr),
      its_pExternal(nullptr),
      its_pLCD(nullptr),
      its_pArduino(nullptr),
      adapter(nullptr) {}

int C_Processor::set_ItsIInternalDHT22(I_DHT22* p) {
    if (!p) return ERR_NULL_POINTER;
    its_pInternal = p;
    return ERR_OK;
}

int C_Processor::set_ItsIExternalDHT22(I_DHT22* p) {
    if (!p) return ERR_NULL_POINTER;
    its_pExternal = p;
    return ERR_OK;
}

int C_Processor::set_ItsILCD(I_LCD1602* p) {
    if (!p) return ERR_NULL_POINTER;
    its_pLCD = p;
    return ERR_OK;
}

int C_Processor::set_ItsIArduino(I_Arduino* p) {
    if (!p) return ERR_NULL_POINTER;
    its_pArduino = p;
    return ERR_OK;
}

int C_Processor::begin() {
    if (!its_pInternal || !its_pExternal || !its_pLCD || !its_pArduino)
        return ERR_INVALID_STATE;

    adapter = new A_Processor(its_pInternal,
                              its_pExternal,
                              its_pLCD,
                              its_pArduino);
    return ERR_OK;
}

I_Processor* C_Processor::get_ItsIProcessor() {
    return adapter;
}

C_Processor* C_Processor::get_ItsCProcessor() {
    return this;
}
