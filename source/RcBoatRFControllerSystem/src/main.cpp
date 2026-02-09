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


//The transmitter for the user.
//This transmitter system will enable 433 MHz
//communication from the users controller
//to the onboard reciever, allowing
//for wireless control of the boat
RH_ASK user_transmitter;

//INPUT DEVICES
byte leftTurnButton_port = 2;
byte rightTurnButton_port = 3;
byte lightToggleButton_port = 4;
byte directionToggle = A0;


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
  
    //Setup digital and analog pins
    //based on how we want to use them
    //for controller inputs
    pinMode(leftTurnButton_port, INPUT);
    pinMode(rightTurnButton_port, INPUT);
    pinMode(lightToggleButton_port, INPUT);
    pinMode(directionToggle, INPUT_PULLUP);



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

    //Allow the user to transmit
    //to the RC Boat with the
    //controller inputs they press.
    transmitCommands();

}



//Takes all inputs from the user on the
//controller that could have occured this cycle,
//then transmits them on the frequency
//for the RC Boat to detect.
void transmitCommands() {

    //Check for given input parameters,
    //such as button presses or specific
    //control/sensor values.
    //If these values are meet, then we
    //will transmit a command to the RC
    //boat realted to the control pressed
    //-------
    //If the left turn button is pressed,
    //tell the boat to power the left motor
    if (digitalRead(leftTurnButton_port) == 1) {
        transmitMessage("LEFT_MOTOR_RUN");
    } else {
        transmitMessage("LEFT_MOTOR_KILL");
    }
    //If the right turn button is pressed,
    //tell the boat to power the right motor
    if (digitalRead(rightTurnButton_port) == 1) {
        transmitMessage("RIGHT_MOTOR_RUN");
    } else {
        transmitMessage("RIGHT_MOTOR_KILL");
    }
    //If the light toggle is pressed,
    //toggle the lights
    if (digitalRead(lightToggleButton_port) == 1) {
        transmitMessage("TOGGLE_LIGHTS");
    }
    //If the analog reverse switch is above
    //has a specific resistance below half,
    //then toggle to reverse or forward
    if (analogRead(directionToggle) > 500) {
        transmitMessage("ACTIVATE_FORWARD");
    } else {
        transmitMessage("ACTIVATE_REVERSE");
    }
}