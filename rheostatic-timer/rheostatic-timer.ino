// 6.05.17

// TO-DO
// Remove rHourS, rMinS, sHourS, and sMinS variables.
// Volatile 

#include <LiquidCrystal.h> //LCD Library
#include <Wire.h> //Wire lib, used for RTC (maybe)
#include "RTClib.h" //Adafruit RTC library for use with ds3231

//External sensors/interactive devices init here
LiquidCrystal disp(12, 11, 7, 6, 5, 4); //Creates LCD with specified pins with name disp
RTC_DS3231 rtc; //Creates new DS3231 RTC with the name rtc

//Pins init here
const byte pot1 = A0; //Analog pin of POT1
const byte pot2 = A1; //Analog pin of POT2
const byte btnIntPin = 3; //Pin of first button
const byte btnDonePin = 2; //Pin of second button
const byte outPin = 9; //Pin to output PWM signal to

//Other vars init here
volatile int potVals[2]; //Stores the values of each pot in an array
const byte changeTime = 1; //Time it takes to transition, in hours
volatile bool timeSet = false; //"Time Set" displays on lcd only once per cycle
bool lastClear = false; //Used to only disp.clear() once per cycle


volatile int rHour; //Volatile Rise Hour value
volatile int rMin; //Volatile Rise Minute value
int rEndHour; //Rise Ending Hour value to be assigned later in loop
int rHourS; //Stable Rise Hour
int rMinS; //Stable Rise Minute

volatile int sHour; //Volatile Set Hour value
volatile int sMin; //Volatile Set Minute value
int sEndHour; //Set Ending Hour value to be assigned later in loop
int sHourS; //Stable Set Hour
int sMinS; //Stable Set Minute


void dispStartMsg(){ //Runs "startup" sequence
    disp.print("Timer v.1.00");
    disp.setCursor(0, 1);
    disp.print("Reed Simon 2017");
    delay(1250);
    disp.clear();
    return;
} //Creates a "welcome message" to be displayed on startup

void dispRisePrefix(byte prefix){
    disp.setCursor(0, 0);
    disp.print("Rise Start: ");
    disp.print(prefix);
} //Used in displaying Rise Time while setting it

void dispSetPrefix(byte prefix){
    disp.setCursor(0, 1);
    disp.print("Set Start: ");
    disp.setCursor(12, 1);
    disp.print(prefix);
} //Used in displaying Set Time while setting it

void potRead(int vals[]){ //Reads values of each POT and modifies array to have current values
    vals[0] = analogRead(pot1);
    vals[1] = analogRead(pot2);
    return;
} //Function to read Pot values and edit the array they're stored in directly
//An array is used because functions reference the original array values while editing but create local copies
//of variables given before editing. If two variables were passed in the values would never be returned withnout additional code

void serialPrintTimes(){
    Serial.println("In changeVals");
} //Editable to change Serial messsage while in changeVals

void setup() { //Runs once at the beginning of arduino pwr cycle (being turned on/reset)
    disp.begin(16, 2); //Initialize the "disp" LCD
    Serial.begin(9600); //Initialze a serial communication with computer

    pinMode(pot1, INPUT); //PinModes for each INPUT/OUTPUT being used
    pinMode(pot2, INPUT);
    pinMode(btnIntPin, INPUT_PULLUP); //Buttons are read for a pressed LOW signal
    pinMode(btnDonePin, INPUT_PULLUP); //Hence the built in pullup resistors being used
    pinMode(outPin, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(btnIntPin), changeVals, LOW); //Interrupt on first button that goes to changeVals when btn pressed

    if (! rtc.begin()) {
        disp.clear();
        disp.print("clock err, call");
        disp.setCursor(0, 1);
        disp.print("(617)-454-4641");
        while (1);
    } //If the RTC can't start or intervface with arduino, error message is displayed and setup stops

    if (rtc.lostPower()) {
        disp.clear();
        disp.print("clock err, call");
        disp.setCursor(0, 1);
        disp.print("(617)-454-4641");
        while(1);
    } //IF the RTC lost power and doesn't remember the time error message is displayed and setup stops

    rtc.adjust(DateTime(2017, 6, 5, 6, 29, 30)); //USED FOR TESTING ONLY sets an inaccurate time on the rtc in format of (Y, M, D, Hour, Min, Sec)

    dispStartMsg(); //Runs welcome message
    changeVals(); //Enters changeVals to initially declare rHour, rMin, sHour, sMin
    //dispRisePrefix(0); // set default prefixes before any changes have been applied --NOT NEEDED?
    Serial.println("startup finished"); //End of setup
}

void changeVals(){
    disp.clear();
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
} //Main function to change rhour, rMin, sHour, sMin and display values on LCD "disp"

