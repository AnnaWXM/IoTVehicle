#include <LiquidCrystal.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
const int JoyX = A8, JoyY = A9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int ledPin1 = 44;  //yellow
int ledPin2 = 46;  //green 
int buttonPin = 19;
boolean buttonState = false;
bool ledState = LOW;
void isr();

void setup() {
  lcd.begin(20, 3);
  lcd.print("hello, world!");
  delay(500);
  lcd.clear();
  Serial.begin(9600);
  Serial.println("SMS Messages Receiver");
  attachInterrupt(digitalPinToInterrupt(buttonPin), isr, FALLING);
  pinMode(JoyX,INPUT);  //X
  pinMode(JoyY,INPUT);  //Y
  pinMode(ledPin1,OUTPUT); //PWM
  pinMode(ledPin2,OUTPUT); //PWM
}

void loop()
{
  int x = analogRead(JoyX);  
  int y = analogRead(JoyY);  //green
  int xp,yp;
  xp = map(x, 0, 1023, -127, 127);
  yp = map(y, 0, 1023, 0, 255);

  if(buttonState == false){
    if(xp >= 4 && yp>= -1) {
    analogWrite(ledPin2,yp);
     analogWrite(ledPin1,0); 
    }
    if(xp <= -5 && yp >= -1) {
    analogWrite(ledPin1,yp);
    analogWrite(ledPin2,0); 
    }
  if(xp>=-3&&xp<=3){
    if(yp >= 0){
    analogWrite(ledPin1,yp);
    analogWrite(ledPin2,yp);
    }else{
    analogWrite(ledPin1,0);
    analogWrite(ledPin2,0);
    }
  }
    }else{
      digitalWrite(ledPin1,ledState);
      digitalWrite(ledPin2,ledState);
      }
}

void isr() {
  Serial.println("button");
    buttonState = !buttonState;
    ledState = !ledState;
}