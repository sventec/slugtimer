//Reed Simon 7.17.17
#include <LiquidCrystal.h>

LiquidCrystal disp(12, 11, 7, 6, 5, 4); //Creates LCD with specified pins with name disp

const byte outPin = 9;
const byte potPin = A0;

int readVal;

void setup(){
    disp.begin(16, 2);
    pinMode(potPin, INPUT);
    pinMode(outPin, OUTPUT);
}

void loop(){
    readVal = map(analogRead(potPin), 0, 1023, 0, 255);
    analogWrite(outPin, readVal);
    disp.print(readVal);
    delay(40);
    disp.clear();
}

