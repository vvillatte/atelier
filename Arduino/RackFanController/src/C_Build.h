#ifndef _C_BUILD_H
#define _C_BUILD_H

#include "C_Arduino.h"
#include "C_DHT22.h"
#include "C_Display.h"
#include "C_Processor.h"

#define MSG_BUILD_START        F("[BUILD] Starting build...")
#define MSG_BUILD_CONSTRUCT    F("[BUILD] Constructing components...")
#define MSG_BUILD_CREATED      F("[BUILD] Build object created.")
#define MSG_BUILD_DONE         F("[BUILD] Build complete.")

#define MSG_INIT_START        F("[INIT] Starting initialisation...")
#define MSG_INIT_RAM_START    F("[DEBUG] RAM when at start: ")
#define MSG_WIRING_DHT1       F("[INIT] Wiring DHT1...")
#define MSG_WIRING_DHT2       F("[INIT] Wiring DHT2...")
#define MSG_WIRING_DISPLAY    F("[INIT] Wiring Display...")
#define MSG_INIT_DISPLAY      F("[INIT] Initialising Display...")
#define MSG_INIT_DHT1         F("[INIT] Initialising DHT1...")
#define MSG_INIT_DHT2         F("[INIT] Initialising DHT2...")
#define MSG_WIRING_PROCESSOR  F("[INIT] Wiring Processor...")
#define MSG_INIT_PROCESSOR    F("[INIT] Initialising Processor...")
#define MSG_INIT_RAM_DONE     F("[DEBUG] RAM when at completion: ")
#define MSG_INIT_DONE         F("[INIT] All components initialised.")

#define MSG_INIT_ERR_ARDUINO  F("[INIT] ERR: Arduino interface null")
#define MSG_INIT_ERR_DHT1     F("[INIT] ERR: DHT1 wiring failed")
#define MSG_INIT_ERR_DHT2     F("[INIT] ERR: DHT2 wiring failed")
#define MSG_INIT_ERR_DISPLAY  F("[INIT] ERR: Display wiring failed")
#define MSG_INIT_ERR_PROCESSOR F("[INIT] ERR: Processor initialisation failed")

#define DISP_INIT_LINE1   F("Init...")
#define DISP_INIT_LINE2   F("Please wait")
#define DISP_ERR_DHT1     F("ERR: DHT1")
#define DISP_ERR_DHT2     F("ERR: DHT2")
#define DISP_ERR_PROCESSOR F("ERR: Processor")
#define DISP_READY        F("System Ready")

constexpr uint8_t PIN_DHT1 = 2;
constexpr uint8_t PIN_DHT2 = 4;

enum BuildState {
    BUILD_STATE_CONSTRUCT,
    BUILD_STATE_INITIALISE,
    BUILD_STATE_EXECUTE
};


class C_Build {
public:
    C_Build();

    // State machine driver
    void run();

    // Optional explicit calls
    void initialise();
    void execute();

private:

    // State handlers
    void doConstruct();
    void doInitialise();
    void doExecute();

    BuildState state;
    C_Arduino  itsC_Arduino;
    C_DHT22    itsC_DHT1;
    C_DHT22    itsC_DHT2;
    C_Display  itsC_Display;
    C_Processor itsC_Processor;
};

#endif
