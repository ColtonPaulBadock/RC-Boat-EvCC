/*
* Onboard controller system for the RC-Boat. This system depends on RcBoatRFControllerSystem
* Or another system that can send 433MHz commands recieved by this RC-Boat.
* Motor, Light and 433MHz receptions and all onboard systems run here.
**/

/**
 * AUTHORS:
 * Colton Paul Badock (EvCC) - 433MHz radio communication/transcieveing system; RC-Boat controller; RC-Body design; Repo manager
 * Jackson Black (EvCC) - Lighting system; Lighting mounts
 * Julio Martinez (EvCC) - Water/Flood detection system
 * Nolan McGuire (EvCC) - Motor Controller System; Motor Mounts; RC-Body design; RC-Boat fins
 * 
 * 3RD-PARTY:
 * Mike McCauley - RH_ASK (Radio Head) library
 * 
*/

//All libraries and dependencies as needed
#include <Arduino.h> //Default Arduino infastructure
#include <RH_ASK.h> //Radio Head library for 433MHz communication and lowlevel communication to transcieveing hardware
#include <SPI.h> //Required  dependency for <RH_ASK.h> (Radio Head) to compile, not ran onboard.


//The reciever for the boat.
//Wire reciever into port "digital 11"
RH_ASK user_reciever;

const byte AIN1 = 8;
const byte AIN2 = 9;
const byte PWMA = 10;

//Left Motor
const byte BIN1 = 7;
const byte BIN2 = 6;
const byte PWMB = 5;
  
 //button pin names/locations
const byte RedB = 2;
const byte BlkB = 3;

//motor velocity
unsigned int spd;

//potentiometer pin 
int phot = 0;


//Intializes the motors
void initMotors() {
  //set button pins to input pullup
  pinMode(RedB, INPUT_PULLUP);
  pinMode(BlkB, INPUT_PULLUP);
  
  //set motor pins to output
  //Right motor
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  //Left motor
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
}



//The following functions are loosely copied from the google doc linked above. 
//I have included comments in my own words to demonstrate (an imperfect) understanding. 

void Rmotor(int motorSpeed)
{
    if (motorSpeed > 0)           //If we want the motor to run forward, it will:
    {
      digitalWrite(AIN1, HIGH);   //essentially, we tell AO1 to be positive
      digitalWrite(AIN2, LOW);    //and AO2 will be ground. 
      //Serial.println("R 1st gear");//The direction of the flow of electrons changes the
                                  //the direction of rotation. 
    }
    
    else if (motorSpeed < 0)      //If we want it to go backwards, it will:
    {
      digitalWrite(AIN1, LOW);    //Now we use AO1 as ground 
      digitalWrite(AIN2, HIGH);   //and AO2 as positive. 
      //Serial.println("R reverse");//The input voltage to AIN1 and AIN2 determins which 
                                  //output pin (AO1 and AO2) will be ground or positive.
    }

    else                          //If we want to go nowhere:
    {
      digitalWrite(AIN1, LOW);    //We basically have no positive, only ground. 
      digitalWrite(AIN2, LOW);
      //Serial.println("R neutral");
    }

    analogWrite(PWMA, abs(motorSpeed)); //This line takes the absolute value of the motor 
    //Serial.print("RThrottle ");         //speed, and sends that through a PWM pin. So a 
    //Serial.println(abs(motorSpeed));    //PWM pin basically has a variable voltage output?
}
//the if statements act as a sort of digital transmission, while the PWM pins are the 
//throttle. 

void Lmotor(int motorSpeed)
{
    if (motorSpeed > 0)           //If we want the motor to run forward, it will:
    {
      digitalWrite(BIN1, HIGH);   //essentially, we tell AO1 to be positive
      digitalWrite(BIN2, LOW);    //and AO2 will be ground. 
      //Serial.println("L 1st gear");//The direction of the flow of electrons changes the
                                  //the direction of rotation. 
    }
    
    else if (motorSpeed < 0)      //If we want it to go backwards, it will:
    {
      digitalWrite(BIN1, LOW);    //Now we use AO1 as ground 
      digitalWrite(BIN2, HIGH);   //and AO2 as positive. 
      //Serial.println("L reverse");//The input voltage to AIN1 and AIN2 determins which 
                                  //output pin (AO1 and AO2) will be ground or positive.
    }

    else                          //If we want to go nowhere:
    {
      digitalWrite(BIN1, LOW);    //We basically have no positive, only ground. 
      digitalWrite(BIN2, LOW);
      //Serial.println("L neutral");
    }

    analogWrite(PWMB, abs(motorSpeed)); //This line takes the absolute value of the motor 
    //Serial.print("LThrottle ");         //speed, and sends that through a PWM pin as a 
    //Serial.println(abs(motorSpeed));    //voltage. This acts as the throttle. 
}



