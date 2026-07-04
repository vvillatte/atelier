#include "C_DHT22.h"

/* ============================
   A_DHT22 (Adapter)
   ============================ */

A_DHT22::A_DHT22(uint8_t pin, I_Arduino* pI_Arduino)
    : dht(pin, DHT22),
      pin(pin),
      pItsArduinoInterface(pI_Arduino)
{}

void A_DHT22::begin() {
    pItsArduinoInterface->pinMode(pin, INPUT);
    dht.begin();
}

int A_DHT22::readTemperature() {
    return (int)(dht.readTemperature()*10);
}

int A_DHT22::readHumidity() {
    return (int)(dht.readHumidity()*10);
}


/* ============================
   C_DHT22 (Component)
   ============================ */

C_DHT22::C_DHT22(uint8_t pin)
    : pin(pin),
      pItsArduinoInterface(nullptr),
      itsAdapter(pin, nullptr),
      adapterReady(false)
{}


int C_DHT22::setItsArduinoInterface(I_Arduino* p_IArduino) {
    if (!p_IArduino) return ERR_NULL_POINTER;

    pItsArduinoInterface = p_IArduino;

    // Pin ownership handled by the Arduino adapter
    int rc = pItsArduinoInterface->requestPin(pin);
    if (rc != ERR_OK) return rc;

    itsAdapter = A_DHT22(pin, pItsArduinoInterface);
    adapterReady = true;

    return ERR_OK;
}

int C_DHT22::begin() {
    if (!adapterReady) return ERR_INVALID_STATE;

    itsAdapter.begin();

    // Minimal sanity check
    float t = itsAdapter.readTemperature();
    if (isnan(t)) return ERR_SENSOR_INIT_FAILED;

    return ERR_OK;
}

I_DHT22* C_DHT22::getInterface() {
    return &itsAdapter;
}

C_DHT22* C_DHT22::getComponent() {
    return this;
}
