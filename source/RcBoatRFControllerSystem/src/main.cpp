#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

/*
Author: Colton Paul Badock
Date: Feb 2026
Version: 5

DESCRIPTION:
A system to control the boat via recieved radio freqencies.
This system is the controller and will send inputs transmitted at 433 MHz to the RC-Boat.
There will be a onboard recieve picking up these inputs, it likely will not transmit back
*/


//The transmitter for the user.
//This transmitter system will enable 433 MHz
//communication from the users controller
//to the onboard reciever, allowing
//for wireless control of the boat
RH_ASK user_transmitter;



//Transmits a message/command on the
//open air at 433MHz. The message
//should be recieved by the RC-Boat
//and the command will be interprated by
//the boards micro controller
void transmitMessage(char *msg) {
    
    
    //const char *msg = "Hello World!";
    user_transmitter.send((uint8_t *)msg, strlen(msg));
    user_transmitter.waitPacketSent();
}



//Init function (constructor)
void setup() {
  
    //Start serial communication
    //back to the PC (if plugged in)
    //for debugging
    Serial.begin(9600);

    //If the transmitter failed to intialize,
    //throw an error back to serial
    if (!user_transmitter.init()) {
        Serial.println("Transmitter failed to intialize; Error Code: 01");
    }

}


//Infinite application loop
void loop() {
  
    transmitMessage("Hello World!");

    delay(5000);

    Serial.println("Successfully transmitted??");
}