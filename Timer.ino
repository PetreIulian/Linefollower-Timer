#include <LiquidCrystal_I2C.h>

#define trig 4
#define echo 3
#define resetButton 5

#define DIST_THRESHOLD 5

LiquidCrystal_I2C lcd(0x27, 16, 2);

enum TimerState { IDLE, RUNNING, STOPPED };
TimerState timerState = IDLE;

unsigned long startTime = 0;
unsigned long elapsedTime = 0;

unsigned long lastDetectionTime = 0;
const unsigned long detection_lock_time = 1000;

bool robotDetected = false;
bool lastRobotDetected = false;
int detectionCount = 0;

void setup() {

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(resetButton, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {

  if(digitalRead(resetButton) == LOW) {
    delay(50);
    while(digitalRead(resetButton) == LOW);
    resetTimer();
  }

  robotDetected = detectRobot();

  if(robotDetected && !lastRobotDetected && detectionAllowed()) {

    lastDetectionTime = millis();
    detectionCount++;

    if(detectionCount == 1 && timerState == IDLE) {
      startTime = millis();
      timerState = RUNNING;
      lcd.clear();
    }
    else if(detectionCount == 2 && timerState == RUNNING) {
      elapsedTime = millis() - startTime;
      timerState = STOPPED;
    }
  }

  lastRobotDetected = robotDetected;

  if(timerState == IDLE) {
    lcd.setCursor(0,0);
    lcd.print("Ready...");
    lcd.setCursor(0,1);
    lcd.print("Waiting Movement");
  }

  if(timerState == RUNNING) {
    elapsedTime = millis() - startTime;
    displayTime(elapsedTime);
  }

  if(timerState == STOPPED) {
    displayTime(elapsedTime);
    lcd.setCursor(0,1);
    lcd.print("Press RESET");
  }
}

void displayTime(unsigned long elapsedTime) {
  unsigned int minutes = elapsedTime / 60000;
  unsigned int seconds = (elapsedTime % 60000) / 1000;
  unsigned int milliseconds = elapsedTime % 1000;

  char timeString[10];
  sprintf(timeString, "02u:%02u:%03u", minutes, seconds, milliseconds);

  lcd.setCursor(0,0);
  lcd.print(timeString);
}

float ultrasonic() {
  digitalWrite(trig, LOW);
  delay(2);
  digitalWrite(trig, HIGH);
  delay(10);
  
  unsigned int duration = pulseIn(echo, HIGH);
  return (duration * 0.0343) / 2;
}

bool detectRobot() {
  float distance = ultrasonic();
  return (distance > 0 && distance < DIST_THRESHOLD);
}

void resetTimer() {
  timerState = IDLE;
  detectionCount = 0;
  elapsedTime = 0;
  startTime = 0;
  lcd.clear();
} 

bool detectionAllowed() {
  return (millis() - lastDetectionTime) > detection_lock_time;
}
