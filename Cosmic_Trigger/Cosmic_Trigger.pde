#include <DateTimeStrings.h>
#include <DateTime.h>

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'A'   // Header tag for serial time sync message

int refPin = 0;
int tempPin = 2;
int humidPin = 1;
int ledPin = 13;

volatile long t1 = -1;
volatile long t2 = -3;
unsigned long eventNumber = 0;
boolean timeSynced = false;

void setup()
{
  pinMode(ledPin, OUTPUT);
  attachInterrupt(0, scintillator1, FALLING);
  attachInterrupt(1, scintillator2, FALLING);
  Serial.begin(115200);
  
  while (!timeSynced) {
    timeSynced = getPCtime();
    if(timeSynced) {  // try to get time sync from pc
      Serial.print("Clock synced at: ");
      Serial.println(DateTime.now(),DEC);
    }
  }
}

void loop()
{
  if (t1 == t2) { // got a hit in both scintillators in the past millisecond
    digitalWrite(ledPin, HIGH);
//    delay(20);

  // Read temperature and humidity and record time of day for the hit
    int ref   = analogRead(refPin);
    int temp  = analogRead(tempPin);
    int humid = analogRead(humidPin);

    float temperatureC = (temp*1.0)/(ref*1.0)*130.0 - 30.0;  // min -30 C, max 100 C
    float temperatureF = 9*temperatureC/5 + 32.0;
    float humidity = 100.0*(humid*1.0)/(ref*1.0); // min 0% RH, max 100% RH
    DateTime.available(); //refresh the Date and time properties

  // Write data to serial port
  // send our time to any app at the other end of the serial port
    Serial.print("A cosmic trigger number ");
    Serial.print(eventNumber++);
    Serial.print(" at ");
//    Serial.print( TIME_HEADER,BYTE); // this is the header for the current time
    Serial.print(DateTime.now());
    Serial.print(" ");
    Serial.print(temperatureF);
    Serial.print(" F ");
    Serial.print(humidity);
    Serial.println(" %RH");  
  
    digitalWrite(ledPin,LOW);
    t1 = -1; // reset timers
    t2 = -3;
  }
}

void scintillator1()
{
  t1 = millis();
//  Serial.print("t1 ");
//  Serial.println(t1);
}

void scintillator2()
{
  t2 = millis();
//  Serial.print("t2 ");
//  Serial.println(t2);
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

