/* ===================================================================================================
    Servo Motor - Stewart Platform Actuation
    Latest: 2019.11.20

    This example provides a starting point for students using an Arduino controller and Adafruit PWM
    Servo Shield to actuate six servo motors. This examples illustrates the following key points:
      1. Setting up and use of the "Adafruit_PWMServoDriver" library
      2. Use of Arduino and C++ commands and syntax:
         e.g. include-statements, constants, integers, strings, functions, arrays, serial
              communications, loops, if-statements, switch-statements, etc.
      3. Operational Summary:
          a. Configure six motors with the same min and max value (students to update)
          b. Using basic serial input/output (stings) to specific motor actuation (students to update)
             Current inputs are:
               i. Input [1,2,3,4,5,6] -> increase respective (1..6) values
              ii. Input [q,w,e,r,t,y] -> decrease respective (1..6) values
             iii. Input [m,>,<,-,+  ] -> adjust all values

    NOTES:
      1. To install the Adafruit Library:
          a. Use the Arduino library manager select the Tools menu, then Manage Libraries.
          b. Search for and install the "Adafruit PWM Servo Driver Library"
      2. Depending on your servo motor, the pulse width (set using 'tick' values) min and max will be
         different for each motor. Students should use unique values (not include here).
      3. The PWM Servo Shield must be provide 5V to the terminal block (V+ and GND are labeled on the board).
         The board is working when both the red and green LEDs are illuminated.
      4. The provided servo motors have a three wire connection that matches the labels shown on
         the PWM Servo Shield: (GND = black/brown wire, V+ = red wire, and PWM = yellow wire).
      5. Additional information on the Adafruit 16 channel PWM shield can be found:
         https://learn.adafruit.com/adafruit-16-channel-pwm-slash-servo-shield/using-the-adafruit-library


    The software is provided "as is", and intended for educational purposes only. This program is
    distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   =================================================================================================== */

// 580
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

const int SERVOMIN = 100; // minimum 'tick' value between 0..4095 -> used in setPWM(pin, on-tick, off-tick)
const int SERVOMAX = 500; // maximum 'tick' value between 0..4095 -> used in setPWM(pin, on-tick, off-tick)
const int SERVOMID = floor((SERVOMAX + SERVOMIN) / 2); // mid 'tick' value
const int SERVOCHG = 10; // 'change' value

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

String valInput; // Serial input var.
int val[6] = {SERVOMID, SERVOMID, SERVOMID, SERVOMID, SERVOMID, SERVOMID}; // PWM var


// =================================================================================================== //
void setup()
{
  Serial.begin(9600); // opens serial port, sets data rate [bps]
  Serial.setTimeout(10); // change default (1000ms) **for a faster response time**
  
  Serial.println("\nServo Motor - Stewart Platform Actuation");
  Serial.println("ALI SUX");
  Serial.println("Enter command: [# from 1 to 6], [q,w,e,r,t,y], or [<, m, >, +, -]");

  pwm.begin();
  pwm.setPWMFreq(50); // set sero Frequency [Hz]
  for (int i = 0; i < 16; i++) pwm.setPWM(i, 0, 0); // Set all pins to 'off', motors can freely move
}


// =================================================================================================== //
void loop()
{
  if (Serial.available() > 0)
  { // if received serial input
    valInput = Serial.readString();
    Serial.print(valInput); // display what was received

    switch (valInput[0]) { // checking first char in sting provided

      // _____________________________________________________ Adjust servo motors 1
      case '1':
        val[0] = min( val[0] + SERVOCHG, SERVOMAX );
        break;
      case 'q':
        val[0] = max( val[0] - SERVOCHG, SERVOMIN );
        break;

      // _____________________________________________________ Adjust servo motors 2
      case '2':
        val[1] = min( val[1] + SERVOCHG, SERVOMAX );
        break;
      case 'w':
        val[1] = max( val[1] - SERVOCHG, SERVOMIN );
        break;

      // _____________________________________________________ Adjust servo motors 3
      case '3':
        val[2] = min( val[2] + SERVOCHG, SERVOMAX );
        break;
      case 'e':
        val[2] = max( val[2] - SERVOCHG, SERVOMIN );
        break;

      // _____________________________________________________ Adjust servo motors 4
      case '4':
        val[3] = min( val[3] + SERVOCHG, SERVOMAX );
        break;
      case 'r':
        val[3] = max( val[3] - SERVOCHG, SERVOMIN );
        break;

      // _____________________________________________________ Adjust servo motors 5
      case '5':
        val[4] = min( val[4] + SERVOCHG, SERVOMAX );
        break;
      case 't':
        val[4] = max( val[4] - SERVOCHG, SERVOMIN );
        break;

      // _____________________________________________________ Adjust servo motors 6
      case '6':
        val[5] = min( val[5] + SERVOCHG, SERVOMAX );
        break;
      case 'y':
        val[5] = max( val[5] - SERVOCHG, SERVOMIN );
        break;

      // _____________________________________________________ Adjust all servo motors
      case '<': // change to min value
        for (int i = 0; i < 6; i++) val[i] = SERVOMIN;
        break;

      case '>': // change to max value
        for (int i = 0; i < 6; i++) val[i] = SERVOMAX;
        break;

      case 'm': // change to mid value
        for (int i = 0; i < 6; i++) val[i] = SERVOMID;
        break;

      case '-': // Reduce all val[]
        for (int i = 0; i < 6; i++) {
          val[i] = max( val[i] - 5, SERVOMIN );
        }
        break;

      case '+': // Increase all val[]
        for (int i = 0; i < 6; i++) {
          val[i] = min( val[i] + 5, SERVOMAX );
        }
        break;

      // _____________________________________________________ Default action
      default: Serial.print(": Command not recognized");

    } // end switch statement

    // Command servos to move to value,
    // Note: offset of +1 used to match pins numbers 1 to 6 on PWM Servo Shield (skipping pin 0)
    for (int i = 0; i < 6; i++) {
      pwm.setPWM(i + 1, 0, val[i]); // setPWM( channel, on-tick(use 0), off-tick )
    }

    // Display current servo value
    Serial.print(": [ ");
    for (int i = 0; i < 6; i++) {
      Serial.print(val[i], DEC);
      Serial.print(" ");
    }
    Serial.println("]");
    
  }// end if-statement: Serial.available() condition
  
}// end loop
