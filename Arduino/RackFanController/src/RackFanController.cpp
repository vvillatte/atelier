#include <Arduino.h>
#include "C_Build.h"

#define MSG_SETUP_START F("[SETUP] Starting Rack Fan Controller...")
constexpr unsigned long BAUD_RATE = 115200;

C_Build* itsBuild = nullptr;

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println(MSG_SETUP_START);
    itsBuild = new C_Build();
}

void loop() {
    itsBuild->run();
}
