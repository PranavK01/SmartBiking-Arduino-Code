#include<Wire.h>
const int MPU6050_addr=0x68;
int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;

int buzzer = 4;
int alertLED = 5;
int abortButton = 3;
int relay = 2;
char start = '0';
char alert ='0';

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(4800);
  pinMode(abortButton,INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(alertLED, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(buzzer, LOW);
  digitalWrite(alertLED, LOW);
  digitalWrite(LED_BUILTIN, LOW);
}
void loop(){
  if (Serial.available() > 0)
    {
      start = Serial.read();
      if (start == '1')
      {
       digitalWrite(relay, HIGH);   
       Serial.println("Ignition ON");  //turn the bike ignition ON      can also be deleted
       delay(1000);
      }
      else if (start == '0')
      {
       digitalWrite(relay, LOW);
       Serial.println("Ignition OFF"); // turn the bike ignition OFF    can also be deleted
       delay(1000);
      }
    }  
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
//  Temp=Wire.read()<<8|Wire.read();
//  GyroX=Wire.read()<<8|Wire.read();
//  GyroY=Wire.read()<<8|Wire.read();
//  GyroZ=Wire.read()<<8|Wire.read();
    Serial.print("AccX = "); Serial.print(AccX);      // need to delete
    Serial.print(" || AccY = "); Serial.print(AccY);  // these lines
    Serial.print(" || AccZ = "); Serial.print(AccZ);  // of code.
    Serial.println();
  static int counter = 0;
  delay(500);
    if (AccX > 5000)    //  accelerometer value for detecting accident
       {
          Serial.println("Accident detected");
          //                digitalWrite(buzzer, HIGH);
            digitalWrite(alertLED, HIGH);
            delay(500);
            digitalWrite(alertLED, LOW);
            counter = counter + 1;
            delay(500);
          if ( (digitalRead(abortButton) == 0) || (Serial.read() == 'a') )
            {
                Serial.println("Alert aborted");
                digitalWrite(buzzer, LOW);
                digitalWrite(alertLED, LOW);
                counter = 0;
                delay(9000); // put some delay to allow rider to stable the bike's position.                                
            }
            else if (counter == 11)
            {
                Serial.println("Accident confirmed");
                digitalWrite(buzzer, LOW);
                digitalWrite(alertLED, LOW);
                counter = 0;                
                delay(9000); // put some delay to allow rider to stable the bike's position.
            }
          }
   else
   {                   
     counter = 0;
     Serial.println("Safe");     
   }
     delay(1000);
}
