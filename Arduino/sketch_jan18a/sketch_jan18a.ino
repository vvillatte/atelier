int buzzer = 8; // set the buzzer control digital IO pin
int laser = 10;

void setup() {
  pinMode(buzzer, OUTPUT); // set pin 8 as output
  pinMode(laser, OUTPUT);

  Serial.begin(115200);
  
}

void loop() {
    analogWrite(laser, 50); // send high signal to buzzer
    int val = analogRead(A0);
    Serial.println(val);
    if(800 >= val){
      for (int i = 0; i < 80; i++) {  // make a sound
        digitalWrite(buzzer, HIGH); // send high signal to buzzer 
        delay(1); // delay 1ms
        digitalWrite(buzzer, LOW); // send low signal to buzzer
        delay(1);
      }
    }
    delay(100);
}
