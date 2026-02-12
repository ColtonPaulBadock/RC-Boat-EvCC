#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

/*
Author: Colton Paul Badock
Date: Feb 2026
Version: 11

DESCRIPTION:
A system to control the boat via recieved radio freqencies.
This system is the controller and will send inputs transmitted at 433 MHz to the RC-Boat.
There will be a onboard recieve picking up these inputs, it likely will not transmit back
*/


//The reciever for the boat.
RH_ASK user_reciever;



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
  uint8_t buf[12]; //Original: 12 bytes
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



//Init function (constructor)
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

char* msg;

//Infinite application loop
void loop() {
  
  msg = recieveCommand();

  if (msg != "NOMSG") {
    Serial.print("Took this message: ");
    Serial.println(msg);
  }

}