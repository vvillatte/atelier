#include "C_Processor.h"

/* ============================
   A_Processor (Adapter)
   ============================ */

A_Processor::A_Processor(I_DHT22* pI_DHT22_1,
                         I_DHT22* pI_DHT22_2,
                         I_Display* pI_Display,
                         I_Arduino* pI_Arduino)
    : pItsDHT22_1Interface(pI_DHT22_1),
      pItsDHT22_2Interface(pI_DHT22_2),
      pItsDisplayInterface(pI_Display),
      pItsArduinoInterface(pI_Arduino),
      lastSample(0)
{}

void A_Processor::loop() {
    if (!pItsDHT22_1Interface || !pItsDHT22_2Interface || !pItsDisplayInterface || !pItsArduinoInterface) {
        pItsDisplayInterface->clear();
        pItsDisplayInterface->printAt(0, 0, "ERR: Ports");
        pItsDisplayInterface->printAt(0, 10, "not wired");
        pItsDisplayInterface->refresh();
        return;
    }

    unsigned long now = pItsArduinoInterface->millis();
    if (now - lastSample < 10000) return;
    lastSample = now;

    float t1 = pItsDHT22_1Interface->readTemperature();
    float h1 = pItsDHT22_1Interface->readHumidity();
    float t2 = pItsDHT22_2Interface->readTemperature();
    float h2 = pItsDHT22_2Interface->readHumidity();

    if (isnan(t1) || isnan(h1) || isnan(t2) || isnan(h2)) {
        pItsDisplayInterface->clear();
        pItsDisplayInterface->printAt(0, 0, "ERR: Sensor");
        pItsDisplayInterface->printAt(0, 10, "read fail");
        pItsDisplayInterface->refresh();
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

    pItsDisplayInterface->clear();

    switch (pItsDisplayInterface->getDisplayType()) {

        case DISPLAY_LCD:
            // LCD: 2 rows, character grid
            pItsDisplayInterface->printAt(0, 0, "T:" + fmtTemp(t1) + " / " + fmtTemp(t2));
            pItsDisplayInterface->printAt(0, 1, "H:" + fmtHum(h1) + " / " + fmtHum(h2));
            break;

        case DISPLAY_OLED:
            // OLED: 128x64 pixels, 8px text height
            pItsDisplayInterface->printAt(0, 0,  "T1: " + fmtTemp(t1));
            pItsDisplayInterface->printAt(0, 10, "T2: " + fmtTemp(t2));
            pItsDisplayInterface->printAt(0, 20, "H1: " + fmtHum(h1));
            pItsDisplayInterface->printAt(0, 30, "H2: " + fmtHum(h2));
            break;
    }

    pItsDisplayInterface->refresh();

    pItsArduinoInterface->serialPrint("T1:");
    pItsArduinoInterface->serialPrint(String(t1, 1));
    pItsArduinoInterface->serialPrint(",H1:");
    pItsArduinoInterface->serialPrint(String(h1, 1));
    pItsArduinoInterface->serialPrint(",T2:");
    pItsArduinoInterface->serialPrint(String(t2, 1));
    pItsArduinoInterface->serialPrint(",H2:");
    pItsArduinoInterface->serialPrintLn(String(h2, 1));
}


/* ============================
   C_Processor (Component)
   ============================ */

C_Processor::C_Processor()
    : pItsDHT22_1Interface(nullptr),
      pItsDHT22_2Interface(nullptr),
      pItsDisplayInterface(nullptr),
      pItsArduinoInterface(nullptr),
      itsAdapter(nullptr, nullptr, nullptr, nullptr)
{}

int C_Processor::setItsDHT22_1Interface(I_DHT22* pI_DHT22) {
    if (!pI_DHT22) return ERR_NULL_POINTER;
    pItsDHT22_1Interface = pI_DHT22;
    return ERR_OK;
}

int C_Processor::setItsDHT22_2Interface(I_DHT22* pI_DHT22) {
    if (!pI_DHT22) return ERR_NULL_POINTER;
    pItsDHT22_2Interface = pI_DHT22;
    return ERR_OK;
}

int C_Processor::setItsDisplayInterface(I_Display* pI_Display) {
    if (!pI_Display) return ERR_NULL_POINTER;
    pItsDisplayInterface = pI_Display;
    return ERR_OK;
}

int C_Processor::setItsArduinoInterface(I_Arduino* pI_Arduino) {
    if (!pI_Arduino) return ERR_NULL_POINTER;
    pItsArduinoInterface = pI_Arduino;
    return ERR_OK;
}

int C_Processor::begin() {
    if (!pItsDHT22_1Interface || !pItsDHT22_2Interface || !pItsDisplayInterface || !pItsArduinoInterface)
        return ERR_INVALID_STATE;

    itsAdapter = A_Processor(pItsDHT22_1Interface,
                              pItsDHT22_2Interface,
                              pItsDisplayInterface,
                              pItsArduinoInterface);
    return ERR_OK;
}

I_Processor* C_Processor::get_ItsIProcessor() {
    return &itsAdapter;
}

C_Processor* C_Processor::get_ItsCProcessor() {
    return this;
}
