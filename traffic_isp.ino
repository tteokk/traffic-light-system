/*Created By: Ajilan Sivaloganathan, Elizabeth Shim, Justin Freemantle
 * Date : 2018-06-01
 * Mr.Wong
 * Eningeering Grade 11 ISP - Fully Functioning Traffic Light System
 */
 
#include <Servo.h>

Servo myservo;
bool gate = false;
int pos;
unsigned long interval=2000; 
unsigned long previousMillis=0; 
unsigned long servoMillis = 0;
int buttonPin=1; 
bool buttonPressed=false; // stores whether the button was pressed or not
int buttonState=LOW; // stores current button state
int ryState = LOW; // red ligh bulb for traffic light pair
int yyState = LOW; // yellow light bulb for traffic light pair
int gyState = HIGH; // green ligh bulb for traffic light pair
int rxState = HIGH; // red ligh bulb for centred traffic light
int yxState = LOW; // yellow ligh bulb for centred traffic light
int gxState = LOW; // green ligh bulb for centred traffic light
int RpedState = HIGH; // Red light bulb for pedestrian light
int GpedState = LOW; // Green light bulb for pedestrian light
const int streetPIN=5; //used for street ligths
int lightSensitivity;// Stores photoresistor values
const int pResistor = A0; // Photoresistor at Arduino analog pin A0
int count = 0; // used for which traffic lights to use
int irValue; // stores value for the IR sensor


void setup() 
{
  pinMode(buttonPin, INPUT);//Takes button input
  //the following lines are for the traffic light bulbs
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(streetPIN, OUTPUT); //for street lights
  pinMode(pResistor, INPUT); //input for photoresistor 
  pinMode(A1,INPUT);
  Serial.begin (9600);
  myservo.attach (3); // outputs servo to pin 3 
}
 
void street()
{
  lightSensitivity = analogRead(pResistor);
  if (lightSensitivity > 200){
    digitalWrite(streetPIN, LOW);  //Turn led off when its light outside
  }else{
    digitalWrite(streetPIN, HIGH); //Turn led on when its darker
  }
}

void loop() { 
  unsigned long currentMillis = millis(); // grab current time
  street();
  buttonState = digitalRead(buttonPin);
  if (buttonState==HIGH && gxState==LOW)
    buttonPressed=true;
  //sets all the light bulbs
  digitalWrite(13, ryState);
  digitalWrite(12, yyState);
  digitalWrite(11, gyState);
  digitalWrite(10, rxState);
  digitalWrite(9, yxState);
  digitalWrite(8, gxState);
  digitalWrite(7,GpedState);
  digitalWrite(6,RpedState);
  if ((unsigned long)(currentMillis - previousMillis) >= interval) //This if statement is responsible for changing the states of the traffic lights
  {
    if(rxState==HIGH && gyState==HIGH)
    {
      yyState=HIGH;
      gyState=LOW;
    }
    else if(yyState==HIGH && rxState==HIGH)
    {
      yyState=LOW;
      ryState=HIGH;
    }
    else if(ryState==HIGH && rxState==HIGH && count==0)
    {
      rxState=LOW;
      gxState=HIGH;
      GpedState=HIGH;
      RpedState=LOW;
      count++;
    }
    else if(ryState==HIGH && gxState==HIGH)
    {
      gxState=LOW;
      yxState=HIGH;
      GpedState=LOW;
      RpedState=HIGH;
    }
    else if (yxState==HIGH && ryState==HIGH)
    {
      rxState=HIGH;
      yxState=LOW;
    }
    else if (rxState==HIGH && ryState==HIGH && count ==1)
    {
      ryState=LOW;
      gyState=HIGH;
      rxState=HIGH;
      count--;
    }
    if ((yyState==HIGH || yxState==HIGH) || (rxState==HIGH && ryState==HIGH)) // this if statements determines for how long each light is on for
    {
      interval=3000;//Yellow lights are on for one second and both red lights are on for one second
      if(buttonPressed==true) //time is reduced if button is pressed
        interval=1500;
    }
    else if (gyState==HIGH)
    {
     interval=6000;//green and single red lights are on for two seconds
     if (buttonPressed==true)//time is reduced if button is pressed
      interval=3000;
    }
    else if (gxState==HIGH)
    { 
      interval=6000;//green lights are on for two seconds
      buttonPressed=false; //records that the pedestrian may now cross
    }
  previousMillis = currentMillis;
  } 

  irValue = (analogRead (A1)); // gets ir sensor value
  //Serial.println (analogRead (A1));
  // if there is an object blocking the gate and the gate is closed
  if(irValue <100 && gate == false && (currentMillis - servoMillis) >= 3000){
    
    //gate closes
    for (pos = 0; pos < 180; pos ++);
    {
      myservo.write (pos);
    }
    
    gate = true; // the gate is open
  }

  //if the path is clear and the gate is open and 3 seconds have elapsed since the gate opened
  if (irValue > 100 && gate == true){

    //closes gate
    for (pos = 180; pos > 0; pos --);
    {
      myservo.write (pos);
    }
    gate = false;  // gate is closed
    
    servoMillis = millis (); // starts timer for servo
  }
}
