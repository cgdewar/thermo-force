/*************************************************************
FYDP-Group12
-Motor characterization PoC
-Will be used to characterize motors during testing
-Needs to be done at no load and stall load
*************************************************************/
// Constants
const int BAUDRATE = 9600;
const int MOTOR_BRAKE_A = 9;
const int MOTOR_DIR_A = 12;
const int MOTOR_PWM_A = 3;
const int MOTOR_ISENSE_A = A0;

// Globals
float motCurrentArr[256];
bool stallTesting = true;
int pwmCmd = 0;

// Motor shield initalize
void setupMotorShield() {
  // Setup Motor Channel A
  pinMode(MOTOR_BRAKE_A, OUTPUT);
  pinMode(MOTOR_DIR_A, OUTPUT);
  pinMode(MOTOR_PWM_A, OUTPUT);
  pinMode(MOTOR_ISENSE_A, INPUT);
  digitalWrite(MOTOR_BRAKE_A, LOW);
  digitalWrite(MOTOR_DIR_A, HIGH);
  Serial.println("Motor shield initialized");
}

// Main
void setup() {
  Serial.begin(BAUDRATE);
  while (!Serial) {} // Wait for initilization
  setupMotorShield();
}

void loop() {
  // Characterize motor -> map pwm to current
  digitalWrite(MOTOR_BRAKE_A, LOW);
  digitalWrite(MOTOR_DIR_A, HIGH);

  if (!stallTesting) {
    // Start motor at high pwm
    analogWrite(MOTOR_PWM_A, 255);
    delay(1000);
  }

  // Turn on motor
  analogWrite(MOTOR_PWM_A, pwmCmd);

  // Wait to reach steady state
  delay(3000);

  // Take 10 measurements
  float motCurrent = 0;
  for (int i = 0; i <= 10; i++) {
    motCurrent += analogRead(MOTOR_ISENSE_A)*2.96/1000; // 2.96mA/count
    delay(100);
  }

  // Print pwmCmd and avgCurrent to console
  //Serial.println("pwmCmd: " + String(pwmCmd) + ", avgMotCurrent: " + String(motCurrent/10));
  motCurrentArr[pwmCmd] = motCurrent;
  
  // Increment
  if (pwmCmd == 255) {
      Serial.println("Stall Testing: " + String(stallTesting));
      for (int i = 0; i <= 255; i++) {
        Serial.println("pwmCmd: " + String(i) + ", avgMotCurrent: " + String(motCurrentArr[i]));
      }
    analogWrite(MOTOR_PWM_A, 0);
    while(true) {}
  }
  else {
    pwmCmd += 1;
  }
}
