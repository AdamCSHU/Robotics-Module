#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4Buzzer buzzer;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;

/*mode3.ino, fully autonomous mode. This mode requires no user interruptions, the zumo robot will drive and turn by itself, while staying inside the boundaries of the maze. Solution is incomplete. 
Research and inspiration includes the following sources: 
https://github.com/pololu/zumo-32u4-arduino-library/tree/master/examples 
https://www.pololu.com/docs/0J63
https://pololu.github.io/zumo-32u4-arduino-library/index.html
https://www.pololu.com/category/7/sensors
https://www.pololu.com/category/170/zumo-32u4-oled-robot
https://registry.platformio.org/libraries/pololu/Zumo32U4
*/

#define numSensors 5;
unsigned int lineSensorValues[numSensors];
uint16_t max = 400;
uint16_t speed = 200;
uint16_t min = 210;
int cornerSpeed = 150;

//calibrate sensors before starting mode 3
void calibrateSensor(){
//wait two second before starting sensor calibration, calibration of line sensors rotates zumo in place to place sensors over the line. 
delay(2000);

for (unsigned int i = 0; i < 120; i++){
  if (i > 30 && i <= 90){
    motors.setSpeeds(-speed, speed);
  }
  else{

    motors.setSpeeds(speed, -speed);
  }
  lineSensors.calibrate();
  }
motors.setSpeeds(0, 0);
}

void setup(){
  //this code will run once during execution.
  Serial.begin(9600);

  lineSensors.initFiveSensors();
  proxSensors.initThreeSensors();

  calibrateSensor();

  delay(1000);

  buzzer.play(">g32<<c32");
}

void loop(){

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
  }
}
