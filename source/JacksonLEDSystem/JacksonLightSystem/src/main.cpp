#include <Servo.h>
#include <Arduino.h>

// Project 2 - Interactive Electronic
// Steering Responsive Lights

// Version 8.0 - This version uses a button to toggle the lights 
// on and off

// global variables
const byte leftB = 5;
const byte rightB= 4;
const byte lightB = 8;

// creating button state variables
bool stateL = digitalRead(leftB);
bool stateR = digitalRead(rightB);
bool currentbuttonState;

Servo serL; // naming the servo
Servo serR; 

byte positionL = 90; // setting default servo position
byte positionR = 90;

const byte leda = 3; // led variables
const byte ledb = 2;

bool ledstate = LOW;
bool lastbuttonState = HIGH;



byte lightsOnOff()
{
 if(currentbuttonState == LOW && lastbuttonState == HIGH)
 {
  ledstate = !ledstate;
  digitalWrite(leda, ledstate);
  digitalWrite(ledb, ledstate);
  delay(50);
  }
  
  lastbuttonState = currentbuttonState;
  
  return(lastbuttonState);
  return(currentbuttonState);
  return(ledstate);
}
  
byte TurnLeft()
{
 while (stateL == 0 && stateR == 1 && positionL < 135 && positionR < 135)
 {
   positionL++;
   positionR++;
   serL.write(positionL);
   serR.write(positionR);
   
   stateL = digitalRead(leftB);
   stateR = digitalRead(rightB);
   positionL = serL.read();
   positionR = serR.read();
   
   return(positionL);
   return(positionR);
 }
}

byte TurnRight()
{
 while(stateR == 0 && stateL == 1 && positionR > 45 && positionL > 45) 
 {
  positionR--;
  positionL--;
  serL.write(positionL);
  serR.write(positionR);
   
  stateR = digitalRead(rightB);
  stateL = digitalRead(leftB);
  positionL = serL.read();
  positionR = serR.read();
   
  return(positionL);
  return(positionR);
 }
}

byte goStraightL()
{
 while(stateL == 1 && stateR == 1 && positionL > 90 && positionR > 90)
 {
  positionL--;
  positionR--;
  serL.write(positionL);
  serR.write(positionR);
   
  stateL = digitalRead(leftB);
  stateR = digitalRead(rightB);
  positionL = serL.read();
  positionR = serR.read();
   
  return(positionL);
  return(positionR);
 }
}

byte goStraightR()
{
 while(stateL == 1 && stateR == 1 && positionL < 90 && positionR < 90)
 {
  positionL++;
  positionR++;
  serL.write(positionL);
  serR.write(positionR);
   
  stateL = digitalRead(leftB);
  stateR = digitalRead(rightB);
  positionL = serL.read();
  positionR = serR.read();
   
  return(positionL);
  return(positionR);
 }
}

byte Straight()
{
 while(stateL == 0 && stateR == 0) 
 {
  positionL = 90;
  positionR = 90;
  serL.write(positionL);
  serR.write(positionR);
   
  stateL = digitalRead(leftB);
  stateR = digitalRead(rightB);
  
  return(positionL);
  return(positionR);
 }
}



void setup()
{
  pinMode(leftB, INPUT_PULLUP); // activating button digital pins
  pinMode(rightB, INPUT_PULLUP);
  pinMode(lightB, INPUT_PULLUP);
  pinMode(leda, OUTPUT); // activiating led digital pins
  pinMode(ledb, OUTPUT);
  
  serL.attach(12); // attaching servos to digital pin
  serR.attach(13);
  
  Serial.begin(9600);
}

void loop()
{
  


  serL.write(positionL); // setting the servos initial position
  serR.write(positionR);
  
  // Debugging
  lightsOnOff();

  //Serial.println(stateL);
  //Serial.println(stateR);
  //Serial.println(positionL);
  //Serial.println(lightB);
  
  // light servo moving functions
  //goStraightL();
  //goStraightR();
  TurnLeft();
  TurnRight();
  
}