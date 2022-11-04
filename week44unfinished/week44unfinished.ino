#include <LiquidCrystal.h>
//#include <TimerOne.h>
#include <Wire.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
const int JoyX = A8, JoyY = A9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define ENC_A_L 23 // for direction sensing pin D24
#define ENC_B_L 3 // for pulse counting pin D3 = INT5
#define ENC_A_R 24 // for direction sensing pin D23
#define ENC_B_R 2 // for pulse counting pin D2 = INT4

#define Motor_L_dir_pin       7
#define Motor_R_dir_pin       8
#define Motor_L_pwm_pin       9
#define Motor_R_pwm_pin       10

#define CMPS14_address 0x60            //define compass
  byte raw;
  int bearing=1;
  int b_1;

int count_L=0;
int count_R=0;
// math for counting distance
int r = 3;                                        // 3 cm
const float pi = 3.14;
//int t=0;                                            //time, unit in s;
float D = 2 * pi * r;
  // here is a small experiment, when this car have full speed, the distance in 254 pluse is 16cm.  week 6
  //float p = 254 * D / 16;                           //how many pulse per round when the wheel move.
float p = 170;              //? ?????
double DistancePerPulse = 0.063;
double distance_L = 0;                                //distance in cm
double distance_R = 0;

void isr_L();
void isr_R();
//int Timer(int);
void forward();
void backward();
void left();
void right();
void stop();

int x,y,xp,xd,ySpeed,yd,pwm_L, pwm_R;                              

void setup()
{
    // setting encoder pins as input
  pinMode(ENC_A_L, INPUT);
  pinMode(ENC_B_L, INPUT);
  pinMode(ENC_A_R, INPUT);
  pinMode(ENC_B_R, INPUT);

  lcd.begin(20, 4);
  lcd.print("Hello, world! :)");
  delay(100);
  lcd.clear();
  Serial.begin(9600);
  Serial.println("SMS Messages Receiver");
  
  pinMode(JoyX,INPUT);  //X
  pinMode(JoyY,INPUT);  //Y
  pinMode(Motor_R_dir_pin,OUTPUT); //PWM
  pinMode(Motor_L_dir_pin,OUTPUT); //PWM
  pinMode(Motor_L_pwm_pin,OUTPUT); //PWM
  pinMode(Motor_R_pwm_pin,OUTPUT); //PWM

  attachInterrupt(digitalPinToInterrupt(ENC_B_L), isr_L, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENC_B_R), isr_R, FALLING);
  // Timer1.initialize( 1000000 );           //initialize timer1, and set a 1s period
  // Timer1.attachInterrupt( Timer ); 
  Wire.begin();
  Wire.beginTransmission(CMPS14_address);            //use compass meature degree
  Wire.write(1);
  Wire.endTransmission(false);
  Wire.requestFrom(CMPS14_address,1,true);
  if(Wire.available()>=1){
    raw = Wire.read();
  }
  bearing = map(raw,0,255,0,365);
  b_1 = bearing; 
}

void loop()                    
{           
  x = analogRead(JoyX);  
  y = analogRead(JoyY);  
  pwm_L=0, pwm_R=0;                       
  xd = map(x, 0, 1023, -10, 10);                     // x dirction
  ySpeed = map(y, 0, 1023, -255, 255);               // y speed, unit in Hz, 1 Hz is 6.28 rad/s, 6.28 Radians in a full circle
  
  if(y<550){                          // y dirction
    yd=0;
  }else if (y>600){
    yd=1;
  }

  if(ySpeed!=-1){
    digitalWrite(Motor_R_dir_pin,yd);  
    digitalWrite(Motor_L_dir_pin,yd);
    if(ySpeed>0){
      pwm_L=ySpeed;
      pwm_R=ySpeed;
    }
    if(ySpeed<0){
      pwm_L=-ySpeed;
      pwm_R=-ySpeed;
    }
    if(xd<-1){
     pwm_L=pwm_L;
     pwm_R=pwm_R/2;
    }else if(xd>0){
     pwm_R=pwm_R;
     pwm_L=pwm_L/2;
    }  
    analogWrite(Motor_L_pwm_pin,pwm_L);
    analogWrite(Motor_R_pwm_pin,pwm_R);
  }

  distance_L = count_L * DistancePerPulse;
  distance_R = count_R * DistancePerPulse;

  lcd.setCursor(0, 0);
  lcd.print("for left: ");
  lcd.setCursor(0, 1);
  lcd.print(distance_L);
  lcd.setCursor(0, 2);
  lcd.print("for right: ");
  lcd.setCursor(0, 3);
  lcd.print(distance_R);
    
  Serial.println(distance_L);

  Wire.beginTransmission(CMPS14_address);            //use compass meature degree
  Wire.write(1);
  Wire.endTransmission(false);
  Wire.requestFrom(CMPS14_address,1,true);
  if(Wire.available()>=1){
    raw = Wire.read();
  }
  bearing = map(raw,0,255,0,365);
  
  if (distance_L<5) {forward();}
  if (distance_L>=5) {stop();}

  //delay(1000);
  
  left();                    //OR right();

  if( bearing >= b_1 + 180) {stop();}

  //delay(1000);

  right();
  if( bearing == b_1) {stop();}
  backward();
  if (distance_L<=0) {stop();}

    
}

void isr_L() {
  if(yd>0)
  {
    count_L++;
    }else {
      count_L--;
    }
}

void isr_R() {
  if(yd>0)
  {
    count_R++;
    }else {
      count_R--;
    }
}

// int Timer(int count){
//   if (count != count) t++;
// }

void forward(){
  Serial.println(digitalRead(Motor_L_dir_pin));
  digitalWrite(Motor_L_dir_pin, HIGH);
  digitalWrite(Motor_L_pwm_pin, HIGH);
  digitalWrite(Motor_R_dir_pin, HIGH);
  digitalWrite(Motor_R_pwm_pin, HIGH);
  Serial.println(digitalRead(Motor_L_dir_pin));
}
void backward(){
  digitalWrite(Motor_L_dir_pin, LOW);
  digitalWrite(Motor_L_pwm_pin, HIGH);
  digitalWrite(Motor_R_dir_pin, LOW);
  digitalWrite(Motor_R_pwm_pin, HIGH);
}
void left(){
  digitalWrite(Motor_L_dir_pin, LOW);
  digitalWrite(Motor_L_pwm_pin, HIGH);
  digitalWrite(Motor_R_dir_pin, HIGH);
  digitalWrite(Motor_R_pwm_pin, LOW);
}
void right(){
  digitalWrite(Motor_L_dir_pin, HIGH);
  digitalWrite(Motor_L_pwm_pin, LOW);
  digitalWrite(Motor_R_dir_pin, LOW);
  digitalWrite(Motor_R_pwm_pin, HIGH);
}
void stop(){
  digitalWrite(Motor_L_dir_pin, LOW);
  digitalWrite(Motor_L_pwm_pin, LOW);
  digitalWrite(Motor_R_dir_pin, LOW);
  digitalWrite(Motor_R_pwm_pin, LOW);
}