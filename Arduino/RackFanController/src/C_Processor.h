#ifndef _C_PROCESSOR_H
#define _C_PROCESSOR_H

#include "C_DHT22.h"
#include "C_Display.h"
#include "C_Arduino.h"
#include "ErrorCodes.h"

/* ============================
   I_Processor (Interface)
   ============================ */

class I_Processor {
public:
    virtual ~I_Processor() = default;
    virtual void loop() = 0;
};


/* ============================
   A_Processor (Adapter)
   ============================ */

class A_Processor : public I_Processor {
public:
    A_Processor(I_DHT22* its_pInternal,
                I_DHT22* its_pExternal,
                I_Display* its_pDisplay,
                I_Arduino* its_pArduino);

    void loop() override;

private:
    I_DHT22*   its_pInternal;
    I_DHT22*   its_pExternal;
    I_Display* its_pDisplay;
    I_Arduino* its_pArduino;

    unsigned long lastSample = 0;
};


/* ============================
   C_Processor (Component)
   ============================ */

class C_Processor {
public:
    C_Processor();

    int set_ItsIInternalDHT22(I_DHT22* p);
    int set_ItsIExternalDHT22(I_DHT22* p);
    int set_ItsIDisplay(I_Display* p);
    int set_ItsIArduino(I_Arduino* p);

    int begin();

    I_Processor* get_ItsIProcessor();
    C_Processor* get_ItsCProcessor();

private:
    I_DHT22*   its_pInternal = nullptr;
    I_DHT22*   its_pExternal = nullptr;
    I_Display* its_pDisplay  = nullptr;
    I_Arduino* its_pArduino  = nullptr;

    A_Processor* adapter = nullptr;
};

#endif
