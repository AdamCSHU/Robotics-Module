# Robotics-Module
A collections of tasks for the SHU Robotics Module

Research includes the following sources: 
https://github.com/pololu/zumo-32u4-arduino-library/tree/master/examples 
https://www.pololu.com/docs/0J63
https://pololu.github.io/zumo-32u4-arduino-library/index.html
https://www.pololu.com/category/7/sensors
https://www.pololu.com/category/170/zumo-32u4-oled-robot
https://registry.platformio.org/libraries/pololu/Zumo32U4


# H1 Steps to setup zumo for application: 

## manual mode (Mode 1):
In this mode, the zumo robot will be controlled totally manually, the user has the option to use the included basic GUI, or they can use the serial monitor. 

### CONTROLS: 
w - go forward 
s - go backwards 
a - go to the left 
d - go to the right
|| the basic GUI includes these controls as well as other buttons to stop the zumo. 

## semi-autonomous mode (Mode 2): 
In this mode, the zumo robot will operate autonomously by keeping itself inside the lines using line sensors. if zumo encounters a corner or turn, manual control will be returned to the user, allowing them to make a turn or go backwards. After zumo has completed a turn, autonomous controls will be returned and zumo will continue along inside the lines of the maze. 

### CONTROLS: 
cControls here are the same as manual mode. please look above.

## autonomous mode (Mode 3) (INCOMPLETE): 
In this mode, zumo will be fully autonomous. It will perform all tasks by itself without user interruption. 
Unfortunately, I have not managed to make this mode fully functional, it is incomplete. 

# Installation and setup 
PlatformIO -> For this project, I have used a visual studio code extension called "platformIO", for that reason, you will need to install the platformIO extension along with it's core associated libraries. Below, I have left links to setup the board (zumo32U4) inside of platformIO. 