void riseStart(){
    Serial.println("In rise start before fade"); //Serial prints for debugging..
    Serial.println(rEndHour);
    int fadeProg = 0;
    int dispProg = 0;
    DateTime rTime = rtc.now();
    while(rTime.hour() != rEndHour || rTime.minute() != rMinS){ //These tests listen for the end hour AND the end minute before exiting
        Serial.print("In rise start during fade: ");
        Serial.println(dispProg);
        Serial.println(fadeProg);
        rTime = rtc.now();
        int newTime;
        Serial.println(newTime);
        if(rMinS != 0){
            if(rTime.minute() > 0 && rTime.hour() == rHourS){ //checking for the correct time
                newTime = (rTime.minute() - rMinS);
            } else if(rTime.minute() >= 0 && rTime.hour() == rEndHour){
                newTime = (rTime.minute() + (60 - rMinS));
            }
        } else{
            newTime = rTime.minute();
        }
        fadeProg = map(newTime, 0, 59, 0, 255);
        analogWrite(outPin, fadeProg);
        delay(60);
        dispProg = map(fadeProg, 0, 255, 0, 100);
        disp.clear();
        disp.setCursor(0, 0);
        disp.print(dispProg);
        disp.print("%");
    }
    digitalWrite(outPin, HIGH);
}

void setStart(){
    Serial.println("In set before while");
    int fadeProg = 0;
    int dispProg = 0;
    DateTime sTime = rtc.now();
    while(sTime.hour() != sEndHour || sTime.minute() != sMinS){ // Loop runs until ending hour and minute are BOTH reached
        Serial.println(dispProg);
        Serial.println(fadeProg);
        sTime = rtc.now();
        int newTime;
        Serial.println(newTime);
        if(sMinS != 0){
            if(sTime.minute() > 0 && sTime.hour() == sHourS){
                newTime = (sTime.minute() - sMinS);
            } else if(sTime.minute() >= 0 && sTime.hour() == sEndHour){
                newTime = (sTime.minute() + (60 - sMinS));
            }
        } else{
            newTime = sTime.minute(); // If sMinS is equal to zero then newTime can = the current time w/o adjustments
        }
        fadeProg = map(newTime, 0, 59, 255, 0);
        analogWrite(outPin, fadeProg);
        dispProg = map(fadeProg, 255, 0, 0, 100);
        disp.clear();
        disp.setCursor(0, 0);
        disp.print(dispProg);
        disp.print("%");
    }
    digitalWrite(outPin, LOW);
}

void loop() { //Loops for duration of arduino uptime (as long as another function isn't running)
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

    if(cTime.second() == 0 && lastClear == false){
        disp.clear();
        lastClear = true;
    } if(cTime.second() == 1 && lastClear == true){
        lastClear = false;
    }
    disp.setCursor(0, 0);
    disp.print(cTime.hour(), DEC);
    disp.print(":");
    disp.print(cTime.minute(), DEC);
    disp.print(":");
    disp.print(cTime.second(), DEC);
    disp.setCursor(0, 1);
    if(cTime.hour() > sEndHour || cTime.hour() <= rHourS){
        if(cTime.hour() > sEndHour){
            disp.print("Rise at: ");
            disp.print(rHourS);
            disp.print(":");
            disp.print(rMinS);
            disp.print(":0");
        } else if(cTime.minute() >= rMinS && cTime.hour() == rHourS){
            //Should be rising while this is true  TODO ERROR MESSAGE IF IT DOESN'T??
        } else if(cTime.minute() < rMinS && cTime.hour() == rHourS){
            // When 
            disp.print("Rise at: ");
            disp.print(rHourS);
            disp.print(":");
            disp.print(rMinS);
            disp.print(":0");
        } else if(cTime.hour() < rHourS){
            disp.print("Rise at: ");
            disp.print(rHourS);
            disp.print(":");
            disp.print(rMinS);
            disp.print(":0");
        }
    } else if(cTime.hour() >= rEndHour && cTime.hour() < sHourS){
        disp.print("Set at: ");
        disp.print(sHourS);
        disp.print(":");
        disp.print(sMinS);
        disp.print(":0");
    } else if(cTime.hour() >= sHourS && cTime.hour() <= sEndHour){
        //Should be setting while this is true
        disp.print("Setting...");
    } else{
        disp.print("Error w/ disp");
    }

    if(cTime.hour() == rHourS && cTime.minute() == rMinS){
        Serial.println("Rise Start if has been entered");
        riseStart();
        Serial.println("This is after rise start in the if function");
        disp.clear();
        disp.print("Sunrise has");
        disp.setCursor(0, 1);
        disp.print("Finished");
        delay(3000);
        disp.clear();
    } else if(cTime.hour() == sHourS && cTime.minute() == sMinS){
        Serial.println("Set Start if has been entered");
        setStart();
        Serial.println("This is after set start in the if function");
        disp.clear();
        disp.print("Sunset has");
        disp.setCursor(0, 1);
        disp.print("Finished");
        delay(3000);
        disp.clear();
    }

    delay(50);
}
