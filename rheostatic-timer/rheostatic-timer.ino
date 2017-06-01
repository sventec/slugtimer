// 6.1.17

// TO-DO
// Remove rHourS, rMinS, sHourS, and sMinS variables.
// Volatile 

#include <LiquidCrystal.h> //LCD Library
#include <Wire.h> //Wire lib, used for RTC (maybe)
#include "RTClib.h" //Adafruit RTC library for use with ds3231

//External sensors/interactive devices init here
LiquidCrystal disp(12, 11, 7, 6, 5, 4); //Creates LCD with specified pins
RTC_DS3231 rtc;

//Pins init here
const byte pot1 = A0; //Analog pin of POT1
const byte pot2 = A1; //Analog pin of POT2
const byte btnIntPin = 3;
const byte btnDonePin = 2;
const byte outPin = 8;

//Other vars init here
volatile int potVals[2]; //Stores the values of each pot in an array
const byte changeTime = 1; // Time it takes to transition, in hours
volatile bool timeSet = false;

volatile int rHour;
volatile int rMin;
int rEndHour;
int rHourS;
int rMinS;

volatile int sHour;
volatile int sMin;
int sEndHour;
int sHourS;
int sMinS;

void dispStartMsg(){ //Runs "startup" sequence
    disp.print("Timer v.1.00");
    disp.setCursor(0, 1);
    disp.print("Reed Simon 2017");
    delay(1250);
    disp.clear();
    return;
}

void dispRisePrefix(byte prefix){
    disp.setCursor(0, 0);
    disp.print("Rise Start: ");
    disp.print(prefix);
}

void dispSetPrefix(byte prefix){
    disp.setCursor(0, 1);
    disp.print("Set Start: ");
    disp.setCursor(12, 1);
    disp.print(prefix);
}

void potRead(int vals[]){ //Reads values of each POT and modifies array to have current values
    vals[0] = analogRead(pot1);
    vals[1] = analogRead(pot2);
    return;
}

void serialPrintTimes(){
    /*Serial.print("Rise Hour = ");
    Serial.print(rHour);
    Serial.println();
    Serial.print("Rise Minute = ");
    Serial.print(rMin);
    Serial.println();
    Serial.println();*/
    Serial.print("Set Hour = ");
    Serial.print(sHour);
    Serial.println();
    Serial.print("Set minute = ");
    Serial.print(sMin);
    Serial.println();
}

void setup() { //Setup
    disp.begin(16, 2); //Begin LCD
    Serial.begin(9600);

    pinMode(pot1, INPUT);
    pinMode(pot2, INPUT);
    pinMode(btnIntPin, INPUT_PULLUP);
    pinMode(btnDonePin, INPUT_PULLUP);
    pinMode(outPin, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(btnIntPin), changeVals, LOW);

    if (! rtc.begin()) {
        disp.clear();
        disp.print("clock err, call");
        disp.setCursor(0, 1);
        disp.print("(617)-454-4641");
        while (1);
    }

    if (rtc.lostPower()) {
        disp.clear();
        disp.print("clock err, call");
        disp.setCursor(0, 1);
        disp.print("(617)-454-4641");
        while(1);
    }

    dispStartMsg(); // "welcome" message
    changeVals();
    //dispRisePrefix(0); // set default prefixes before any changes have been applied
    Serial.println("startup finished");
}

