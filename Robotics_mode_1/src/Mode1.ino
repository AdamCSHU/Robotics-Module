#include <Wire.h>
#include <Zumo32U4.h>

/*
work on code for robotics mode 1 through weekend. Start mode 3 on saturday and mode 2 as well.
//mode 1 is fully manual, make sure to slow down as proxmity nears person/survivor. 
*/
//proximity code inspired by https://github.com/pololu/zumo-32u4-arduino-library/blob/master/examples/SumoProximitySensors/SumoProximitySensors.ino

Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;

uint16_t proxRightFrontActive, proxLeftFrontActive, proxLeftActive, proxRightActive;
const int proxTolerance = 5;

char action;
int speed = 100;

unsigned int lineSensorValues[3];

//mode 1.ino file, contains all code related to zumo mode 1 task including proximity which sends a message to the serial console if an object is detected on either side (left/right).
void setup()
{
  Serial.begin(9600);
  ledGreen(1);
  proxSensors.initThreeSensors();
}

/*
//code inspired by https://github.com/pvcraven/zumo_32u4_examples
void printToSerial()
{
  static char buffer[80];
  sprintf(buffer, "Person detected at co-ordinates: \n%d %d %d %d %d %d  %d %d %d  %4d %4d %4d %4d %4d\n",
  proxSensors.countsLeftWithLeftLeds(),
  proxSensors.countsLeftWithRightLeds(),
  proxSensors.countsFrontWithLeftLeds(),
  proxSensors.countsFrontWithRightLeds(),
  proxSensors.countsRightWithLeftLeds(),
  proxSensors.countsRightWithRightLeds(),
proxLeftActive,
proxRightActive,
proxFrontActive
  );
  Serial.print(buffer);
}
*/

void loop()
{
  if (Serial.available() > 0) 
  { 
          static uint16_t lastTime = 0;
    if ((uint16_t)(millis()- lastTime) >= 100)
    {

      proxSensors.read();
      
        proxLeftFrontActive = proxSensors.countsFrontWithLeftLeds();
        proxRightFrontActive = proxSensors.countsFrontWithRightLeds();
        proxRightActive = proxSensors.countsRightWithRightLeds();
        proxLeftActive = proxSensors.countsLeftWithLeftLeds();
       
      if (proxLeftActive >= proxTolerance){
        Serial.print("person detected to the left");
        delay(1000);
        motors.setSpeeds(50, 50);
      } else if (proxRightActive >= proxTolerance){
        Serial.print("person detected to the right");
        delay(1000);
        motors.setSpeeds(50, 50);
      }
    }

    action = Serial.read();  

      switch (action)
      {
      case 'w':
        // go forwards
        ledYellow(1);
        ledRed(0);
        ledGreen(0);
        motors.setSpeeds (speed, speed);
        delay(100);
        Serial.write("You are moving forwards");
        break;
      case 's':
        // go backwards
        ledYellow(0);
        ledRed(0);
        ledGreen(1);
        motors.setSpeeds(speed*-1, speed*-1);
        delay(100);
        Serial.write("You are moving backwards");
        break;  
      case 'a':
        // turn left
        ledRed(1);
        ledGreen(0);
        ledYellow(0);
        motors.setLeftSpeed(0);
        motors.setRightSpeed(speed);
        delay(100);
        Serial.write("You are moving to the left");
        break;   
      case 'd':
        // turn right
        ledRed(1);
        ledGreen(0);
        ledYellow(0);
        motors.setLeftSpeed(speed);
        motors.setRightSpeed(0);
        delay(100);
        Serial.write("You are moving to the right");
        break;      
      default:
        motors.setLeftSpeed(0);
        motors.setRightSpeed(0);
        ledYellow(0);
        ledRed(0);
        ledGreen(0);
        delay(100);
    }

    delay(500);
   }
  
}
  
void calibrateSensors(){

}
 
