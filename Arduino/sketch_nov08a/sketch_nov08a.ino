// Set for debug or comment out
#define _DEBUG

// Include Thread.h for multithreading
//#define _MULTITHREAD
#ifdef  _MULTITHREAD
#include <Thread.h>
#endif

// Global definitions
#define PIN_RED   9
#define PIN_GREEN 10
#define PIN_BLUE  11
#define PERIOD_MAX  2550

class RGBController {
  // Variables
  private:
  int R;
  int G;
  int B;

#ifdef  _MULTITHREAD
  private:
  // The light controller thread
  Thread* p_RGBControllerThread;
  static RGBController * itself;
#endif

  // Functions
  public:
  RGBController(){
    this->R = 0;
    this->G = 0;
    this->B = 0;
#ifdef  _MULTITHREAD
    this->initThread();
    this->itself = this;
#endif
  };
  
#ifdef  _MULTITHREAD
  void initThread(){
    *p_RGBControllerThread = Thread();
    p_RGBControllerThread->onRun(Callback);
    p_RGBControllerThread->setInterval(200);
  };
#endif
  
  void setR(int var_R) {this->R = var_R;};
  void setG(int var_G) {this->G = var_G;};
  void setB(int var_B) {this->B = var_B;};
  int getR(void) {return(this->R);};
  int getG(void) {return(this->G);};
  int getB(void) {return(this->B);};
  void setColour(){   
    analogWrite(PIN_RED, this->R);
    analogWrite(PIN_GREEN, this->G);
    analogWrite(PIN_BLUE, this->B);
  };

#ifdef  _MULTITHREAD
  private:
  static void Callback(){
    if(NULL != RGBController::itself){
      RGBController::itself->setColour();
    }
  };

#endif

  public:
  void setColour(int var_R, int var_G, int var_B){
    this->R = var_R;
    this->G = var_G;
    this->B = var_B;
    analogWrite(PIN_RED, this->R);
    analogWrite(PIN_GREEN, this->G);
    analogWrite(PIN_BLUE, this->B);
  };
};


// Global variables
RGBController myRGBController = RGBController();

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
#ifdef  _DEBUG
  Serial.begin(115200);
#endif
}

void loop() {
  // put your main code here, to run repeatedly:
  myRGBController.setColour(0, 0, 0);

//  int rawVoltage = map(analogRead(A0), 0, 1023, 0, 255);
  int rawVoltage = analogRead(A0)/4;
  int intensity = 255 - rawVoltage;
  intensity = (intensity < 5) ? 5 : intensity;
  
//  float voltage = value * (5.0/1023);
#ifdef  _DEBUG
  Serial.println(rawVoltage);
  Serial.println(intensity);
  delay(200);
#endif

  rainbowGenerator(intensity, 1, PERIOD_MAX/66);
}

void rainbowGenerator(int Intensity, int Steps, int Speed){
  Intensity = (Intensity > 255) ? 255 : Intensity;
  Steps = (Steps > Intensity) ? Intensity : Steps;
  Speed = (Speed > 255) ? 255 : Speed;
  int Period = PERIOD_MAX / Speed;
  int i=0;

  // R
  for(i=0; i<Intensity; i+=Steps){
    myRGBController.setColour(i, 0, 0);
    delay(Period);
  }
  // R+G
  for(i=0; i<Intensity; i+=Steps){
    myRGBController.setColour(Intensity, i, 0);
    delay(Period);
  }
  // G
   for(i=0; i<Intensity; i+=Steps){
    myRGBController.setColour(Intensity-i, Intensity, 0);
    delay(Period);
  }
  // G+B
  for(i=0; i<Intensity; i+=Steps){
    myRGBController.setColour(0, Intensity, i);
    delay(Period);
  }
  // B
  for(i=0; i<Intensity; i+=Steps){
    myRGBController.setColour(0, Intensity-i, Intensity);
    delay(Period);
  }
  // B+R
  for(i=0; i<Intensity; i+=Steps){
    myRGBController.setColour(i, 0, Intensity);
    delay(Period);
  }
  // R+B+G
  for(i=0; i<Intensity; i+=Steps){
    myRGBController.setColour(Intensity, i, Intensity);
    delay(Period);
  }
  //Fade
  for(i=0; i<Intensity; i+=Steps){
    myRGBController.setColour(Intensity-i, Intensity-i, Intensity-i);
    delay(Period);
  }  
}
