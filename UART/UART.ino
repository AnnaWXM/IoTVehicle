#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
//const int JoyX = A8, JoyY = A9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define ENC_A_L 23 // for direction sensing pin D24
#define ENC_B_L 3 // for pulse counting pin D3 = INT5
#define ENC_A_R 24 // for direction sensing pin D23
#define ENC_B_R 2 // for pulse counting pin D2 = INT4

#define Motor_L_dir_pin       7
#define Motor_R_dir_pin       8
#define Motor_L_pwm_pin       9
#define Motor_R_pwm_pin       10

double count_L = 0 , count_R = 0;
// math for counting distance
int r = 3; // 3 cm
const float pi = 3.14;
float p = 270;              //? ?????170
double DistancePerPulse = 0.069;
double DPulse=1;
double distance_L = 0;                                //distance in cm
double distance_R = 0;
int distVal = 0;


void isr_L();
void isr_R();

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
  
  pinMode(Motor_R_dir_pin,OUTPUT); //PWM
  pinMode(Motor_L_dir_pin,OUTPUT); //PWM
  pinMode(Motor_L_pwm_pin,OUTPUT); //PWM
  pinMode(Motor_R_pwm_pin,OUTPUT); //PWM

  attachInterrupt(digitalPinToInterrupt(ENC_B_L), isr_L, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENC_B_R), isr_R, FALLING);
}

void loop()                    
{   
       
  //x = analogRead(JoyX);  
  //y = analogRead(JoyY);  
  pwm_L=0, pwm_R=0;                       
  //xd = map(x, 0, 1023, -10, 10);                     // x dirction
  //ySpeed = map(y, 0, 1023, -255, 255);               // y speed, unit in Hz, 1 Hz is 6.28 rad/s, 6.28 Radians in a full circle

  //distance_L = count_L * DistancePerPulse ;
  //distance_R = count_R * DistancePerPulse ;

if (Serial.available() > 0){
    String message = Serial.readStringUntil('\n'); 
    Serial.print("Message received, content: ");  
    Serial.println(message);
    int pos_s = message.indexOf("Move");
    lcd.setCursor(0, 0);
    lcd.clear();

    if (pos_s > -1){
      Serial.println("Command = Move ");
      pos_s = message.indexOf(":");
      String stat = message.substring(pos_s + 1);
      lcd.print(stat);
      distVal = stat.toInt();
      Serial.print("Distance:");
      Serial.println(distVal);
      Serial.println("i'm here");

      if(distVal > 0){
        digitalWrite(Motor_L_dir_pin, 1);
        digitalWrite(Motor_R_dir_pin, 1);
        Serial.println("Motor_L_dir_pin");
      }
      if(distVal < 0){
        digitalWrite(Motor_L_dir_pin, 0);
        digitalWrite(Motor_R_dir_pin, 0);
      }

      while (count_L*DistancePerPulse < distVal) {                    //forward 10cm
  //Serial.println(count_L*DistancePerPulse);
        analogWrite(Motor_L_pwm_pin, 254);
        analogWrite(Motor_R_pwm_pin, 254);
        Serial.println(count_L);
      }

      analogWrite(Motor_L_pwm_pin, 0);
      analogWrite(Motor_R_pwm_pin, 0);

      delay(500);

      count_L = 0;
    } 
    else{
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

                          
  
  