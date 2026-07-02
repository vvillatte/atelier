const int togglePin = 2;
//const int PWMPin = 5;
int pinState = LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(togglePin, OUTPUT);
//  pinMode(PWMPin, OUTPUT);

  digitalWrite(togglePin, pinState);
//  analogWrite(PWMPin, 127);
}

void loop() {
  // put your main code here, to run repeatedly:
  pinState = (LOW == pinState) ? HIGH : LOW;
  digitalWrite(togglePin, pinState);
}
