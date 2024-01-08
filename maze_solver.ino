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

float motor_speed_ratio = 0.6;

bool out_of_the_red = false;

int light_sensor_value;
int black_counter = 0;
int red_counter = 0;
bool black_square_passed = false;
bool red_square_reached = false;
bool stop = false;

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

  if (!stop) {

    light_sensor_value = analogRead(LIGHT_SENSOR_PIN);

    if ((out_of_the_red == false) && (!is_in_range(light_sensor_value, 260, 300))) {
      out_of_the_red = true;
    }

    if ((out_of_the_red == true) && (light_sensor_value <= 50)) {
      black_counter++;
      if(black_counter >= 1000) {
        black_square_passed = true;
      }
    }
    else {
      black_counter = 0;
    }

    if((black_square_passed)&&(is_in_range(light_sensor_value, 260, 300))) {
      red_counter++;
      if(red_counter >= 1000) {
        red_square_reached = true;
        stop = true;
      }
    }

    if (digitalRead(PROX_SENSOR_DR_PIN)) {
      analogWrite(MOTOR_R_SPEED, 255);
      analogWrite(MOTOR_L_SPEED, 10);
    } else {
      analogWrite(MOTOR_R_SPEED, 255 * motor_speed_ratio);
      analogWrite(MOTOR_L_SPEED, 255);
    }
  }
  else {
    analogWrite(MOTOR_R_SPEED, 0);
    analogWrite(MOTOR_L_SPEED, 0);
  }
}
