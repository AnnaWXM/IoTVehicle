/*

ESP Steering wheel example 14.11.2019 KN
  
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>


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


// Replace with your network credentials
//const char* ssid = "mokkula_925936";
//const char* password = "12345678";

const char* ssid = "SOURCE";
const char* password = "Pelle!23";


#define TESTFILE "/index.html"
bool    spiffsActive = false;


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


 // Replaces placeholder with LED state value
String processor(const String& var)
{
;  
}
  


 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);

Serial.println("-------------");

Serial.println("-------------");

  // Start filing subsystem
  if (SPIFFS.begin()) {
      Serial.println("SPIFFS Active");
      Serial.println();
      spiffsActive = true;
  } else {
      Serial.println("Unable to activate SPIFFS");
  }


  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  
  

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);

 //     Serial.println(" HTML GET ");


int paramsNr = request->params();

 //   Serial.println(" ------ ");
 //   Serial.print("Param qty: ");
 //   Serial.println(paramsNr);
      
for (byte i=0;i<paramsNr;i++)
 {

     AsyncWebParameter* p = request->getParam(0);
 
   //  Serial.print("Param name: ");
   //  Serial.println(p->name());

   if ((p->name())=="dist"){ Serial.print("Move:");Serial.println(p->value()); };
   if ((p->name())=="dire"){ Serial.print("Turn:");Serial.println(p->value()); };
      
   //  Serial.print("Param value: ");
   //  Serial.println(p->value());
 }
  ;

      
  });


    // Route to load style.css file
  server.on("/jquery-1.11.3.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/jquery-1.11.3.min.js", "text/javascript");

      Serial.println(" JS jquery GET ");
      
  });

    // Route to load style.css file
  server.on("/roundslider.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/roundslider.min.js", "text/javascript");

    Serial.println(" min JS GET ");
      
  });

  
  // Route to load style.css file
  server.on("/roundslider.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/roundslider.min.css", "text/css");
    
    Serial.println(" min css GET ");
    
  });

  // Route to set GPIO to HIGH
  server.on("/G---", HTTP_GET, [](AsyncWebServerRequest *request){
    
    request->send(SPIFFS, "/index.html", String(), false, processor);
    
      Serial.println(" ---- X ----- ");
  
  });

 


  // Start server
  server.begin();
}
 
void loop()                    
{           

if (Serial.available() > 0){
    String message = Serial.readStringUntil('\n'); 
    Serial.print("Message received, content: ");  
    Serial.println(message);
    int pos_s = message.indexOf("Move");

    if (pos_s > -1){
      Serial.println("Command = Move ");
      pos_s = message.indexOf(":");

      
        String stat = message.substring(pos_s + 1);
        distVal = stat.toInt();
        Serial.print("Distance:");
        Serial.println(distVal);
      
    }
    else{
      Serial.println("No message found, try typing dist:text\n");
    }
  }


  while (count_L*DistancePerPulse<distVal) {                    //forward 10cm

  //Serial.println(count_L*DistancePerPulse);
    digitalWrite(Motor_L_dir_pin, 1);
    analogWrite(Motor_L_pwm_pin, 254);
    digitalWrite(Motor_R_dir_pin, 1);
    analogWrite(Motor_R_pwm_pin, 254);
  }

    analogWrite(Motor_L_pwm_pin, 0);
    analogWrite(Motor_R_pwm_pin, 0);
    delay(500);


    Serial.println(count_L);

    delay(5000);
    
}

void isr_L() {
    count_L++;
}

void isr_R() {
    count_R++;
}
