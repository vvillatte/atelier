#include "C_Processor.h"

C_Processor::C_Processor()
    : internal(nullptr), external(nullptr), lcd(nullptr), lastSample(0) {}

int C_Processor::set_ItsIInternalDHT22(I_DHT22* p) {
    if (!p) return ERR_NULL_POINTER;
    internal = p;
    return ERR_OK;
}

int C_Processor::set_ItsIExternalDHT22(I_DHT22* p) {
    if (!p) return ERR_NULL_POINTER;
    external = p;
    return ERR_OK;
}

int C_Processor::set_ItsILCD(I_LCD1602* p) {
    if (!p) return ERR_NULL_POINTER;
    lcd = p;
    return ERR_OK;
}

void C_Processor::loop() {
    if (!internal || !external || !lcd) {
        lcd->clear();
        lcd->printAt(0, 0, "ERR: Ports");
        lcd->printAt(0, 1, "not wired");
        return;
    }

    unsigned long now = millis();
    if (now - lastSample < 10000) return;

    lastSample = now;

    float t1 = internal->readTemperature();
    float h1 = internal->readHumidity();
    float t2 = external->readTemperature();
    float h2 = external->readHumidity();

    if (isnan(t1) || isnan(h1) || isnan(t2) || isnan(h2)) {
        lcd->clear();
        lcd->printAt(0, 0, "ERR: Sensor");
        lcd->printAt(0, 1, "read fail");
        return;
    }

    lcd->clear();
    lcd->printAt(0, 0, "T: " + String(t1,1) + " / " + String(t2,1));
    lcd->printAt(0, 1, "H: " + String(h1,1) + " / " + String(h2,1));
}

C_Processor* C_Processor::get_ItsCProcessor() {
    return this;
}

I_Processor* C_Processor::get_ItsIProcessor() {
    return this;
}
