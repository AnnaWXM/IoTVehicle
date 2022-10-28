#include <LiquidCrystal.h>
#include <Wire.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
const int JoyX = A8, JoyY = A9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define CMPS14_address 0x60
  byte raw;
  int bearing=1;
  String dir;
  String DIR(int);

void setup()
{
  Serial.begin(9600);
  Serial.println("SMS Messages Receiver");
  lcd.begin(20,4);
  lcd.print("Hello world!");
  Wire.begin();
  
}

void loop()                    
{           
  Wire.beginTransmission(CMPS14_address);
  Wire.write(1);
  Wire.endTransmission(false);
  Wire.requestFrom(CMPS14_address,1,true);
  if(Wire.available()>=1){
    raw = Wire.read();
    Serial.println(raw);
  }
  bearing = map(raw,0,255,0,365);
  
  lcd.setCursor(0,0);
  lcd.println("The bearing value is: ");
  lcd.setCursor(0,1);
  lcd.println(bearing);
  lcd.setCursor(0,2);
  lcd.println("The direction is: ");
  lcd.setCursor(0,3);
  lcd.println(DIR(bearing));
  delay(500);
  lcd.clear();

}
                                
                            
String DIR(int bearing){
  if (bearing>=0&&bearing<=5){ 
    dir="N";
  }else 
  if(bearing>=90&&bearing<=95){
    dir="E";
  }else
  if(bearing>=180&&bearing<=185){
    dir="S";
  }else
  if(bearing>=270&&bearing<=275){
    dir="W";
  }else
  if(bearing>5&&bearing<90){
    dir="NE";
  }else
  if(bearing>95&&bearing<180){
    dir="SE";
  }else
  if(bearing>185&&bearing<270){
    dir="SW";
  }else
  if(bearing>275&&bearing<360){
    dir="NW";
  }
  return dir;
}