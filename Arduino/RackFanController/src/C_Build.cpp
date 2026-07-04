#include "C_Build.h"
#include <Arduino.h>

C_Build::C_Build()
    : state(BUILD_STATE_CONSTRUCT),
      itsC_Arduino(),
      itsC_DHT1(2),
      itsC_DHT2(4),
      itsC_Display(),
      itsC_Processor()
{
    Serial.println("[BUILD] Build object created.");
}

/* ============================================================
   State Machine Driver
   ============================================================ */

void C_Build::run() {
    switch (state) {

        case BUILD_STATE_CONSTRUCT:
            doConstruct();
            break;

        case BUILD_STATE_INITIALISE:
            doInitialise();
            break;

        case BUILD_STATE_EXECUTE:
            doExecute();
            break;
    }
}

/* ============================================================
   Optional explicit calls
   ============================================================ */

void C_Build::initialise() {
    state = BUILD_STATE_INITIALISE;
    doInitialise();
}

void C_Build::execute() {
    state = BUILD_STATE_EXECUTE;
    doExecute();
}

/* ============================================================
   BUILD PHASE — Construction (already done by constructor)
   ============================================================ */

void C_Build::doConstruct() {
    Serial.println("[BUILD] Construction complete.");
    state = BUILD_STATE_INITIALISE;
}

/* ============================================================
   INITIALISE PHASE — Wiring + hardware init
   ============================================================ */

void C_Build::doInitialise() {
    Serial.println("[INIT] Starting initialisation...");

    I_Arduino* pI_Arduino = itsC_Arduino.getInterface();
    if (!pI_Arduino) {
        Serial.println("[INIT] ERR: Arduino interface null");
        return;
    }

    /* --- Wire DHT1 --- */
    Serial.println("[INIT] Wiring DHT1...");
    if (itsC_DHT1.setItsArduinoInterface(pI_Arduino) != ERR_OK) {
        Serial.println("[INIT] ERR: DHT1 wiring failed");
        return;
    }

    /* --- Wire DHT2 --- */
    Serial.println("[INIT] Wiring DHT2...");
    if (itsC_DHT2.setItsArduinoInterface(pI_Arduino) != ERR_OK) {
        Serial.println("[INIT] ERR: DHT2 wiring failed");
        return;
    }

    /* --- Wire Display --- */
    Serial.println("[INIT] Wiring Display...");
    if (itsC_Display.setItsArduinoInterface(pI_Arduino) != ERR_OK) {
        Serial.println("[INIT] ERR: Display wiring failed");
        return;
    }

    Serial.println("[INIT] Initialising Display...");
    if (itsC_Display.begin() != ERR_OK) {
        Serial.println("[INIT] ERR: Display init failed");
        return;
    }

    I_Display* pI_Display = itsC_Display.getInterface();
    pI_Display->clear();
    pI_Display->printAt(0, 0, "Init...");
    pI_Display->printAt(0, 1, "Please wait");
    pI_Display->refresh();

    /* --- Begin DHT1 --- */
    Serial.println("[INIT] Initialising DHT1...");
    if (itsC_DHT1.begin() != ERR_OK) {
        Serial.println("[INIT] ERR: DHT1 init failed");
        pI_Display->clear();
        pI_Display->printAt(0, 0, "ERR: DHT1");
        pI_Display->refresh();
        return;
    }

    /* --- Begin DHT2 --- */
    Serial.println("[INIT] Initialising DHT2...");
    if (itsC_DHT2.begin() != ERR_OK) {
        Serial.println("[INIT] ERR: DHT2 init failed");
        pI_Display->clear();
        pI_Display->printAt(0, 0, "ERR: DHT2");
        pI_Display->refresh();
        return;
    }

    /* --- Wire Processor --- */
    Serial.println("[INIT] Wiring Processor...");
    itsC_Processor.setItsDHT22_1Interface(itsC_DHT1.getInterface());
    itsC_Processor.setItsDHT22_2Interface(itsC_DHT2.getInterface());
    itsC_Processor.setItsArduinoInterface(itsC_Arduino.getInterface());
    itsC_Processor.setItsDisplayInterface(itsC_Display.getInterface());

    Serial.println("[INIT] Initialising Processor...");
    if (itsC_Processor.begin() != ERR_OK) {
        Serial.println("[INIT] ERR: Processor init failed");
        pI_Display->clear();
        pI_Display->printAt(0, 0, "ERR: Processor");
        pI_Display->refresh();
        return;
    }

    Serial.println("[INIT] All components initialised.");
    pI_Display->clear();
    pI_Display->printAt(0, 0, "System Ready");
    pI_Display->refresh();

    state = BUILD_STATE_EXECUTE;
}

/* ============================================================
   EXECUTE PHASE — Main processor loop
   ============================================================ */

void C_Build::doExecute() {
    itsC_Processor.get_ItsIProcessor()->loop();
}
