int fsrPin = 0;     // the FSR and 1K pulldown are connected to a0
int fsrReading;     // the analog reading from the FSR resistor divider
int belt = 4;       // digital pin for helmet belt
int val;
int led = 13;


void setup() {
    Serial.begin(9600); //Sets the baud for serial data transmission                                 
    pinMode(led, OUTPUT); //Sets digital pin 13 as output pin  
    pinMode(belt, INPUT_PULLUP);
}

void loop() {
  int count = 0;
  //  ****************** helmet force sensor code part ***********************
        fsrReading = analogRead(fsrPin); 
//        Serial.print("fsr: "); Serial.println(fsrReading);
        if (fsrReading > 200 && fsrReading < 400)      //define range for FSR sensor.
        {
          count = count + 1;
        }  
  // ************* helmet buckle switch code part *************************
        val = digitalRead(belt);       
        if (val == 0 )
        {
          count = count + 1;
        }

// ****************** helmet ON/OFF detection part *****************************
        if (count == 2)
        {
          Serial.flush();
          digitalWrite(13, HIGH);   
          Serial.println("ON");
          delay(1000);
        }
        else
        {
//          Serial.flush();
          digitalWrite(13, LOW);   
          Serial.println("OFF");
          delay(1000);
        }
}
