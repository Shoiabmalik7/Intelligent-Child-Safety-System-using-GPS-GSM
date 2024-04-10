#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 600;           // by default 550
                               
static const int RXPin = 4, TXPin = 3;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

int rled = 7;
int gled= 6;
int buz = 8;
int sw = 5;

float lat=17.3724,lng=78.5320;    //Initial Values
int gp=0;

int delay1()
{
int p;
unsigned int count=0;
for(p=0;p<127;p++)
{
  if(analogRead(A0)>550)
  {
    count++;
  }  
}  
return count;
}

void setup() {

  Serial.begin(9600);
  ss.begin(9600);
  dht.begin();
  pinMode(sw,INPUT_PULLUP);
  pinMode(rled,OUTPUT);
  pinMode(gled,OUTPUT);
  pinMode(buz,OUTPUT);
  digitalWrite(rled,0);
  digitalWrite(gled,0);
  digitalWrite(buz,1);
  ss.println("AT");
  delay(1000);
  ss.println("AT+CMGF=1");
  delay(2000);
  digitalWrite(buz,0);
  gp=0;
}

void loop() {
  digitalWrite(rled,0);
  digitalWrite(gled,1);
  delay(1000);
 
  float f = dht.readTemperature(true);
  if (isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  int gsr=analogRead(A1);
 
  int bpm=delay1();
  Serial.print(F("%  Temperature: "));
  Serial.print(f);
  Serial.print(F("°F "));
  Serial.print(" G:");
  Serial.print(gsr);
 
  Serial.print(" P:");
  Serial.println(bpm);
 gp=0;
//  while(gp==0)

  if(digitalRead(sw)==0)
  {
    digitalWrite(rled,1);
    digitalWrite(gled,0);
    digitalWrite(buz,1);
    ss.println("AT+CMGS=\"8790966275\"");
    delay(1000);
    ss.println("ALert... Iam in Danger. I am at this location..");
    ss.print("http://maps.google.co.in/maps?q=");
    ss.print((lat),6);
    ss.print('+');
    ss.print((lng),6);
    ss.println();
    ss.write(0x1A);
    delay(2000);
    while(digitalRead(sw)==0);
    digitalWrite(rled,0);
    digitalWrite(gled,1);
    digitalWrite(buz,0);
    delay(500);    
  }
  int rp=0;
    if(f>99)
    {
      rp='T';
      digitalWrite(buz,1);
      digitalWrite(rled,1);
      delay(200);
      digitalWrite(buz,0);
      digitalWrite(rled,0);
    }
    if(gsr>100)
    {
      rp = 'G';
      digitalWrite(buz,1);
      digitalWrite(rled,1);
      delay(500);
      digitalWrite(buz,0);
      digitalWrite(rled,0);
    }
    if(bpm>90)
    {
      rp='P';
      digitalWrite(buz,1);
      digitalWrite(rled,1);
      delay(1000);
      digitalWrite(buz,0);
      digitalWrite(rled,0);
    }
    if(rp=='T' || rp=='G' || rp=='P')
    {
      ss.println("AT+CMGS=\"8790966275\"");
      delay(1000);
      ss.println("ALert... ");
      if(rp=='T')
      {
        ss.println("Temperature Alert... Please Help. I am at this location..");
      }
      else if(rp=='G')
      {
        ss.println("Skin Response Alert... Please Help. I am at this location..");
      }
      else if(rp=='P')
      {
        ss.println("Pulse Alert... Please Help. I am at this location..");
      }
      
      ss.print("http://maps.google.co.in/maps?q=");
      ss.print((lat),6);
      ss.print('+');
      ss.print((lng),6);
      ss.println();
      ss.write(0x1A);
      delay(2000);
    }    
 
   ss.flush();
   delay(100);
   ss.listen();
  for(int i=0;i<5000;i++)
  {
    if(digitalRead(sw)==0)
  {
    digitalWrite(rled,1);
    digitalWrite(gled,0);
    digitalWrite(buz,1);
    ss.println("AT+CMGS=\"8790966275\"");
    delay(1000);
    ss.println("ALert... Iam in Danger. I am at this location..");
    ss.print("http://maps.google.co.in/maps?q=");
    ss.print((lat),6);
    ss.print('+');
    ss.print((lng),6);
    ss.println();
    ss.write(0x1A);
    delay(2000);
    while(digitalRead(sw)==0);
    digitalWrite(rled,0);
    digitalWrite(gled,1);
    digitalWrite(buz,0);
    delay(500);    
  }
    
    while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();
  
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    //Serial.println(F("No GPS detected: check wiring."));
    delay(100);
  }
  }
  
  delay(100);                    // considered best practice in a simple sketch.

}

void displayInfo()
{
  //Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    gp=1;
    lat=(gps.location.lat());
   // Serial.print(lat,6);
   // Serial.print(F(","));
    lng=(gps.location.lng());
    //Serial.print(lng,6);
  }
  else
  {
    gp=0;
   // Serial.print(F("INVALID"));
  }

}