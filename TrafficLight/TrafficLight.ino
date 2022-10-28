#include <LiquidCrystal.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
const int RED = A15, YELLOW = A14, GREEN = A13 ;
int DELAY_RED = 2000, DELAY_YELLOW = 500, DELAY_GREEN = 2000;
double U_pot, U_thr;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  delay(500);
  lcd.clear();
  pinMode(RED, OUTPUT);
  pinMode(YELLOW,OUTPUT);
  pinMode(GREEN,OUTPUT);
}

void loop()
{
  green_light();
  delay(DELAY_GREEN);
  yellow_light();
  delay(DELAY_YELLOW);
  red_light();
  delay(DELAY_RED);
  int sensorValue = analogRead(A12);
  U_pot =  sensorValue * (5.0 / 1023.0);
  U_thr =  sensorValue * (3.3 / 1023.0);
  lcd.setCursor(0,0);
  lcd.print("Pot.meter = ");
  lcd.print(U_pot);
  lcd.setCursor(0,2);
  lcd.print("3,3level = ");
  lcd.print(U_thr);
}

void green_light()
{
  digitalWrite(GREEN, HIGH);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
}

void yellow_light()
{
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(RED, LOW);
}

void red_light()
{
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, HIGH);
}