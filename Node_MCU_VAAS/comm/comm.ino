#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "IOT"
#define WIFI_PASSWORD "12345678"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCw-UQoH6-rkoqI2Mqv27hdjJh5EHsGlbc"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://vehicleaccident-3819b-default-rtdb.firebaseio.com/"


#include<SoftwareSerial.h>
SoftwareSerial mySerial(D1,D2);

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

char c;
String dataIn;
int8_t indexOfA,indexOfB;
String Vib_measurement = "";
String flame_status = "";

void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  }

void loop()
{
  while(mySerial.available()>0)
  {
    c = mySerial.read();

    
    Serial.println(Vib_measurement);
    Serial.println(flame_status);
    Serial.println("=======================================");
    
    if(c == '\n')
    {
      break;
      }
     else
     {
      dataIn+=c;
      }
    }

  if(c == '\n')
  {
    parse_the_data();


    c = 0;
    dataIn = "";
    
    }

    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setString(&fbdo, "status/vib_measurement", Vib_measurement)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    count++;
    
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setString(&fbdo, "status/flame_status",flame_status)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
    
  }


  void parse_the_data()
  {
    indexOfA = dataIn.indexOf("A");
    indexOfB = dataIn.indexOf("B");

    Vib_measurement = dataIn.substring(0,indexOfA);
    flame_status = dataIn.substring(indexOfA+1,indexOfB);
    }
