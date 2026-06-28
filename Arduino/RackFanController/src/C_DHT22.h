#ifndef _C_DHT22_H
#define _C_DHT22_H

#include "C_Arduino.h"
#include "ErrorCodes.h"
#include <DHT.h>

class I_DHT22 {
public:
    virtual ~I_DHT22() = default;
    virtual float readTemperature() = 0;
    virtual float readHumidity() = 0;
};

class A_DHT22 : public I_DHT22 {
public:
    A_DHT22(uint8_t pin, I_Arduino* arduino);
    void begin();
    float readTemperature() override;
    float readHumidity() override;

private:
    DHT dht;
    uint8_t pin;
    I_Arduino* arduino;
};

class C_DHT22 {
public:
    C_DHT22(uint8_t pin);

    int set_ItsIArduino(I_Arduino* p);
    int begin();

    I_DHT22* get_ItsIDHT22();
    C_DHT22* get_ItsCDHT22();

private:
    uint8_t pin;
    I_Arduino* arduino = nullptr;
    A_DHT22* adapter = nullptr;
};

#endif
