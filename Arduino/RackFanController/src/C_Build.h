#ifndef _C_BUILD_H
#define _C_BUILD_H

#include "C_Arduino.h"
#include "C_DHT22.h"
#include "C_Display.h"
#include "C_Processor.h"

class C_Build {
public:
    C_Build();
    void begin();

    C_Processor* get_ItsCProcessor();
    I_Processor* get_ItsIProcessor();

private:
    C_Arduino  its_Arduino;
    C_DHT22    its_dhtInternal;
    C_DHT22    its_dhtExternal;
    C_Display  its_display;
    C_Processor its_processor;
};

#endif
