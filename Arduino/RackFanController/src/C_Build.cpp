#include "C_Build.h"
#include <Arduino.h>

extern unsigned int __heap_start;
extern void *__brkval;

int freeRam() {
    int free_memory;
    if ((int)__brkval == 0)
        free_memory = ((int)&free_memory) - ((int)&__heap_start);
    else
        free_memory = ((int)&free_memory) - ((int)__brkval);
    return free_memory;
}



C_Build::C_Build()
    : state(BUILD_STATE_CONSTRUCT),
      itsC_Arduino(),
      itsC_DHT1(PIN_DHT1),
      itsC_DHT2(PIN_DHT2),
      itsC_Display(),
      itsC_Processor()
{
    Serial.println(MSG_BUILD_CREATED);
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
    Serial.println(MSG_BUILD_DONE);
    state = BUILD_STATE_INITIALISE;
}

/* ============================================================
   INITIALISE PHASE — Wiring + hardware init
   ============================================================ */

void C_Build::doInitialise() {
    Serial.println(MSG_INIT_START);

    Serial.print(MSG_INIT_RAM_START);
    Serial.println(freeRam());

    I_Arduino* pI_Arduino = itsC_Arduino.getInterface();
    if (!pI_Arduino) {
        Serial.println(MSG_INIT_ERR_ARDUINO);
        return;
    }

    /* --- Wire DHT1 --- */
    Serial.println(MSG_WIRING_DHT1);
    if (itsC_DHT1.setItsArduinoInterface(pI_Arduino) != ERR_OK) {
        Serial.println(MSG_INIT_ERR_DHT1);
        return;
    }

    /* --- Wire DHT2 --- */
    Serial.println(MSG_WIRING_DHT2);
    if (itsC_DHT2.setItsArduinoInterface(pI_Arduino) != ERR_OK) {
        Serial.println(MSG_INIT_ERR_DHT2);
        return;
    }

    /* --- Wire Display --- */
    Serial.println(MSG_WIRING_DISPLAY);
    if (itsC_Display.setItsArduinoInterface(pI_Arduino) != ERR_OK) {
        Serial.println(MSG_INIT_ERR_DISPLAY);
        return;
    }

    Serial.println(MSG_INIT_DISPLAY);
    if (itsC_Display.begin() != ERR_OK) {
        Serial.println(MSG_INIT_ERR_DISPLAY);
        return;
    }

    I_Display* pI_Display = itsC_Display.getInterface();
    pI_Display->clear();
    pI_Display->printAt(0, 0, DISP_INIT_LINE1);
    pI_Display->printAt(0, 10, DISP_INIT_LINE2);
    pI_Display->refresh();

    /* --- Begin DHT1 --- */
    Serial.println(MSG_INIT_DHT1);
    if (itsC_DHT1.begin() != ERR_OK) {
        Serial.println(MSG_INIT_ERR_DHT1);
        pI_Display->clear();
        pI_Display->printAt(0, 0, DISP_ERR_DHT1);
        pI_Display->refresh();
        return;
    }

    /* --- Begin DHT2 --- */
    Serial.println(MSG_INIT_DHT2);
    if (itsC_DHT2.begin() != ERR_OK) {
        Serial.println(MSG_INIT_ERR_DHT2);
        pI_Display->clear();
        pI_Display->printAt(0, 0, DISP_ERR_DHT2);
        pI_Display->refresh();
        return;
    }

    /* --- Wire Processor --- */
    Serial.println(MSG_WIRING_PROCESSOR);
    itsC_Processor.setItsDHT22_1Interface(itsC_DHT1.getInterface());
    itsC_Processor.setItsDHT22_2Interface(itsC_DHT2.getInterface());
    itsC_Processor.setItsArduinoInterface(itsC_Arduino.getInterface());
    itsC_Processor.setItsDisplayInterface(itsC_Display.getInterface());

    Serial.println(MSG_INIT_PROCESSOR);
    if (itsC_Processor.begin() != ERR_OK) {
        Serial.println(MSG_INIT_ERR_PROCESSOR);
        pI_Display->clear();
        pI_Display->printAt(0, 0, DISP_ERR_PROCESSOR);
        pI_Display->refresh();
        return;
    }

    Serial.println(MSG_INIT_DONE);
    pI_Display->clear();
    pI_Display->printAt(0, 0, DISP_READY);
    pI_Display->refresh();

    Serial.print(MSG_INIT_RAM_DONE);
    Serial.println(freeRam());

    state = BUILD_STATE_EXECUTE;
}

/* ============================================================
   EXECUTE PHASE — Main processor loop
   ============================================================ */

void C_Build::doExecute() {
    itsC_Processor.get_ItsIProcessor()->loop();
}
