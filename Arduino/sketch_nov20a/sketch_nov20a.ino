#include <ArduinoBLE.h>

const int ledPin = LED_BUILTIN; // set ledPin to on-board LED

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);


void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(ledPin, OUTPUT); // use the LED as an output

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("TestLED");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(ledService);

  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);
  // add the service
  BLE.addService(ledService);

  ledCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // put your main code here, to run repeatedly:
  // poll for BLE events
  BLE.poll();

  if (ledCharacteristic.written()) {
    // update LED, either central has written to characteristic or button state has changed
    if (ledCharacteristic.value()) {
      Serial.println("LED on");
      digitalWrite(ledPin, HIGH);
    } else {
      Serial.println("LED off");
      digitalWrite(ledPin, LOW);
    }
  }
}
