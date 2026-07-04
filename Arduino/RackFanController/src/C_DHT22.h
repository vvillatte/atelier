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
    virtual float readTemperature() = 0;
    virtual float readHumidity() = 0;
};


/* ============================
   A_DHT22 (Adapter)
   ============================ */

class A_DHT22 : public I_DHT22 {
public:
    A_DHT22(uint8_t pin, I_Arduino* p_IArduino);

    void begin() override;
    float readTemperature() override;
    float readHumidity() override;

private:
    DHT dht;
    uint8_t pin;
    I_Arduino* p_ItsIArduino;
};


/* ============================
   C_DHT22 (Component)
   ============================ */

class C_DHT22 {
public:
    C_DHT22(uint8_t pin);

    int set_ItsIArduino(I_Arduino* p);
    int begin();

    I_DHT22* get_ItsIDHT22();
    C_DHT22* get_ItsCDHT22();

private:
    uint8_t pin;

    I_Arduino* p_ItsIArduino = nullptr;
    A_DHT22 itsADHT22;
    bool adapterReady = false;
};

#endif
