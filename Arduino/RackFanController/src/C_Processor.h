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
    A_Processor(I_DHT22* p_ItsIDHT22_1,
                I_DHT22* p_ItsIDHT22_2,
                I_Display* p_ItsIDisplay,
                I_Arduino* p_ItsIArduino);

    void loop() override;

private:
    I_DHT22*   p_ItsIDHT22_1;
    I_DHT22*   p_ItsIDHT22_2;
    I_Display* p_ItsIDisplay;
    I_Arduino* p_ItsIArduino;

    unsigned long lastSample = 0;
};


/* ============================
   C_Processor (Component)
   ============================ */

class C_Processor {
public:
    C_Processor();

    int set_ItsIInternalDHT22(I_DHT22* pIDHT22);
    int set_ItsIExternalDHT22(I_DHT22* pIDHT22);
    int set_ItsIDisplay(I_Display* pIDisplay);
    int set_ItsIArduino(I_Arduino* pIArduino);

    int begin();

    I_Processor* get_ItsIProcessor();
    C_Processor* get_ItsCProcessor();

private:
    I_DHT22*   p_ItsIDHT22_1 = nullptr;
    I_DHT22*   p_ItsIDHT22_2 = nullptr;
    I_Display* p_ItsIDisplay  = nullptr;
    I_Arduino* p_ItsIArduino  = nullptr;

    A_Processor itsAdapter;
};

#endif
