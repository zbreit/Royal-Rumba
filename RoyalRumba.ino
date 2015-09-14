#include <ZumoBuzzer.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
 
#define LED  13
 
// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD 1500 // microseconds
  
// these might need to be tuned for different motor types
#define REVERSE_SPEED    380 // 0 is stopped, 400 is full speed
#define TURN_SPEED       380
#define FORWARD_SPEED    400
#define REVERSE_DURATION 200 // ms
#define TURN_DURATION    200 // ms
 
ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
 
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
 
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

void waitForButtonAndCountDown()
{
  digitalWrite(LED, HIGH);
  button.waitForButton();
}
 
void setup()
{
  // uncomment if necessary to correct motor directions
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
   
  pinMode(LED, HIGH);
  waitForButtonAndCountDown();
}

void loop()
{
  if (button.isPressed())
  {
    // if button is pressed, stop and wait for another press to go again
    motors.setSpeeds(0, 0);
    button.waitForRelease();
  }
   
  randomSeed(millis());
  int random1 = random(-20,20);
  int random2 = random(-20,20);
  #define REVERSE_DURATION 200+random1 // ms
  #define TURN_DURATION    200+random2
  sensors.read(sensor_values);
  
  
  if ((sensor_values[0] > QTR_THRESHOLD) && (sensor_values[5] > QTR_THRESHOLD))        //Left and Right Sensor
  {
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else if (sensor_values[0] > QTR_THRESHOLD)        //Left Sensor
  {
    // if leftmost sensor detects line, reverse and turn to the right (r,l)
    motors.setSpeeds(-REVERSE_SPEED, 0);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-(TURN_SPEED-100), TURN_SPEED + 100);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else if (sensor_values[5] > QTR_THRESHOLD)   //Right Sensor
  {
    // if rightmost sensor detects line, reverse and turn to the left
     motors.setSpeeds(0, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED +100, -(TURN_SPEED-100));
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else
  {
    // otherwise, go straight
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  #define REVERSE_DURATION 200-random1 // ms
  #define TURN_DURATION    200-random2
}
