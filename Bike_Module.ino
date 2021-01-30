#include<Wire.h>
const int MPU6050_addr=0x68;
int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;

int buzzer = 4;
int alertLED = 5;
int abortButton = 3;
int relay = 2;
char start = '0';
char alert ='0';
int ride = 0;

void setup(){
Wire.begin();
Wire.beginTransmission(MPU6050_addr);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);

Serial.begin(9600);

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

if (Serial.available())
  {
    start = Serial.read();
    if (start == '1')
    {
       Serial.write('i');  //turn the bike ignition ON      can also be deleted
       digitalWrite(relay, HIGH);  
       ride = 1;
       while(ride > 0)
       {       
          Wire.beginTransmission(MPU6050_addr);
          Wire.write(0x3B);
          Wire.endTransmission(false);
          Wire.requestFrom(MPU6050_addr,14,true);
          AccX=Wire.read()<<8|Wire.read();
          AccY=Wire.read()<<8|Wire.read();
          AccZ=Wire.read()<<8|Wire.read();
          static int counter = 0;
          delay(500);
          if (AccX < 13000 && AccZ < 7000 )    //  accelerometer value for detecting accident
          {
            Serial.println("accident");
            digitalWrite(buzzer, HIGH);
            digitalWrite(alertLED, HIGH);
            delay(500);
            digitalWrite(alertLED, LOW);
            counter = counter + 1;
            delay(2000);
            if ( (digitalRead(abortButton) == 0) || (Serial.read() == 'a') )
            {
              Serial.println("aborted");
              digitalWrite(buzzer, LOW);
              digitalWrite(alertLED, LOW);
              counter = 0;
              delay(15000); // put some delay to allow rider to stable the bike's position.                                
            }
            else if (counter == 10)
            {
              Serial.println("confirmed");
              digitalWrite(buzzer, LOW);
              digitalWrite(alertLED, LOW);
              counter = 0;                
              delay(15000); // put some delay to allow rider to stable the bike's position.
            }
          }
          else if (Serial.read() == '0')
          {
             delay(500);
             Serial.write('o'); // turn the bike ignition OFF    can also be deleted
             digitalWrite(relay, LOW);
             break;
          }  
          else
          {                   
             counter = 0;
             Serial.write('s');
             digitalWrite(buzzer, LOW);
             digitalWrite(alertLED, LOW);
          }
       }
    }
  }
}
