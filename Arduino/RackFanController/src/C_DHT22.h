#ifndef _C_DHT22_H
#define _C_DHT22_H

#include "C_Arduino.h"
#include "ErrorCodes.h"
#include <DHT.h>

/* ============================
   I_DHT22 (Interface)
   ============================ */

class I_DHT22 {
public:
    virtual ~I_DHT22() = default;
    virtual void begin() = 0;
    virtual int readTemperature() = 0;
    virtual int readHumidity() = 0;
};


/* ============================
   A_DHT22 (Adapter)
   ============================ */

class A_DHT22 : public I_DHT22 {
public:
    A_DHT22(uint8_t pin, I_Arduino* p_IArduino);

    void begin() override;
    int readTemperature() override;
    int readHumidity() override;

private:
    DHT dht;
    uint8_t pin;
    I_Arduino* pItsArduinoInterface = nullptr;
};


/* ============================
   C_DHT22 (Component)
   ============================ */

class C_DHT22 {
public:
    C_DHT22(uint8_t pin);

    int setItsArduinoInterface(I_Arduino* p);
    int begin();

    I_DHT22* getInterface();
    C_DHT22* getComponent();

private:
    uint8_t pin;

    I_Arduino* pItsArduinoInterface = nullptr;
    A_DHT22 itsAdapter;
    bool adapterReady = false;
};

#endif
