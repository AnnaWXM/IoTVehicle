#include <LiquidCrystal.h>
#include <Wire.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define CMPS14_address 0x60
byte raw;
int bearing = 1, bearIN = 0, bearDO = 0, bearFI =0;
String dir;
String DIR(int);

#define ENC_A_L 23  // for direction sensing pin D24
#define ENC_B_L 3   // for pulse counting pin D3 = INT5
#define ENC_A_R 24  // for direction sensing pin D23
#define ENC_B_R 2   // for pulse counting pin D2 = INT4

#define Motor_L_dir_pin 7
#define Motor_R_dir_pin 8
#define Motor_L_pwm_pin 9
#define Motor_R_pwm_pin 10

double count_L = 0, count_R = 0;
// math for counting distance
int r = 3;  // 3 cm
const float pi = 3.14;
float p = 270;  //? ?????170
double DistancePerPulse = 0.069;
double DPulse = 1;
double distance_L = 0;  //distance in cm
double distance_R = 0;
int distVal = 0, DistVal = 0;

void isr_L();
void isr_R();

int x, y, xp, xd, ySpeed, yd, pwm_L, pwm_R;

void setup() {
  Wire.begin();

  // setting encoder pins as input
  pinMode(ENC_A_L, INPUT);
  pinMode(ENC_B_L, INPUT);
  pinMode(ENC_A_R, INPUT);
  pinMode(ENC_B_R, INPUT);

  lcd.begin(20, 4);
  lcd.print("Hello, world! :)");
  Serial.begin(9600);
  Serial.println("SMS Messages Receiver");

  pinMode(Motor_R_dir_pin, OUTPUT);  //PWM
  pinMode(Motor_L_dir_pin, OUTPUT);  //PWM
  pinMode(Motor_L_pwm_pin, OUTPUT);  //PWM
  pinMode(Motor_R_pwm_pin, OUTPUT);  //PWM

  attachInterrupt(digitalPinToInterrupt(ENC_B_L), isr_L, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENC_B_R), isr_R, FALLING);
}

void loop() {

  Wire.beginTransmission(CMPS14_address);
  Wire.write(1);
  Wire.endTransmission(false);
  Wire.requestFrom(CMPS14_address,1,true);
  if(Wire.available()>=1){
    raw = Wire.read();
  }
  bearing = map(raw,0,255,0,360);
  lcd.setCursor(0, 1);
  lcd.println("Initial bearing");  
  lcd.println(bearing);
  pwm_L = 0, pwm_R = 0;

  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    Serial.print("Message received, content: ");
    Serial.println(message);
    int pos_sum, pos_m, pos_t, pos_s;
    pos_m = message.indexOf("Move");                       // return -1 when there is no move or turn
    pos_t = message.indexOf("Turn");
    pos_sum = pos_m + pos_t;
    Serial.println(pos_s);

    if (pos_sum > -2) {
      if (pos_m > -1){
        Serial.println("Command = Move ");
      }
      if (pos_t > -1){
        Serial.println("Command = Turn ");
      }

      pos_s = message.indexOf(":");
      String stat = message.substring(pos_s + 1);
      lcd.print(stat);

      if  (pos_m > -1){
        distVal = stat.toInt();
        lcd.print("Distance:");
        lcd.println(distVal);
        if (distVal > 0) {
          digitalWrite(Motor_L_dir_pin, 1);
          digitalWrite(Motor_R_dir_pin, 1);
          DistVal = distVal;
        }
        if (distVal < 0) {
          digitalWrite(Motor_L_dir_pin, 0);
          digitalWrite(Motor_R_dir_pin, 0);
          DistVal = - distVal;
        }
        while (count_L * DistancePerPulse < DistVal) {
          //Serial.println(count_L*DistancePerPulse);
          analogWrite(Motor_L_pwm_pin, 254);
          analogWrite(Motor_R_pwm_pin, 254);
        }
      }

      if  (pos_t > -1){
        bearDO = stat.toInt();
        lcd.print("Turn Degree:");
        lcd.println(bearDO);
        bearFI = (bearIN + bearDO) % 360 ;
        digitalWrite(Motor_L_dir_pin, 1);
        digitalWrite(Motor_R_dir_pin, 1);
        if (bearDO > 0 && bearFI == bearIN + bearDO) {                   //turn right
          while (bearing < bearFI) {
            analogWrite(Motor_L_pwm_pin, 254);
            analogWrite(Motor_R_pwm_pin, 0);
            Wire.beginTransmission(CMPS14_address);
            Wire.write(1);
            Wire.endTransmission(false);
            Wire.requestFrom(CMPS14_address,1,true);
            if(Wire.available()>=1){
              raw = Wire.read();
            }
            bearing = map(raw,0,255,0,360);
            Serial.println(bearing);
          }
        }
        if (bearDO < 0 && bearFI == bearIN + bearDO) {                  //turn left
          while (bearing > bearFI) {
            analogWrite(Motor_L_pwm_pin, 0);
            analogWrite(Motor_R_pwm_pin, 254);
            Wire.beginTransmission(CMPS14_address);
            Wire.write(1);
            Wire.endTransmission(false);
            Wire.requestFrom(CMPS14_address,1,true);
            if(Wire.available()>=1){
              raw = Wire.read();
            }
            bearing = map(raw,0,255,0,360);
            Serial.println(bearing);
          }
        }
        if (bearFI != bearIN + bearDO) {
          Serial.println("put the car to better position");
          lcd.clear();
          lcd.println("put the car to better position");
        }

      }

      analogWrite(Motor_L_pwm_pin, 0);
      analogWrite(Motor_R_pwm_pin, 0);

      delay(500);

      count_L = 0;
      bearIN = bearing;
    } else {
      lcd.print("No message found");
      Serial.println("No message found, try typing dist:text\n");
    }
  }
}


void isr_L() {
  count_L++;
}

void isr_R() {
  count_R++;
}