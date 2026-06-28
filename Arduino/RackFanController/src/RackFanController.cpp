#include <Arduino.h>
#include "C_Build.h"

C_Build build;
I_Processor* processor;

void setup() {
    Serial.begin(9600);
    build.begin();
    processor = build.get_ItsIProcessor();
}

void loop() {
    processor->loop();
}
