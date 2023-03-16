#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4Buzzer buzzer;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;

#define numSensors 5
unsigned int lineSensorValues[numSensors];

char action;
uint16_t speed = 200;
uint16_t max = 400;
uint16_t min = 210;
int cornerSpeed = 150;

static bool corner = false;
static bool rightObj = false;
static bool leftObj = false;
static bool Detected = false; 

/*
mode 2 for zumo robot, semi autonomous. Using line sensors along with proximity sensors to keep zumo inside of the lines, if a turn is detected, zumo will switch to manual controls for the user to make the turn, once the turn is complete
zumo will continue in autonomous mode.
*/
void calibrateSensors()
{
  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  for(uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200);
    }
    else
    {
      motors.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  lineSensors.initThreeSensors();
  proxSensors.initThreeSensors();

  calibrateSensors();

}

void mode3(){
  motors.setRightSpeed(speed);
  motors.setLeftSpeed(speed);

  if (lineSensorValues[0] > max){
    motors.setLeftSpeed(0);
    motors.setRightSpeed(0);
    delay(100);

    motors.setLeftSpeed(speed);
    motors.setRightSpeed(-speed);
    delay(100);
  }

  if (lineSensorValues[2] > max){
    motors.setLeftSpeed(0);
    motors.setRightSpeed(0);
    delay(100);

    motors.setLeftSpeed(-speed);
    motors.setRightSpeed(speed);
    delay(100);
  }
}

void mode1(){
  motors.setSpeeds(0, 0);

    if (Serial.available() > 0) 
    { 

    action = Serial.read();  

      switch (action)
      {
      case 'w' : case 'W':
        ledYellow(1);
        ledRed(0);
        ledGreen(0);
        motors.setSpeeds(speed, speed);
        delay(800);
        Serial.write("you are moving forwards");
        break;
      case 's' : case 'S':
        // go backwards
        ledYellow(0);
        ledRed(0);
        ledGreen(1);
        motors.setSpeeds(speed*-1, speed*-1);
        delay(800);
        Serial.write("You are moving backwards");
        break;  
      case 'a' : case 'A':
        // turn left
        ledRed(1);
        ledGreen(0);
        ledYellow(0);
        motors.setLeftSpeed(-speed);
        motors.setRightSpeed(speed);
        delay(500);
        Serial.write("You are moving to the left");
        break;   
      case 'd': case 'D':
        // turn right
        ledRed(1);
        ledGreen(0);
        ledYellow(0);
        motors.setLeftSpeed(speed);
        motors.setRightSpeed(-speed);
        delay(500);
        Serial.write("You are moving to the right");
        break;   
      case 'q' : case 'Q' :
        motors.setSpeeds(0, 0);
        delay(100);
        Serial.write("you have come to a stop");   
    }
    delay(500);
   }
}

//autonomous code is inside the loop, autonomous mode will continue until zumo detects a turn, then manual controls will be given via the serial monitor or the associated gui, for the user to control zumo around the corner
void loop() {
  // put your main code here, to run repeatedly:

  proxSensors.read();

  uint16_t leftSensor = proxSensors.countsLeftWithLeftLeds();
  uint16_t rightSensor = proxSensors.countsRightWithRightLeds();
  uint16_t leftCentreSensor = proxSensors.countsFrontWithLeftLeds();
  uint16_t rightCentreSensor = proxSensors.countsFrontWithRightLeds();

  uint16_t sampleTime = 0;
  if ((uint16_t)(millis()) - sampleTime >=100){
    sampleTime = millis();
    lineSensors.read(lineSensorValues);
  }

  if ((lineSensorValues[0] > max && lineSensorValues[1] > cornerSpeed || lineSensorValues[2] > max && lineSensorValues[1] > cornerSpeed) || (leftSensor > 5 || rightSensor > 5 || (leftCentreSensor > 5 && rightCentreSensor > 5 && leftSensor > 5 && rightSensor > 5))){
   
    if (lineSensorValues[0] > max && lineSensorValues[1] > cornerSpeed || lineSensorValues[2] > max && lineSensorValues[1] > cornerSpeed)
    {
          Serial.print("corner detected \n");
    }

    if (leftSensor > 5 || rightSensor > 5 || (leftCentreSensor > 5 && rightCentreSensor > 5 && leftSensor > 5 && rightSensor > 5))
    {
      if (leftSensor > 5)
      {
          Serial.print("object detected to the left");
        motors.setSpeeds(-speed, speed);
      }
      if (rightSensor > 5)
      {
          Serial.print("object detected to the left");
        motors.setSpeeds(speed, -speed);
      }
      if (leftCentreSensor > 6 && rightCentreSensor > 6 && leftSensor > 5 && rightSensor > 5)
      {
          Serial.print("objected detected in front of zumo");
        motors.setSpeeds(0, 0);
      }
      delay(200);
    }
    mode1();
  }else{
    mode3();
  } 
}