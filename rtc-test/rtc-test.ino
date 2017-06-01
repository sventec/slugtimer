#include <Wire.h>
#include "RTClib.h"


RTC_DS3231 rtc;

void setup(){

    rtc.begin();

    Serial.begin(9600);
    Serial.println("startup complete");
    
}

void loop(){
    
    DateTime cTime = rtc.now();
    
    Serial.print(cTime.minute());
    Serial.print(":");
    Serial.print(cTime.second());
    Serial.println();

    DateTime future (cTime + TimeSpan(0,1, 0, 0));
        Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
    
    delay(500);
    
}
