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
    C_Arduino  its_CArduino;
    C_DHT22    its_CDHT1;
    C_DHT22    its_CDHT2;
    C_Display  its_CDisplay;
    C_Processor its_CProcessor;
};

#endif
