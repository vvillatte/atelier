#include "C_Processor.h"

/* ============================
   A_Processor (Adapter)
   ============================ */

A_Processor::A_Processor(I_DHT22* its_pInternal,
                         I_DHT22* its_pExternal,
                         I_Display* its_pDisplay,
                         I_Arduino* its_pArduino)
    : p_ItsIDHT22_1(its_pInternal),
      p_ItsIDHT22_2(its_pExternal),
      p_ItsIDisplay(its_pDisplay),
      p_ItsIArduino(its_pArduino),
      lastSample(0)
{}

void A_Processor::loop() {
    if (!p_ItsIDHT22_1 || !p_ItsIDHT22_2 || !p_ItsIDisplay || !p_ItsIArduino) {
        p_ItsIDisplay->clear();
        p_ItsIDisplay->printAt(0, 0, "ERR: Ports");
        p_ItsIDisplay->printAt(0, 10, "not wired");
        p_ItsIDisplay->refresh();
        return;
    }

    unsigned long now = p_ItsIArduino->millis();
    if (now - lastSample < 10000) return;
    lastSample = now;

    float t1 = p_ItsIDHT22_1->readTemperature();
    float h1 = p_ItsIDHT22_1->readHumidity();
    float t2 = p_ItsIDHT22_2->readTemperature();
    float h2 = p_ItsIDHT22_2->readHumidity();

    if (isnan(t1) || isnan(h1) || isnan(t2) || isnan(h2)) {
        p_ItsIDisplay->clear();
        p_ItsIDisplay->printAt(0, 0, "ERR: Sensor");
        p_ItsIDisplay->printAt(0, 10, "read fail");
        p_ItsIDisplay->refresh();
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

    p_ItsIDisplay->clear();

    switch (p_ItsIDisplay->getDisplayType()) {

        case DISPLAY_LCD1602I2C:
            // LCD: 2 rows, character grid
            p_ItsIDisplay->printAt(0, 0, "T:" + fmtTemp(t1) + "/" + fmtTemp(t2));
            p_ItsIDisplay->printAt(0, 1, "H:" + fmtHum(h1) + "/" + fmtHum(h2));
            break;

        case DISPLAY_OLED12864SSD1306:
            // OLED: 128x64 pixels, 8px text height
            p_ItsIDisplay->printAt(0, 0,  "T1: " + fmtTemp(t1));
            p_ItsIDisplay->printAt(0, 10, "T2: " + fmtTemp(t2));
            p_ItsIDisplay->printAt(0, 20, "H1: " + fmtHum(h1));
            p_ItsIDisplay->printAt(0, 30, "H2: " + fmtHum(h2));
            break;
    }

    p_ItsIDisplay->refresh();

    p_ItsIArduino->serialPrint("T1:");
    p_ItsIArduino->serialPrint(String(t1, 1));
    p_ItsIArduino->serialPrint(",H1:");
    p_ItsIArduino->serialPrint(String(h1, 1));
    p_ItsIArduino->serialPrint(",T2:");
    p_ItsIArduino->serialPrint(String(t2, 1));
    p_ItsIArduino->serialPrint(",H2:");
    p_ItsIArduino->serialPrintLn(String(h2, 1));
}


/* ============================
   C_Processor (Component)
   ============================ */

C_Processor::C_Processor()
    : p_ItsIDHT22_1(nullptr),
      p_ItsIDHT22_2(nullptr),
      p_ItsIDisplay(nullptr),
      p_ItsIArduino(nullptr),
      itsAdapter(nullptr, nullptr, nullptr, nullptr)
{}

int C_Processor::set_ItsIInternalDHT22(I_DHT22* pIDHT22) {
    if (!pIDHT22) return ERR_NULL_POINTER;
    p_ItsIDHT22_1 = pIDHT22;
    return ERR_OK;
}

int C_Processor::set_ItsIExternalDHT22(I_DHT22* pIDHT22) {
    if (!pIDHT22) return ERR_NULL_POINTER;
    p_ItsIDHT22_2 = pIDHT22;
    return ERR_OK;
}

int C_Processor::set_ItsIDisplay(I_Display* pIDisplay) {
    if (!pIDisplay) return ERR_NULL_POINTER;
    p_ItsIDisplay = pIDisplay;
    return ERR_OK;
}

int C_Processor::set_ItsIArduino(I_Arduino* pIArduino) {
    if (!pIArduino) return ERR_NULL_POINTER;
    p_ItsIArduino = pIArduino;
    return ERR_OK;
}

int C_Processor::begin() {
    if (!p_ItsIDHT22_1 || !p_ItsIDHT22_2 || !p_ItsIDisplay || !p_ItsIArduino)
        return ERR_INVALID_STATE;

    itsAdapter = A_Processor(p_ItsIDHT22_1,
                              p_ItsIDHT22_2,
                              p_ItsIDisplay,
                              p_ItsIArduino);
    return ERR_OK;
}

I_Processor* C_Processor::get_ItsIProcessor() {
    return &itsAdapter;
}

C_Processor* C_Processor::get_ItsCProcessor() {
    return this;
}