void changeVals(){
    while(digitalRead(btnDonePin) == HIGH){
        potRead(potVals); // read pot values to start
        potVals[0] = map(potVals[0], 0, 1023, 500, 800); // map the pot1 val to a value between 0500 and 0800
        if(potVals[0] <= 559){ // SUNRISE FUNCTIONS GO HERE.. start
            dispRisePrefix(0);
            disp.setCursor(13, 0);
            disp.print(potVals[0]);
            rHour = 5;
            rMin = (potVals[0] - 500);
            serialPrintTimes();
        } else if(potVals[0] == 560){
            dispRisePrefix(0);
            disp.setCursor(13, 0);
            disp.print("600");
            rHour = 6;
            rMin = 0;
            serialPrintTimes();
        } else if(potVals[0] <= 619){
            dispRisePrefix(0);
            disp.setCursor(13, 0);
            disp.print(potVals[0] + 40);
            rHour = 6;
            rMin = (potVals[0] - 560);
            serialPrintTimes();
        } else if(potVals[0] == 620){
            dispRisePrefix(0);
            disp.setCursor(13, 0);
            disp.print("700");
            rHour = 7;
            rMin = 0;
            serialPrintTimes();
        } else if(potVals[0] <= 679){
            dispRisePrefix(0);
            disp.setCursor(13, 0);
            disp.print(potVals[0] + 80);
            rHour = 7;
            rMin = (potVals[0] - 620);
            serialPrintTimes();
        } else if(potVals[0] == 680){
            dispRisePrefix(0);
            disp.setCursor(13, 0);
            disp.print("800");
            rHour = 8;
            rMin = 0;
            serialPrintTimes();
        } else if(potVals[0] <= 739){
            dispRisePrefix(0);
            disp.setCursor(13, 0);
            disp.print(potVals[0] + 120);
            rHour = 8;
            rMin = (potVals[0] - 680);
            serialPrintTimes();
        } else if(potVals[0] == 740){
            dispRisePrefix(0);
            disp.setCursor(13, 0);
            disp.print("900");
            rHour = 9;
            rMin = 0;
            serialPrintTimes();
        } else if(potVals[0] <= 799){
            dispRisePrefix(0);
            disp.setCursor(13, 0);
            disp.print(potVals[0] + 160);
            rHour = 9;
            rMin = (potVals[0] - 740);
            serialPrintTimes();
        } else if(potVals[0] == 800){
            dispRisePrefix(1);
            disp.setCursor(13, 0);
            disp.print("000"); // SUNRISE functions END HERE
            rHour = 10;
            rMin = 0;
            serialPrintTimes();
        }

        potVals[1] = map(potVals[1], 0, 1023, 700, 940);
        if(potVals[1] <= 759){ // SUNSET FUNCTIONS GO HERE.. start
            dispSetPrefix(1);
            disp.setCursor(13, 1);
            disp.print(potVals[1]);
            sHour = 17;
            sMin = (potVals[1] - 700);
            serialPrintTimes();
        } else if(potVals[1] == 760){
            dispSetPrefix(1);
            disp.setCursor(13, 1);
            disp.print("800");
            sHour = 18;
            sMin = 0;
            serialPrintTimes();
        } else if(potVals[1] <= 819){
            dispSetPrefix(1);
            disp.setCursor(13, 1);
            disp.print(potVals[1] + 40);
            sHour = 18;
            sMin = (potVals[1] - 760);
            serialPrintTimes;
        } else if(potVals[1] == 820){
            dispSetPrefix(1);
            disp.setCursor(13, 1);
            disp.print("900");
            sHour = 19;
            sMin = 0;
            serialPrintTimes();
        } else if(potVals[1] <= 879){
            dispSetPrefix(1);
            disp.setCursor(13, 1);
            disp.print(potVals[1] + 80);
            sHour = 19;
            sMin = (potVals[1] - 820);
            serialPrintTimes();
        } else if(potVals[1] == 880){
            dispSetPrefix(2);
            disp.setCursor(13, 1);
            disp.print("000");
            sHour = 20;
            sMin = 0;
            serialPrintTimes();
        } else if(potVals[1] <= 939 && potVals[1] > 880){
            int printVal;
            printVal = map(potVals[1], 881, 939, 1, 59);
            dispSetPrefix(2);
            if(printVal < 10){
                disp.setCursor(13, 1);
                disp.print("00");
                disp.setCursor(15, 1);
                disp.print(printVal);
            } else if(printVal > 10){
                disp.setCursor(13, 1);
                disp.print("0");
                disp.setCursor(14, 1);
                disp.print(printVal);
            }
            sHour = 20;
            sMin = printVal;
            serialPrintTimes;
        } else if(potVals[1] == 940){
            dispSetPrefix(2);
            disp.setCursor(13, 1);
            disp.print("100");
            sHour = 21;
            sMin = 0;
            serialPrintTimes(); // SUNSET functions END HERE
        }
        //delay(50);
    }
    if(digitalRead(btnDonePin) == LOW){
        timeSet = true;
    }
}

void riseStart(){
    Serial.println("In rise start before while");
    int fadeProg = 0;
    DateTime rTime = rtc.now();
    while(rTime.hour() != rEndHour){
        DateTime rTime = rtc.now();
        if(rTime.minute() == 0){
            fadeProg = 0;
        } else if(rTime.minute() != 0){
            fadeProg = map(rTime.minute(), 1, 59, 0, 255);
            //Serial.println(fadeProg);
            analogWrite(outPin, fadeProg);
        }
    }
}

void setStart(){
    Serial.println("In set before while");
    int fadeProg = 0;
    DateTime sTime = rtc.now();
    while(sTime.hour() != sEndHour){
        DateTime sTime = rtc.now();
        if(sTime.minute() == 0){
            fadeProg = 255;
        } else if(sTime.minute() != 0){
            fadeProd = map(sTime.minute(), 1, 59, 255, 0);
            analogWrite(outPin, fadeProg);
        }
    }
}

void loop() { //Loops for duration of program uptime
    if(timeSet == true){
        disp.clear();
        disp.setCursor(0, 0);
        disp.print("Time set...");
        delay(1500);
        disp.clear();
        timeSet = false;
    }

    rHourS = rHour;
    rMinS = rMin;
    sHourS = sHour;
    sMinS = sMin;

    DateTime cTime = rtc.now();
    rEndHour = (rHourS + changeTime);
    sEndHour = (sHourS + changeTime);

    if(cTime.hour() == rHourS && cTime.minute() == rMinS){
        Serial.println("Rise Start if has been entered");
        riseStart();
        Serial.println("This is after rise start in the if function");
    } else if(cTime.hour() == sHourS && cTime.minute() == sMinS){
        setStart();
    }

    delay(50);
}
