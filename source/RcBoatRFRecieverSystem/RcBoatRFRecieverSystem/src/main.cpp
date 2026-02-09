#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

/*
Author: Colton Paul Badock
Date: Feb 2026
Version: 6

DESCRIPTION:
A system to control the boat via recieved radio freqencies.
This system is the controller and will send inputs transmitted at 433 MHz to the RC-Boat.
There will be a onboard recieve picking up these inputs, it likely will not transmit back
*/


//The reciever for the boat.
RH_ASK user_reciever;







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



//Infinite application loop
void loop() {

  /*
  *Test code to see if we can recieve anything
  *from our arduino on 433 mhz
  **/
  uint8_t buf[12];
  uint8_t buflen = sizeof(buf);
  if (user_reciever.recv(buf, &buflen)) {
    int i;
    // Message with a good checksum received, dump it.
    Serial.print("Message: ");
    Serial.println((char*)buf);
  }
}