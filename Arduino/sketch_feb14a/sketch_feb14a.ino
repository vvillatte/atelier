const int ANALOG_PIN  = A0;
const int DIGITAL_PIN = 7;

void setup() {
  Serial.begin(9600);
  pinMode(DIGITAL_PIN, INPUT);
}

void loop() {
  int analogValue  = analogRead(ANALOG_PIN);
  int digitalValue = digitalRead(DIGITAL_PIN);

  // Return to start of line
  Serial.print("\r");

  // Print fresh content
  Serial.print("Analog: ");
  Serial.print(analogValue);
  Serial.print("   Digital: ");
  Serial.println(digitalValue);

  delay(500);
}