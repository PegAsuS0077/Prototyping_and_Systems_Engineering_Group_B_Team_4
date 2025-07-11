// Motor A pins (Left motor)
const int enA = 3;   // PWM pin
const int in1 = 8;
const int in2 = 9;

// Motor B pins (Right motor)
const int enB = 5;   // PWM pin
const int in3 = 10;
const int in4 = 11;

// Speed setting (0 to 255)
int speed = 180;

// Delay durations
int runTime = 2000;  // 2 seconds running
int pauseTime = 200; // 0.2 seconds pause

// To keep track of previous motor states
enum Direction { STOPPED, FORWARD, REVERSE };
Direction prevMotorAState = STOPPED;
Direction prevMotorBState = STOPPED;

void setup() {
  // Set motor direction pins as OUTPUT
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Set enable pins as OUTPUT
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  Serial.begin(9600);
  Serial.println("Motor test starting...");
}

void runMotor(int enPin, int dirPin1, int dirPin2, int speed, int duration, const char* motorName, Direction currentDir, Direction &prevDir) {
  // Check if phase/direction changed
  if (currentDir != prevDir) {
    if (currentDir == FORWARD) {
      Serial.print(motorName);
      Serial.println(" changing phase to FORWARD");
    } else if (currentDir == REVERSE) {
      Serial.print(motorName);
      Serial.println(" changing phase to REVERSE");
    } else if (currentDir == STOPPED) {
      Serial.print(motorName);
      Serial.println(" stopped");
    }
    prevDir = currentDir;  // Update previous state
  }

  if (currentDir == FORWARD) {
    digitalWrite(dirPin1, HIGH);
    digitalWrite(dirPin2, LOW);
  } else if (currentDir == REVERSE) {
    digitalWrite(dirPin1, LOW);
    digitalWrite(dirPin2, HIGH);
  } else { // STOPPED
    digitalWrite(dirPin1, LOW);
    digitalWrite(dirPin2, LOW);
    analogWrite(enPin, 0);
    delay(duration);
    return;
  }

  analogWrite(enPin, speed);

  Serial.print("Running ");
  Serial.print(motorName);
  Serial.print(" ");
  Serial.print((currentDir == FORWARD) ? "forward" : "reverse");
  Serial.print(" at speed ");
  Serial.println(speed);

  delay(duration);

  // Stop motor and print stopping message
  analogWrite(enPin, 0);
  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, LOW);

  Serial.print(motorName);
  Serial.println(" stopped");

  delay(pauseTime);
}

void loop() {
  // Motor A forward
  runMotor(enA, in1, in2, speed, runTime, "Motor A", FORWARD, prevMotorAState);

  // Motor A reverse
  runMotor(enA, in2, in1, speed, runTime, "Motor A", REVERSE, prevMotorAState);

  // Motor B forward
  runMotor(enB, in3, in4, speed, runTime, "Motor B", FORWARD, prevMotorBState);

  // Motor B reverse
  runMotor(enB, in4, in3, speed, runTime, "Motor B", REVERSE, prevMotorBState);
}
