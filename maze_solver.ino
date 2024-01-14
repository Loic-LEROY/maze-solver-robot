#define LIGHT_SENSOR_PIN A0

#define PROX_SENSOR_L_PIN A1
#define PROX_SENSOR_R_PIN A2
#define PROX_SENSOR_FL_PIN A3
#define PROX_SENSOR_FR_PIN A4
#define PROX_SENSOR_RL_PIN A5
#define PROX_SENSOR_RR_PIN 12
#define PROX_SENSOR_DL_PIN 6
#define PROX_SENSOR_DR_PIN 9

#define MOTOR_RF_PIN 2
#define MOTOR_RB_PIN 4
#define MOTOR_R_SPEED 3
#define MOTOR_LF_PIN 7
#define MOTOR_LB_PIN 8
#define MOTOR_L_SPEED 5

// coefficient that is applied between the speed of the two motors when the robot moves forward.
float motor_speed_ratio = 0.6;
// Variable allowing us to know if we are out of the starting tile.
bool out_of_the_red = false;
// Variable used to store the light sensor value.
int light_sensor_value;
// Counter of values linked to the black color.
int black_counter = 0;
// Counter of values linked to the red color.
int red_counter = 0;
// Variable allowing you to know if the balck tile has been crossed.
bool black_tile_passed = false;
// Variable allowing you to know if the red tile has been crossed.
bool red_tile_reached = false;
// Variable used to know if the game is over.
bool stop = false;

// The purpose of this function is to test whether an integer is between two values. 
// It returns true if this is the case.
bool is_in_range(int value, int lower_value, int upper_value) {
  if((value > lower_value)&&(value < upper_value)) {
    return true;
  }
  return false;
}

void hardware_setup() {
  new DCMotor_Hbridge(MOTOR_RF_PIN, MOTOR_RB_PIN, MOTOR_R_SPEED, "ePuck_rightJoint", 2.5, 3 * 3.14159, 1);
  new DCMotor_Hbridge(MOTOR_LF_PIN, MOTOR_LB_PIN, MOTOR_L_SPEED, "ePuck_leftJoint", 2.5, 3 * 3.14159, 1);

  new VisionSensor(LIGHT_SENSOR_PIN, "ePuck_lightSensor", 0.1);

  new ProximitySensor(PROX_SENSOR_FL_PIN, "ePuck_proxSensor3", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_FR_PIN, "ePuck_proxSensor4", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_L_PIN, "ePuck_proxSensor1", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_R_PIN, "ePuck_proxSensor6", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_RL_PIN, "ePuck_proxSensor7", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_RR_PIN, "ePuck_proxSensor8", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_DL_PIN, "ePuck_proxSensor2", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_DR_PIN, "ePuck_proxSensor5", 0.1, 1);
}

void setup() {
  Serial.begin(4800);

  pinMode(MOTOR_RF_PIN, OUTPUT);
  pinMode(MOTOR_RB_PIN, OUTPUT);
  pinMode(MOTOR_R_SPEED, OUTPUT);
  pinMode(MOTOR_LF_PIN, OUTPUT);
  pinMode(MOTOR_LB_PIN, OUTPUT);
  pinMode(MOTOR_L_SPEED, OUTPUT);

  // Set speed to max
  analogWrite(MOTOR_R_SPEED, 255);
  analogWrite(MOTOR_L_SPEED, 255);

  digitalWrite(MOTOR_RF_PIN, HIGH);
  digitalWrite(MOTOR_RB_PIN, LOW);
  digitalWrite(MOTOR_LF_PIN, HIGH);
  digitalWrite(MOTOR_LB_PIN, LOW);
}

void loop() {

// If the stop variable is false, we enter this loop.
  if (!stop) {

    // The value of the light sensor is read
    light_sensor_value = analogRead(LIGHT_SENSOR_PIN);

    // If the robot has still not left the start tile and the light sensor value does not match the colour red, 
    // we know that the robot has left the start tile.
    if ((out_of_the_red == false) && (!is_in_range(light_sensor_value, 260, 300))) {
      out_of_the_red = true;
    }

    // If he has already left the starting tile, we look to see if he has reached the black tile.
    if ((out_of_the_red == true) && (light_sensor_value <= 50)) {
      black_counter++;
      // Experimentally, we found that a recurrence of more than 1000 values corresponding to the colour black 
      // was sufficient to know that it had reached the black tile and that it didn't just pass over a line.
      if(black_counter >= 1000) {
        black_tile_passed = true;
      }
    }
    // If none of the above is true, set the black_counter variable back to 0.
    else {
      black_counter = 0;
    }

    // If the robot has already crossed the black tile, we test as before if it has returned to the red tile.
    if((black_tile_passed)&&(is_in_range(light_sensor_value, 260, 300))) {
      red_counter++;
      if(red_counter >= 1000) {
        red_tile_reached = true;
        stop = true;
      }
    }

    // In order to follow the right wall, if the right diagonal sensor detects a wall, we considerably reduce the 
    // speed of the left motor to move away from the wall.
    if (digitalRead(PROX_SENSOR_DR_PIN)) {
      analogWrite(MOTOR_R_SPEED, 255);
      analogWrite(MOTOR_L_SPEED, 10);
    // If it does not detect a wall, we move forward but with a small difference in speed between the motor to 
    // always get a little closer to the right wall as we move forward.
    } else {
      analogWrite(MOTOR_R_SPEED, 255 * motor_speed_ratio);
      analogWrite(MOTOR_L_SPEED, 255);
    }
  }
  // If the stop variable is true, we set the motors speed to 0.
  else {
    analogWrite(MOTOR_R_SPEED, 0);
    analogWrite(MOTOR_L_SPEED, 0);
  }
}
