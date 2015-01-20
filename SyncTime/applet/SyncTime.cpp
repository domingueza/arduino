// DateTime.pde
// example sketch for the DateTime library

#include <DateTime.h>
#include <DateTimeStrings.h>

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'A'   // Header tag for serial time sync message

#include "WProgram.h"
void setup();
void  loop();
boolean getPCtime();
boolean timeSynced = false;

void setup(){
  Serial.begin(19200);
  pinMode(13,OUTPUT); // we flash the LED each second
}

void  loop(){
  unsigned long  prevtime;
  if (!timeSynced) {
    timeSynced = getPCtime();
    if(timeSynced) {  // try to get time sync from pc
      Serial.print("Clock synced at: ");
      Serial.println(DateTime.now(),DEC);
    }
  }
  if(timeSynced && DateTime.available()) { // update clocks if time has been synced
    digitalWrite(13,LOW);  // first flash the LED
    prevtime = DateTime.now();
    while( prevtime == DateTime.now() )  // wait for the second to rollover
        ;
    DateTime.available(); //refresh the Date and time properties

    // send our time to any app at the other end of the serial port
    Serial.print( TIME_HEADER,BYTE); // this is the header for the current time
    Serial.println(DateTime.now());
    digitalWrite(13,HIGH);
  }
  delay(100); 
}

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

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

