#include<SoftwareSerial.h>


int Flpin = 7;
int vib_pin = 6;
bool Flame_status = true;

SoftwareSerial arduino_serial(1,2);



  


void setup() {

  Serial.begin(9600);
  pinMode(Flpin,INPUT);
  arduino_serial.begin(9600);

}

void loop() {
  
  
  String flame_status = "False";

  Flame_status = digitalRead(Flpin);

  long measurement = vibration();
  
  if(Flame_status == false)
  {
    flame_status = "True";
    }

  if(measurement >= 5000)
  {
    Serial.print(measurement);Serial.print("A");
    Serial.print(flame_status);Serial.print("B");
    Serial.print('\n');

    arduino_serial.print(measurement);arduino_serial.print("A");
    arduino_serial.print(flame_status);arduino_serial.print("B");

    arduino_serial.print('\n');
    
    }

  

   delay(100);
  
  // put your main code here, to run repeatedly:

}



long vibration()
{
  long measurement = pulseIn(vib_pin,HIGH);
  return measurement;
  }
