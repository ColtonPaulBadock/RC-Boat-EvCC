#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

/*
Author: Colton Paul Badock
Date: Feb 2026
Version: 9

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
byte leftMotorButton_port = 2;
byte rightMotorButton_port = 3;
byte lightToggleButton_port = 4;
byte directionToggle = A0;


//Allows the user to run a timer
//using the default Arduino timer system.
class ColtonTimerSystem {
  private:

    //This will hold the total time
    //that has passed since the timer
    //started.
    long timerStarted = 0;
  
  public:

    //Starts the timer;
    //Can be called to reset the timer
    void startTimer() {
      //Store the current milliseconds
      //since the board started.
      timerStarted = millis();
    }

    //Returns the time since the timer started
    long getTime() {
      return millis() - timerStarted;
    }
};

//Timer to prevent debounce for the lights toggle
ColtonTimerSystem lightToggleDebounce;


//Transmits a message/command on the
//open air at 433MHz. The message
//should be recieved by the RC-Boat
//and the command will be interprated by
//the boards micro controller
void transmitMessage(const char *msg) {
    
    
    //const char *msg = "Hello World!";
    user_transmitter.send((uint8_t *)msg, strlen(msg));
    user_transmitter.waitPacketSent();

    //Log that we transmitted a message
    Serial.print("Message transmitted: ");
    Serial.println(msg);
}


//Booleans for what we have already transmitted,
//this serves to save bandwidth on 433MHz.
//If "transmittedLEFT_MOTOR_RUN" is true,
//we don't need to transmit telling it to
//run the forward motor again since
//we already did, and vice-versa for all
//other motors here.
bool transmittedLEFT_MOTOR_RUN = 0;
bool transmittedRIGHT_MOTOR_RUN = 0;
bool directionIsForward = 0;


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
    //If the left motor button is pressed and we have
    //not yet told the boat we are running the left motor, transmit
    //saying we are running the left motor, then we will not tell
    //it to kill the motor until we let go of the left motor button
    if (digitalRead(leftMotorButton_port) == 0 && transmittedLEFT_MOTOR_RUN == 0) {
        transmitMessage("LEFT_MOTOR_RUN");
        transmittedLEFT_MOTOR_RUN = 1;
    
    //If the left motor button is not pressed,
    //anymore or at all, we will tell the boat to
    //kill the left motor one time until we hold the button again
    } else if (digitalRead(leftMotorButton_port) == 1) {
        if (transmittedLEFT_MOTOR_RUN == 1) {
            transmitMessage("LEFT_MOTOR_KILL");
        }
        transmittedLEFT_MOTOR_RUN = 0;
    }
    //If the right motor button is pressed, we will tell the
    //RC boat to run the right motor one time,
    //we will then tell it to kill the motor once we let go.
    if (digitalRead(rightMotorButton_port) == 0 && transmittedRIGHT_MOTOR_RUN == 0) {
        transmitMessage("RIGHT_MOTOR_RUN");
        transmittedRIGHT_MOTOR_RUN = 1;
    //If the right motor button is released,
    //we will kill the motor and do nothing
    //till its pressed again for this statement.
    } else if (digitalRead(rightMotorButton_port) == 1) {
        if (transmittedRIGHT_MOTOR_RUN == 1) {
            transmitMessage("RIGHT_MOTOR_KILL");
        }
        transmittedRIGHT_MOTOR_RUN = 0;
    }
    //If the light toggle is pressed,
    //toggle the lights
    if (digitalRead(lightToggleButton_port) == 0 && lightToggleDebounce.getTime() > 500) {
        transmitMessage("TOGGLE_LIGHTS");
        lightToggleDebounce.startTimer();
    }
    //If the analog reverse switch is above
    //has a specific resistance below half,
    //then toggle to reverse or forward
    if (analogRead(directionToggle) > 500) {
        //If we already told the boat to go forward,
        //we will not transmit to go forward anymore,
        //until we switch back from reverse again
        if (directionIsForward == 0) {
            transmitMessage("ACTIVATE_FORWARD");
        }
        directionIsForward = 1;
    } else {
        //If we already told the boat to reverse,
        //we will not transmit to reverse anymore,
        //until we switch back from forward again
        if (directionIsForward == 1) {
            transmitMessage("ACTIVATE_REVERSE");
        }
        directionIsForward = 0;
    }
}


//Init function (constructor)
void setup() {
  
    Serial.begin(9600);

    //Setup digital and analog pins
    //based on how we want to use them
    //for controller inputs
    pinMode(leftMotorButton_port, INPUT_PULLUP);
    pinMode(rightMotorButton_port, INPUT_PULLUP);
    pinMode(lightToggleButton_port, INPUT_PULLUP);
    pinMode(directionToggle, INPUT_PULLUP);

    //Start debounce timers or timers if needed
    lightToggleDebounce.startTimer();


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

    //Serial.println(digitalRead(leftMotorButton_port));

    //Allow the user to transmit
    //to the RC Boat with the
    //controller inputs they press.
    transmitCommands();

    //DEBUG PRINT BUTTON
    //Serial.println(digitalRead(leftMotorButton_port));
}