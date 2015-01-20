/*
 *  PreConSensorNew.cpp
 *  
 *
 *  Created by Tony Kelly II on 8/4/09.
 *  Copyright 2009 UNL. All rights reserved.
 *
 */


#include <DateTimeStrings.h>
#include <DateTime.h>

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  255   // Header tag for serial time sync message

/*
 *  Author: Tony Kelly
 *  Email: tkelly72@gmail.com
 *
 *  The PreConSensor code is meant to be used with the Arduino Nano microcontroller, as well as, the HS-2000V Precon sensor.
 *    This sensor is able to measure both relative humidity from 0 to 100% and the Temperature from -30 to 100 Celsius.  It
 *    arrives from the manufacturer already calibrated.  This code attempts to take input into analog pins then record and
 *    graph that information over time.
 *
 *  Special Note 1: The analogRead function takes a parameter that specifies the pin number to be read.  For example, 
 *    analogRead(0) reads out the signal on A0 pin of Nano board.  Since it takes a 5V input, it will spread the 0 to
 *    5 volt range over a read out value of 0 to 1023.  This means that the number returned is a unit of .0049 volts.
 *    If the analogRead returns 200 (for example), then it must have read 200*.0049 = 0.98 Volts on the analog pin.
 *    This might need to be modified by whoever uses this code.  For example, your own powersupply might actually give
 *    4.95 volts which would actually give 4.95/1024 = .00483 volts.
 */

int MODTemp = 0;
int MODHumid = 1;
int FPGATemp = 2;
int FPGAHumid = 3;
int Holder = 0;

int ledPin = 13;

float previous[4] = {0,0,0,0};
float present[4]  = {0,0,0,0};

int readswitch = 0;


void setup()
{
	pinMode(ledPin, OUTPUT);
	pinMode(MODTemp, INPUT);
	pinMode(MODHumid, INPUT);
	pinMode(FPGATemp, INPUT);
	pinMode(FPGAHumid, INPUT);
	Serial.begin(9600);

	Holder = analogRead(MODTemp);
	present[0] = 0.12695313*Holder-30;
	Holder = analogRead(MODHumid);
	present[1] = 0.09765625*Holder;
	Holder = analogRead(FPGATemp);
	present[2] = 0.12695313*Holder-30;
	Holder = analogRead(FPGAHumid);
	present[3] = 0.09765625*Holder;
                
                digitalClockDisplay();
		Serial.print(" ");
		Serial.print(present[1]);
		Serial.print(" ");
		Serial.print(present[2]);
		Serial.print(" ");
		Serial.print(present[3]);
		Serial.print(" ");
		Serial.print(present[4]);
		Serial.println(" ");
}

void loop()
{
    readswitch = 0;
    analogReference(EXTERNAL);
    Holder = analogRead(MODTemp);
    Holder = analogRead(MODTemp);
    present[0] = 0.12695313*Holder-30;
    Holder = analogRead(MODHumid);
    present[1] = 0.09765625*Holder;
    Holder = analogRead(FPGATemp);
    present[2] = 0.12695313*Holder-30;
    Holder = analogRead(FPGAHumid);
    present[3] = 0.09765625*Holder;
    if (present[0]-previous[0] > 0.30) {
      		digitalClockDisplay();
		Serial.print(" ");
		Serial.print(present[1]);
		Serial.print(" ");
		Serial.print(present[2]);
		Serial.print(" ");
		Serial.print(present[3]);
		Serial.print(" ");
		Serial.print(present[4]);
		Serial.println(0x1A,BYTE);
		readswitch=1;
                for (int i=0; i<4 ; ++i){
		previous[i] = present[i];
                }  
	}
	if(readswitch != 1) {
		if (present[1]-previous[1] > 0.30) {
  			digitalClockDisplay();
			Serial.print(" ");
			Serial.print(present[0]);
			Serial.print(" ");
			Serial.print(present[1]);
			Serial.print(" ");
			Serial.print(present[2]);
			Serial.print(" ");
			Serial.print(present[3]);
			Serial.println(0x1A,BYTE);
                        for (int i=0; i<4 ; ++i){
		        previous[i] = present[i];
                        }  
	        }
	}
        digitalWrite(ledPin, HIGH);
	delay(100);
	digitalWrite(ledPin, LOW);
	delay(100);
	digitalWrite(ledPin, HIGH);
	delay(100);
	digitalWrite(ledPin, LOW);
}
/*
boolean getPCtime() {
	// if time sync available from serial port, update time and return true
	while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of a header and ten ascii digits
		if( Serial.read() == TIME_HEADER ) {        
			time_t pctime = 0;
			for(int i=0; i < TIME_MSG_LEN -1; i++){   
				char c= Serial.read();          
				if( c >= '0' && c <= '9'){   
					pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
				}
			}   
			DateTime.sync(pctime);   // Sync Arduino clock to the time received on the serial port
			return true;   // return true if time message received on the serial port
		}  
	}
	return false;  //if no message return false
}
*/

void digitalClockDisplay(){
	Serial.print(DateTime.Hour,DEC);
	printDigits(DateTime.Minute);
	printDigits(DateTime.Second);
	Serial.print(" ");
	Serial.print(DateTimeStrings.dayStr(DateTime.DayofWeek));
	Serial.print(" ");
	Serial.print(DateTimeStrings.monthStr(DateTime.Month));
	Serial.print(" ");
	Serial.print(DateTime.Day,DEC); 
}

void printDigits(byte digits){
	// utility function for digital clock display: prints preceding colon and leading 0
	Serial.print(":");
	if(digits < 10)
		Serial.print('0');
	Serial.print(digits,DEC);
}
