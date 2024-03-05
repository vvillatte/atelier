int trigPin = 11; // Trigger
int echoPin = 12; // Echo
int laserPin = 6; // Laser
long duration = 0; // Duration
long distance = 0;  // Distance

void setup(){
  //Serial Port begin
  Serial.begin (115200);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(laserPin, OUTPUT);
}

//void blink(int delay){
//  analogueWrite(laserPin, HIGH);
//  
//}

void loop(){
  // Pulse the laser at low rate initially then accelerate when the distance reduces
  analogWrite(laserPin,5);
  
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  
  // Convert the time into a distance
  // Multiply by 0.3436426116838488‬ for mm
  distance = (duration/2.0) * 0.3436426116838488;
  
  Serial.print(distance);
  Serial.print("mm");
  Serial.println();
  
  delay(250);
}
