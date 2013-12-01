#include <SoftwareSerial.h>
#include <OneWire.h> 
#include "TinyGPS.h"

#define HRMI_I2C_ADDR      127
#define HRMI_HR_ALG        1   // 1= average sample, 0 = raw sample

TinyGPS gps;
SoftwareSerial ss(2, 3);

int pp_3vr = 13;
int pp_xb = 6;
int pp_gps = 7;
int pp_nd = 8;

int pin_temp = 5; //DS18S20 Signal pin on digital 5
//Temperature chip i/o
OneWire ds(pin_temp);  // on digital pin 2

int blueled = 10;
int redled = 11;
int ledmaxpwr = 80;

int pin_battery = A3;

boolean printed = false;
char lastchar = 0;
int gps_line_count = 0;
boolean firstrun = true;

float batteryvoltagerating = 7.4;

void setup() {
  Serial.begin(57600);
  Serial.println(" ");
  
  pinMode(pp_3vr, OUTPUT);
  pinMode(pp_xb, OUTPUT);
  pinMode(pp_gps, OUTPUT);
  pinMode(pp_nd, OUTPUT);
  pinMode(blueled, OUTPUT);
  pinMode(redled, OUTPUT);

  ledsw(blueled, ledmaxpwr);

  pwrsw(pp_3vr, HIGH);
  pwrsw(pp_xb, LOW);
  pwrsw(pp_gps, HIGH);
  pwrsw(pp_nd, HIGH);

  //let items boot up and then initialize
  Serial.println("Delay to init...");
  delay(200);
  Serial.println("Initializing...");
  
  ss.begin(9600);

  delay(100);
  Serial.println("Done init.");
}

void loop() {
  
  

  if (ss.available()) {
    while(ss.available()) {
      char ch = ss.read();
      //Serial.write(ch);
      
      if(lastchar == 36 && ch == 71) {
        gps_line_count++;
      }
      lastchar = ch;
    }
    printed = false;
  } else if (!printed) {

    if(gps_line_count > 4 && lastchar == 10) {
      float temperature = getTemp();
      float batval = (((float)analogRead(pin_battery) / 1023.00) * 5.0) * 3.0; //number of volts going into analog pin
      
      Serial.println(" ");
      Serial.print("Temp: ");
      Serial.print(temperature);
      Serial.print(", Batt: ");
      Serial.print(batval);
      Serial.println(" ");
      Serial.println(" ");

      gps_line_count = 0;
    }
    printed=true;
  }
  
  

}






void ledsw(int sled, int pwr) {
  //sled is pin of desired led
  digitalWrite(sled, pwr);
}
void pwrsw(int spin, boolean tostate) {
  digitalWrite(spin, tostate);
}
void gettemp() {

}
void getbattery() {

}
float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
  
}