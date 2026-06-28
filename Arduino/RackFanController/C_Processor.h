#ifndef _C_PROCESSOR_H
#define _C_PROCESSOR_H

#include "C_DHT22.h"
#include "C_LCD1602I2C.h"
#include "ErrorCodes.h"

class I_Processor {
public:
    virtual ~I_Processor() = default;
    virtual void loop() = 0;
};

class C_Processor : public I_Processor {
public:
    C_Processor();

    int set_ItsIInternalDHT22(I_DHT22* p);
    int set_ItsIExternalDHT22(I_DHT22* p);
    int set_ItsILCD(I_LCD1602* p);

    void loop() override;

    C_Processor* get_ItsCProcessor();
    I_Processor* get_ItsIProcessor();

private:
    I_DHT22* internal = nullptr;
    I_DHT22* external = nullptr;
    I_LCD1602* lcd = nullptr;

    unsigned long lastSample;
};

#endif
