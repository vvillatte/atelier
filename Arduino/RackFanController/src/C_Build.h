#ifndef _C_BUILD_H
#define _C_BUILD_H

#include "C_Arduino.h"
#include "C_DHT22.h"
#include "C_Display.h"
#include "C_Processor.h"

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