//Intializes the boat
void setup() {
  //Start serial communication
  //back to the PC (if plugged in)
  //for debugging
  Serial.begin(9600);
  Serial.println("Started; starting RC");


  //Intialize all motors
  initMotors();

  //If the reciever failed to intialize,
  //throw an error back to serial
  if (!user_reciever.init()) {
    Serial.println("Reciever failed to intialize; Error Code: 01");
  }
}


//AUTHOR: Colton Paul Badock
//Returns the latest command/transmittion
//recieved from the 433MHz microwave controller
//for the RC-Boat.
//----------------------
//RETURNS:
//
//If message recieved -> Returns the msg/command
//
//If no message recieved -> returns "NOMSG"
//
char* recieveCommand() {

  //Buffer to hold message recieved in an
  //array
  uint8_t buf[12];
  uint8_t buflen = sizeof(buf);


  //If we recieve a message with a good checksum,
  //we will return it as reception here
  if (user_reciever.recv(buf, &buflen)) {
    
    //Unused?
    int i;

    //Print the message we recieved for debug
    //Serial.print("433MHz msg recv: ");
    //Serial.println((char*)buf);

    //Return the message/command
    //recieved by the 433MHz reciever
    return (char*)buf;
  
  //If no message was recieved, we return
  //"NOMSG"
  } else {
    return "NOMSG";
  }
}



//The message/command
//picked up from the 433MHz transmitter
//by the 433MHz reciever each cycle
//in "loop"
char *msg = "NOMS";


//Boolean status of whats running
//------
//Motor run values. If 0, motor
//not running, if 1 motor is running
bool leftMotorRunning = 0;
bool rightMotorRunning = 0;



//infinite application loop
void loop() {
  
  //Get the current transmitted
  //command. This will be "NOMSG"
  //if nothing was recieved
  //msg = "NOMSG";//recieveCommand();

  //if (msg != "NOMS") {
  //  Serial.print("Message: ");
  //  Serial.println(msg);
  //}

  /* 
  * Check what message/command we recieved from the
  * main transmitter, if its a command, set motor/light boolean
  * operation status to true or false, we will then  set
  * the operation later in the loop.
  *
  * **/

  
  //Evaluate the left motor commands,
  //based on what we recieved as a command,
  //we will set the motors on or off as
  //a bool status
  if (msg == "LMRA") {
    leftMotorRunning = 1;
    Serial.println("Started left motor!");
  } else if (msg == "LMKA") {
    leftMotorRunning = 0;
  }

  //Evaluate the right motor commands,
  //based on what we recieved as a command,
  //we will set the motors on or off as
  //a bool status
  if (msg == "RMRA") {
    rightMotorRunning = 1;
  } else if (msg == "RMKA") {
    rightMotorRunning = 0;
  }
  

  /* 
  * Based on the command/boolean status we have
  * we will set components operations here 
  ** */
  
  //Run/Kill the left motor based
  //on the boolean status of the motor
  //as to wether it is running or not.
  if (leftMotorRunning == 1) {
    Lmotor(255);
  } else {
    Lmotor(0);
  }

  //Run/Kill the right motor based
  //on the boolean status of the motor
  //as to wether it is running or not.
  if (rightMotorRunning == 1) {
    Rmotor(255);
  } else {
    Rmotor(0);
  }


  /*
  Check the transmitter for a message.
  If we recieved a message, store it in "msg" char buffer (string)
  to be interpreted as a command later.
  If we recieve no message, we will set msg to "NOMSG" command,
  so we will execute no onboard commands on the boat.
  */
  uint8_t buf[5]; //Original size: 12. 
  uint8_t buflen = sizeof(buf);
  if (user_reciever.recv(buf, &buflen)) // Non-blocking
  {
    int i;
    // Message with a good checksum received, dump it.
    Serial.print("Message: ");
    //Serial.println((char*)buf);
    msg = (char*)buf;
    msg[5] = '\0';
    Serial.println(msg);
  } else {
    msg = "NOMSG";
  }


}