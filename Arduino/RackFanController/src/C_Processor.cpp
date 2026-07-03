#include "C_Processor.h"

/* ============================
   A_Processor (Adapter)
   ============================ */

A_Processor::A_Processor(I_DHT22* its_pInternal,
                         I_DHT22* its_pExternal,
                         I_Display* its_pDisplay,
                         I_Arduino* its_pArduino)
    : its_pInternal(its_pInternal),
      its_pExternal(its_pExternal),
      its_pDisplay(its_pDisplay),
      its_pArduino(its_pArduino),
      lastSample(0)
{}

void A_Processor::loop() {
    if (!its_pInternal || !its_pExternal || !its_pDisplay || !its_pArduino) {
        its_pDisplay->clear();
        its_pDisplay->printAt(0, 0, "ERR: Ports");
        its_pDisplay->printAt(0, 10, "not wired");
        its_pDisplay->refresh();
        return;
    }

    unsigned long now = its_pArduino->millis();
    if (now - lastSample < 10000) return;
    lastSample = now;

    float t1 = its_pInternal->readTemperature();
    float h1 = its_pInternal->readHumidity();
    float t2 = its_pExternal->readTemperature();
    float h2 = its_pExternal->readHumidity();

    if (isnan(t1) || isnan(h1) || isnan(t2) || isnan(h2)) {
        its_pDisplay->clear();
        its_pDisplay->printAt(0, 0, "ERR: Sensor");
        its_pDisplay->printAt(0, 10, "read fail");
        its_pDisplay->refresh();
        return;
    }

    auto fmtHum = [](float h) {
        String s = String(h, 1);
        if (s == "100.0") s = "100";
        s += "%";
        if (s.length() < 5) s += " ";
        return s;
    };

    auto fmtTemp = [](float t) {
        return String(t, 1) + "C";
    };

    its_pDisplay->clear();

    switch (its_pDisplay->getDisplayType()) {

        case DISPLAY_LCD1602:
            // LCD: 2 rows, character grid
            its_pDisplay->printAt(0, 0, "T:" + fmtTemp(t1) + "/" + fmtTemp(t2));
            its_pDisplay->printAt(0, 1, "H:" + fmtHum(h1) + "/" + fmtHum(h2));
            break;

        case DISPLAY_OLED12864:
            // OLED: 128x64 pixels, 8px text height
            its_pDisplay->printAt(0, 0,  "T1: " + fmtTemp(t1));
            its_pDisplay->printAt(0, 10, "T2: " + fmtTemp(t2));
            its_pDisplay->printAt(0, 20, "H1: " + fmtHum(h1));
            its_pDisplay->printAt(0, 30, "H2: " + fmtHum(h2));
            break;
    }

    its_pDisplay->refresh();

    its_pArduino->serialPrint("T1:");
    its_pArduino->serialPrint(String(t1, 1));
    its_pArduino->serialPrint(",H1:");
    its_pArduino->serialPrint(String(h1, 1));
    its_pArduino->serialPrint(",T2:");
    its_pArduino->serialPrint(String(t2, 1));
    its_pArduino->serialPrint(",H2:");
    its_pArduino->serialPrintLn(String(h2, 1));
}


/* ============================
   C_Processor (Component)
   ============================ */

C_Processor::C_Processor()
    : its_pInternal(nullptr),
      its_pExternal(nullptr),
      its_pDisplay(nullptr),
      its_pArduino(nullptr),
      adapter(nullptr)
{}

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

int C_Processor::set_ItsIDisplay(I_Display* p) {
    if (!p) return ERR_NULL_POINTER;
    its_pDisplay = p;
    return ERR_OK;
}

int C_Processor::set_ItsIArduino(I_Arduino* p) {
    if (!p) return ERR_NULL_POINTER;
    its_pArduino = p;
    return ERR_OK;
}

int C_Processor::begin() {
    if (!its_pInternal || !its_pExternal || !its_pDisplay || !its_pArduino)
        return ERR_INVALID_STATE;

    adapter = new A_Processor(its_pInternal,
                              its_pExternal,
                              its_pDisplay,
                              its_pArduino);
    return ERR_OK;
}

I_Processor* C_Processor::get_ItsIProcessor() {
    return adapter;
}

C_Processor* C_Processor::get_ItsCProcessor() {
    return this;
}
