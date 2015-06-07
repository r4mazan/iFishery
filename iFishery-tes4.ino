#include <SoftwareServo.h>
#include <SoftwareSerial.h>


#define echoPin 6             // the SRF05's echo pin
#define initPin 7             // the SRF05's init pin
#define VCC2  5
#define numReadings 5        // set a variable for the number of readings to take

char incoming;
SoftwareSerial mySerial(4,5);
String str;
SoftwareServo leftRightServo; // set a variable to map the servo
int leftRightPos = 0;         // set a variable to store the servo position 
int index = 0;                // the index of the current reading
int total = 0;                // the total of all readings
int average = 0;              // the average             
unsigned long pulseTime = 0;  // variable for reading the pulse
unsigned long distance = 0;   // variable for storing distance
int distanceArray[30];
int sensor[30];
int n=0,c=0,x=0;

void setup() {
  // put your setup code here, to run once:
  leftRightServo.attach(9);
  leftRightServo.setMaximumPulse(2200);
  pinMode(initPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //pinMode(VCC2, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(2400);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  n = 0;
  for(leftRightPos = 0; leftRightPos < 180; leftRightPos++) {  // going left to right.
    leftRightServo.write(leftRightPos);
      for (index = 0; index<=numReadings;index++) {            // take x number of readings from the sensor and average them
          digitalWrite(initPin, LOW);
          delayMicroseconds(50);
          digitalWrite(initPin, HIGH);                           // send signal
          delayMicroseconds(50);                                 // wait 50 microseconds for it to return
          digitalWrite(initPin, LOW);                            // close signal
          pulseTime = pulseIn(echoPin, HIGH);                    // calculate time for signal to return
          distance = pulseTime/58;                               // convert to centimetres
          total = total + distance;                              // update total
          delay(10);
      }
    SoftwareServo::refresh();
    average = total/numReadings;                               // create average reading
 
    if (index >= numReadings)  {                               // reset the counts when at the last item of the array
      index = 0;
      total = 0;
    }
    Serial.print("X");                                         // print leading X to mark the following value as degrees
    Serial.print(leftRightPos);                                // current servo position
    Serial.print("V");                                         // preceeding character to separate values
    Serial.println(average);                                   // average of sensor readings
    
    if(leftRightPos%6==0){
      distanceArray[n]=average;
      sensor[n] = analogRead(6);
      if(n < 29){
        n++;
      }
    }
  }
  for(leftRightPos = 180; leftRightPos > 0; leftRightPos--) {  // going right to left
    leftRightServo.write(leftRightPos);
    for (index = 0; index<=numReadings;index++) {
      digitalWrite(initPin, LOW);
      delayMicroseconds(50);
      digitalWrite(initPin, HIGH);
      delayMicroseconds(50);
      digitalWrite(initPin, LOW);
      pulseTime = pulseIn(echoPin, HIGH);
      distance = pulseTime/58;
      total = total + distance;
      delay(10);
    }
    SoftwareServo::refresh();
    average = total/numReadings;
    if (index >= numReadings)  {
      index = 0;
      total = 0;
    }
    Serial.print("X");
    Serial.print(leftRightPos);
    Serial.print("V");
    Serial.println(average);
   
   if(leftRightPos%6==0){
      distanceArray[n]=average;
      sensor[n] = analogRead(6);
      if(n < 29){
        n++;
      }
    }
   }
  for(leftRightPos = 0; leftRightPos < 180; leftRightPos++) {  // going left to right.
    leftRightServo.write(leftRightPos);
      for (index = 0; index<=numReadings;index++) {            // take x number of readings from the sensor and average them
          digitalWrite(initPin, LOW);
          delayMicroseconds(50);
          digitalWrite(initPin, HIGH);                           // send signal
          delayMicroseconds(50);                                 // wait 50 microseconds for it to return
          digitalWrite(initPin, LOW);                            // close signal
          pulseTime = pulseIn(echoPin, HIGH);                    // calculate time for signal to return
          distance = pulseTime/58;                               // convert to centimetres
          total = total + distance;                              // update total
          delay(10);
      }
    SoftwareServo::refresh();
    average = total/numReadings;                               // create average reading
 
    if (index >= numReadings)  {                               // reset the counts when at the last item of the array
      index = 0;
      total = 0;
    }
    Serial.print("X");                                         // print leading X to mark the following value as degrees
    Serial.print(leftRightPos);                                // current servo position
    Serial.print("V");                                         // preceeding character to separate values
    Serial.println(average);                                   // average of sensor readings
    
    if(leftRightPos%6==0){
      distanceArray[n]=average;
      sensor[n] = analogRead(6);
      if(n < 29){
        n++;
      }
    }
  }

   Serial.write("sensor value gotten : ");
   for(x=0;x<30;x++){
       Serial.print(distanceArray[x]);
       Serial.print(sensor[x]);
   }
   Serial.write("\n");
   
  mySerial.write("AT+SAPBR=3,1,\"APN\",\"internet\"\x0d");
  while(mySerial.available() > 0){
     incoming = mySerial.read();
     Serial.write(incoming);
  }  
  delay(3000);
  while(mySerial.available() == 0);
  while(mySerial.available() > 0){
     incoming = mySerial.read();
     Serial.write(incoming);
  }
 
  mySerial.write("AT+SAPBR=1,1\x0d");
  while(mySerial.available() > 0){
     incoming = mySerial.read();
     Serial.write(incoming);
  }
  delay(3000);
  while(mySerial.available() == 0);
  while(mySerial.available() > 0){
     incoming = mySerial.read();
     Serial.write(incoming);
  } 
  
  mySerial.write("AT+HTTPINIT\x0d");
  while(mySerial.available() > 0){
     incoming = mySerial.read();
     Serial.write(incoming);
  }
  delay(1000);
  while(mySerial.available() == 0);
  while(mySerial.available() > 0){
     incoming = mySerial.read();
     Serial.write(incoming);
  } 
    
  for(x=0;x<30;x++){
  
    Serial.write("Sending ");
    Serial.print(x);
    Serial.write("th data");
    
    mySerial.write("AT+HTTPPARA=\"URL\",\"http://184.106.153.149/update?key=HROXWG39QFLE9HB1&field1=");
    mySerial.print(sensor[x]);
    mySerial.write("&field2=");
    mySerial.print(distanceArray[x]);
    mySerial.write("\"\x0d");
    while(mySerial.available() > 0){
       incoming = mySerial.read();
       Serial.write(incoming);
    }
    delay(1500);
    while(mySerial.available() == 0);
    while(mySerial.available() > 0){
       incoming = mySerial.read();
       Serial.write(incoming);
    }
    
    Serial.write("\n\n");
    mySerial.write("AT+HTTPACTION=0\x0d");
    while(mySerial.available() > 0){
       incoming = mySerial.read();
       Serial.write(incoming);
    }
    delay(1500);
    while(mySerial.available() == 0);
    while(mySerial.available() > 0){
       incoming = mySerial.read();
       Serial.write(incoming);
    } 
  }
          
  mySerial.write("AT+HTTPTERM\x0d");
  while(mySerial.available() > 0){
     incoming = mySerial.read();
     Serial.write(incoming);
  }
  delay(2000);
  while(mySerial.available() == 0);
  while(mySerial.available() > 0){
     incoming = mySerial.read();
     Serial.write(incoming);
  }
}

