# Timer to register the track time of a linefollower robot
  
## Components
   1.**Ultrasonic Sensor**
   
   2.**LCD Display**
   
   3.**I2C Adapter for Display**
   
   4.**Button**
   
   5.**Arduino Nano v3**

### Functionality
  The timer has 3 state: IDLE, RUNNING and STOPPED:
  
  1.**IDLE** the system is waiting to detect the robot.
     
  2.**RUNNING** after the robot is detected the timer start to count; the elapsed time is showed on the display.
     
  3.**STOPPED** after the second detection of the robot, the timer is stopped and the total time is displayed on the display.
   
