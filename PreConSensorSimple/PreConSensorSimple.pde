#include <DateTimeStrings.h>
#include <DateTime.h>

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'A'   // Header tag for serial time sync message

int refPin = 0;
int tempPin = 2;
int humidPin = 1;
int ledPin = 13;

float previous[4] = {0,0,0,0};
float present[4]  = {0,0,0,0};

boolean timeSynced = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(tempPin, INPUT);
  pinMode(humidPin, INPUT);
  pinMode(refPin, INPUT);
  Serial.begin(9600);
  
  while (!timeSynced) {
    timeSynced = getPCtime();
    if(timeSynced) {  // try to get time sync from pc
      Serial.print("Clock synced at: ");
      Serial.println(DateTime.now(),DEC);
    }
  }
}

void loop() {

  digitalWrite(ledPin, HIGH);
  delay(100);
  
  int ref   = analogRead(refPin);
  int temp  = analogRead(tempPin);
  int humid = analogRead(humidPin);

  float temperatureC = (temp*1.0)/(ref*1.0)*130.0 - 30.0;  // min -30 C, max 100 C
  float temperatureF = 9*temperatureC/5 + 32.0;
  float humidity = 100.0*(humid*1.0)/(ref*1.0); // min 0% RH, max 100% RH

  unsigned long prevtime = DateTime.now();
  while( prevtime == DateTime.now() )  // wait for the second to rollover
      ;
  DateTime.available(); //refresh the Date and time properties

  // send our time to any app at the other end of the serial port
  Serial.print( TIME_HEADER,BYTE); // this is the header for the current time
  Serial.print(DateTime.now());
  Serial.print(" ");
  Serial.print(temperatureF);
  Serial.print(" F ");
  Serial.print(humidity);
  Serial.println(" %RH");
  
  digitalWrite(ledPin, LOW);
  
  delay(1000);
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

