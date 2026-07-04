#include "C_DHT22.h"

/* ============================
   A_DHT22 (Adapter)
   ============================ */

A_DHT22::A_DHT22(uint8_t pin, I_Arduino* p_IArduino)
    : dht(pin, DHT22),
      pin(pin),
      p_ItsIArduino(p_IArduino)
{}

void A_DHT22::begin() {
    p_ItsIArduino->pinMode(pin, INPUT);
    dht.begin();
}

float A_DHT22::readTemperature() {
    return dht.readTemperature();
}

float A_DHT22::readHumidity() {
    return dht.readHumidity();
}


/* ============================
   C_DHT22 (Component)
   ============================ */

C_DHT22::C_DHT22(uint8_t pin)
    : pin(pin),
      p_ItsIArduino(nullptr),
      itsADHT22(pin, nullptr),   // temporary, will be replaced
      adapterReady(false)
{}


int C_DHT22::set_ItsIArduino(I_Arduino* p_IArduino) {
    if (!p_IArduino) return ERR_NULL_POINTER;

    p_ItsIArduino = p_IArduino;

    // Pin ownership handled by the Arduino adapter
    int rc = p_ItsIArduino->requestPin(pin);
    if (rc != ERR_OK) return rc;

    // Construct adapter safely (no heap)
    itsADHT22 = A_DHT22(pin, p_ItsIArduino);
    adapterReady = true;

    return ERR_OK;
}

int C_DHT22::begin() {
    if (!adapterReady) return ERR_INVALID_STATE;

    itsADHT22.begin();

    // Minimal sanity check
    float t = itsADHT22.readTemperature();
    if (isnan(t)) return ERR_SENSOR_INIT_FAILED;

    return ERR_OK;
}

I_DHT22* C_DHT22::get_ItsIDHT22() {
    return &itsADHT22;
}

C_DHT22* C_DHT22::get_ItsCDHT22() {
    return this;
}
