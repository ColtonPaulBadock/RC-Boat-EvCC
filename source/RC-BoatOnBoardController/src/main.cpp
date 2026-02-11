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

//Intializes the boat
void setup() {
  //Start serial communication
  //back to the PC (if plugged in)
  //for debugging
  Serial.begin(9600);

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
    Serial.print("433MHz msg recv: ");
    Serial.println((char*)buf);

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
const char *msg;


//infinite application loop
void loop() {
  
  //Get the current transmitted
  //command. This will be "NOMSG"
  //if nothing was recieved
  msg = recieveCommand();

  

}