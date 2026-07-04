#include "C_Build.h"
#include <Arduino.h>

C_Build::C_Build()
    : state(BUILD_STATE_CONSTRUCT),
      its_CArduino(),
      its_CDHT1(2),
      its_CDHT2(4),
      its_CDisplay(),
      its_CProcessor()
{
    Serial.println("[BUILD] Build object created, entering CONSTRUCT state...");
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
    Serial.println("[BUILD] Constructing components...");

    Serial.println("[BUILD] Arduino constructed");
    Serial.println("[BUILD] DHT1 constructed on pin 2");
    Serial.println("[BUILD] DHT2 constructed on pin 4");
    Serial.println("[BUILD] Display constructed");
    Serial.println("[BUILD] Processor constructed");

    Serial.println("[BUILD] Construction complete.");
    state = BUILD_STATE_INITIALISE;
}

/* ============================================================
   INITIALISE PHASE — Wiring + hardware init
   ============================================================ */

void C_Build::doInitialise() {
    Serial.println("[INIT] Starting initialisation...");

    I_Arduino* p_ItsIArduino = its_CArduino.get_ItsIArduino();
    if (!p_ItsIArduino) {
        Serial.println("[INIT] ERR: Arduino interface null");
        return;
    }

    /* --- Wire DHT1 --- */
    Serial.println("[INIT] Wiring DHT1...");
    if (its_CDHT1.set_ItsIArduino(p_ItsIArduino) != ERR_OK) {
        Serial.println("[INIT] ERR: DHT1 wiring failed");
        return;
    }

    /* --- Wire DHT2 --- */
    Serial.println("[INIT] Wiring DHT2...");
    if (its_CDHT2.set_ItsIArduino(p_ItsIArduino) != ERR_OK) {
        Serial.println("[INIT] ERR: DHT2 wiring failed");
        return;
    }

    /* --- Wire Display --- */
    Serial.println("[INIT] Wiring Display...");
    if (its_CDisplay.set_ItsIArduino(p_ItsIArduino) != ERR_OK) {
        Serial.println("[INIT] ERR: Display wiring failed");
        return;
    }

    Serial.println("[INIT] Initialising Display...");
    if (its_CDisplay.begin() != ERR_OK) {
        Serial.println("[INIT] ERR: Display init failed");
        return;
    }

    I_Display* p_ItsIDisplay = its_CDisplay.get_ItsIDisplay();
    p_ItsIDisplay->clear();
    p_ItsIDisplay->printAt(0, 0, "Init...");
    p_ItsIDisplay->printAt(0, 10, "Please wait");
    p_ItsIDisplay->refresh();

    /* --- Begin DHT1 --- */
    Serial.println("[INIT] Initialising DHT1...");
    if (its_CDHT1.begin() != ERR_OK) {
        Serial.println("[INIT] ERR: DHT1 init failed");
        p_ItsIDisplay->clear();
        p_ItsIDisplay->printAt(0, 0, "ERR: DHT1");
        p_ItsIDisplay->refresh();
        return;
    }

    /* --- Begin DHT2 --- */
    Serial.println("[INIT] Initialising DHT2...");
    if (its_CDHT2.begin() != ERR_OK) {
        Serial.println("[INIT] ERR: DHT2 init failed");
        p_ItsIDisplay->clear();
        p_ItsIDisplay->printAt(0, 0, "ERR: DHT2");
        p_ItsIDisplay->refresh();
        return;
    }

    /* --- Wire Processor --- */
    Serial.println("[INIT] Wiring Processor...");
    its_CProcessor.set_ItsIInternalDHT22(its_CDHT1.get_ItsIDHT22());
    its_CProcessor.set_ItsIExternalDHT22(its_CDHT2.get_ItsIDHT22());
    its_CProcessor.set_ItsIArduino(its_CArduino.get_ItsIArduino());
    its_CProcessor.set_ItsIDisplay(its_CDisplay.get_ItsIDisplay());

    Serial.println("[INIT] Initialising Processor...");
    if (its_CProcessor.begin() != ERR_OK) {
        Serial.println("[INIT] ERR: Processor init failed");
        p_ItsIDisplay->clear();
        p_ItsIDisplay->printAt(0, 0, "ERR: Processor");
        p_ItsIDisplay->refresh();
        return;
    }

    Serial.println("[INIT] All components initialised.");
    p_ItsIDisplay->clear();
    p_ItsIDisplay->printAt(0, 0, "System Ready");
    p_ItsIDisplay->refresh();

    state = BUILD_STATE_EXECUTE;
}

/* ============================================================
   EXECUTE PHASE — Main processor loop
   ============================================================ */

void C_Build::doExecute() {
    its_CProcessor.get_ItsIProcessor()->loop();
}
