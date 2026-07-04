#ifndef _C_PROCESSOR_H
#define _C_PROCESSOR_H

#include "C_DHT22.h"
#include "C_Display.h"
#include "C_Arduino.h"
#include "ErrorCodes.h"

#define DISP_ERR_PORTS F("ERR: Ports")
#define DISP_ERR_NOT_WIRED F("not wired")
#define DISP_ERR_SENSOR F("ERR: Sensor")
#define DISP_ERR_READ_FAIL F("read fail")

#define DISP_HEADER F("RACKMON")

#define SER_T1   F("T1:")
#define SER_H1   F(",H1:")
#define SER_T2   F(",T2:")
#define SER_H2   F(",H2:")


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
    A_Processor(I_DHT22* pI_DHT22_1,
                I_DHT22* pI_DHT22_2,
                I_Display* pI_Display,
                I_Arduino* pI_Arduino);

    void loop() override;

private:
    I_DHT22*   pItsDHT22_1Interface = nullptr;
    I_DHT22*   pItsDHT22_2Interface = nullptr;
    I_Display* pItsDisplayInterface = nullptr;
    I_Arduino* pItsArduinoInterface = nullptr;

    unsigned long lastSample = 0;
};


/* ============================
   C_Processor (Component)
   ============================ */

class C_Processor {
public:
    C_Processor();

    int setItsDHT22_1Interface(I_DHT22* pIDHT22);
    int setItsDHT22_2Interface(I_DHT22* pIDHT22);
    int setItsDisplayInterface(I_Display* pIDisplay);
    int setItsArduinoInterface(I_Arduino* pIArduino);

    int begin();

    I_Processor* get_ItsIProcessor();
    C_Processor* get_ItsCProcessor();

private:
    I_DHT22*   pItsDHT22_1Interface = nullptr;
    I_DHT22*   pItsDHT22_2Interface = nullptr;
    I_Display* pItsDisplayInterface  = nullptr;
    I_Arduino* pItsArduinoInterface  = nullptr;

    A_Processor itsAdapter;
};

#endif
