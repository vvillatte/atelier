#include "C_DHT22.h"

/* ============================
   A_DHT22 (Adapter)
   ============================ */

A_DHT22::A_DHT22(uint8_t pin, I_Arduino* its_pArduino)
    : dht(pin, DHT22),
      pin(pin),
      its_pArduino(its_pArduino) {}

void A_DHT22::begin() {
    its_pArduino->pinMode(pin, INPUT);
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
      its_pArduino(nullptr),
      adapter(nullptr) {}

int C_DHT22::set_ItsIArduino(I_Arduino* p) {
    if (!p) return ERR_NULL_POINTER;

    its_pArduino = p;

    // Pin ownership handled by the Arduino adapter
    int rc = its_pArduino->requestPin(pin);
    if (rc != ERR_OK) return rc;

    adapter = new A_DHT22(pin, its_pArduino);
    return ERR_OK;
}

int C_DHT22::begin() {
    if (!adapter) return ERR_INVALID_STATE;

    adapter->begin();

    // Minimal sanity check
    float t = adapter->readTemperature();
    if (isnan(t)) return ERR_SENSOR_INIT_FAILED;

    return ERR_OK;
}

I_DHT22* C_DHT22::get_ItsIDHT22() {
    return adapter;
}

C_DHT22* C_DHT22::get_ItsCDHT22() {
    return this;
}
