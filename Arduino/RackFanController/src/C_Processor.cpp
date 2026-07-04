#include "C_Processor.h"

/* ============================
   Optimal statis formatting
   for temperature and humidity
   ============================ */

static void formatTemp(char* buf, int v) {
    int iv = v / 10;
    int dv = v % 10;

    buf[0] = '0' + (iv / 10);
    buf[1] = '0' + (iv % 10);
    buf[2] = '.';
    buf[3] = '0' + dv;
    buf[4] = '\0';
}

static void formatHum(char* buf, int v) {
    int iv = v / 10;
    int dv = v % 10;

    buf[0] = '0' + (iv / 10);
    buf[1] = '0' + (iv % 10);
    buf[2] = '.';
    buf[3] = '0' + dv;
    buf[4] = '%';
    buf[5] = '\0';
}


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

    if (!pItsDHT22_1Interface || !pItsDHT22_2Interface ||
        !pItsDisplayInterface || !pItsArduinoInterface)
    {
        pItsDisplayInterface->clear();
        pItsDisplayInterface->printAt(0, 0, DISP_ERR_PORTS);
        pItsDisplayInterface->printAt(0, 10, DISP_ERR_NOT_WIRED);
        pItsDisplayInterface->refresh();
        return;
    }

    unsigned long now = pItsArduinoInterface->millis();
    if (now - lastSample < 10000) return;
    lastSample = now;

    int t1 = pItsDHT22_1Interface->readTemperature();
    int h1 = pItsDHT22_1Interface->readHumidity();
    int t2 = pItsDHT22_2Interface->readTemperature();
    int h2 = pItsDHT22_2Interface->readHumidity();

    if (isnan(t1) || isnan(h1) || isnan(t2) || isnan(h2)) {
        pItsDisplayInterface->clear();
        pItsDisplayInterface->printAt(0, 0, DISP_ERR_SENSOR);
        pItsDisplayInterface->printAt(0, 10, DISP_ERR_READ_FAIL);
        pItsDisplayInterface->refresh();
        return;
    }

    // --- Format buffers ---
    char t1buf[8], t2buf[8], h1buf[8], h2buf[8];

    formatTemp(t1buf, t1);
    formatTemp(t2buf, t2);
    formatHum(h1buf, h1);
    formatHum(h2buf, h2);

    if (strcmp(h1buf, "100.0") == 0) strcpy(h1buf, "100");
    if (strcmp(h2buf, "100.0") == 0) strcpy(h2buf, "100");

    pItsDisplayInterface->clear();

    // ============================================================
    // OLED‑only build
    // ============================================================
#ifdef USE_OLED_DISPLAY

    // Clear screen
    pItsDisplayInterface->clear();

    pItsDisplayInterface->setTextSize(OLED_TEXT_SIZE_MEDIUM);
    pItsDisplayInterface->printAt(22, 0,  DISP_HEADER);   // Centred

    // --- Temperature (bright) ---
    pItsDisplayInterface->printAt(0, 20, t1buf);
    pItsDisplayInterface->printAt(68, 20, h1buf);

    // --- Humidity (dim) ---
    pItsDisplayInterface->printAt(0, 36, t2buf);
    pItsDisplayInterface->printAt(68, 36, h2buf);
#else

    // ============================================================
    // LCD‑only build
    // ============================================================

    char line1[20];
    char line2[20];

    snprintf(line1, sizeof(line1), "T:%s / %s", t1buf, t2buf);
    snprintf(line2, sizeof(line2), "H:%s / %s", h1buf, h2buf);

    pItsDisplayInterface->printAt(0, 0, line1);
    pItsDisplayInterface->printAt(0, 1, line2);

#endif

    pItsDisplayInterface->refresh();

    // --- Serial output ---
    pItsArduinoInterface->serialPrint(SER_T1);
    pItsArduinoInterface->serialPrint(t1buf);
    pItsArduinoInterface->serialPrint(SER_H1);
    pItsArduinoInterface->serialPrint(h1buf);
    pItsArduinoInterface->serialPrint(SER_T2);
    pItsArduinoInterface->serialPrint(t2buf);
    pItsArduinoInterface->serialPrint(SER_H2);
    pItsArduinoInterface->serialPrintLn(h2buf);
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
