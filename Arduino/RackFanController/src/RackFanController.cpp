#include <Arduino.h>
#include "C_Build.h"

C_Build* itsBuild = nullptr;

void setup() {
    Serial.begin(115200);
    Serial.println("[SETUP] Starting Rack Fan Controller...");
    itsBuild = new C_Build();
}

void loop() {
    itsBuild->run();
}
