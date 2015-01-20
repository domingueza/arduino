/*

Example sketch 11

RELAYS

  Use a transistor to drive a relay

  A relay is a electrically-controlled mechanical switch.
  It can control much more voltage and current than an Arduino pin
  (or the transistor included in your kit) can. If you want to use
  the Arduino to control a 120V bulb, coffee maker, or other high-
  power device, a relay is an excellent way to do that. Because
  the relay needs more power to switch than an Arduino pin can
  provide, we'll use a transistor to drive the relay in exactly
  the same way we used a transistor to drive a motor in circuit 12.

  A relay consists of a coil of wire, and switch contacts. When
  you apply power to the coil, it becomes magnetized, and pulls
  the switch contacts closed. Since the switch contacts are 
  completely isolated from the Arduino, you can safely use a
  relay to control normally dangerous voltages (but please only do
  this if you already know how to safely work with high voltage!).
  
  The relay has three contact pins, COM (common), NC (Normally
  Closed), and NO (Normally Open). When the relay is turned off,
  the COM pin is connected to the NC (Normally Closed) pin. When
  the relay is turned on, the COM pin is connected to the NO
  (Normally Open) pin.

  This code is very simple - it turns the relay on for one second,
  and off for one second, the same as the blink sketch!

Hardware connections:

  Transistor:
  
    The transistor has three pins. Looking at the flat side with
    the pins down, the order is COLLECTOR, BASE, EMITTER.
    
    Connect the BASE pin through a 1K resistor to digital pin 2.
    
    Connect the EMITTER pin to GND.
  
  Relay coil:
  
    The relay has pins for a coil (which you use to control the
    relay), and contacts (which you connect to the device you'd
    like to switch). The top or bottom of the relay should have
    a symbol indicating the coil pins.
  
    Connect one side of the coil to the COLLECTOR pin
    on the transistor.

    Connect other side of the coil to 5V.
  
  Diode:

    The relay has a coil that you energize to close the switch.
    When you disconnect power from a coil, the coil will generate
    a voltage spike that can damage the transistor. This diode
    protects the transistor from the voltage spike.

    Connect the side of the diode with the band (cathode) to 5V
    
    Connect the other side of the diode (anode) to the COLLECTOR
    pin of the transistor.

  Relay contacts and LEDs:

    We'll use the relay contacts to turn LEDs on and off, but you
    can use them to switch almost anything on and off.
    
    Connect the COMMON side of the switch to a 330 Ohm resistor.
    Connect the other side of the above resistor to 5V.
    
    Connect the NC (Normally Closed) side of the switch to the
    positive (longer) leg of LED 1.

    Connect the NO (Normally Open) side of the switch to the
    positive (longer) leg of LED 2.
    
    Connect the negative sides (shorter leg) of both LEDs to GND.
 
This sketch was written by SparkFun Electronics,
with lots of help from the Arduino community.
This code is completely free for any use.
Visit http://www.arduino.cc to learn about the Arduino.

Version 2.0 6/2012 MDG
*/


const int relayPin = 2;	    // use this pin to drive the transistor
const int timeDelay = 10000; // delay in ms for on and off phases
const int sensorSupplyPin = 13;  // Use to power temperature sensor so that we can reset the sucker

// You can make timeDelay shorter, but note that relays, being
// mechanical devices, will wear out quickly if you try to drive
// them too fast.

// We'll use analog input 0 to measure the temperature sensor's
// signal pin.

const int temperaturePin = 0;
const int referencePin = 1;
const int gndPin = 2;


void setup()
{
  pinMode(relayPin, OUTPUT);  // set pin as an output
  pinMode(sensorSupplyPin, OUTPUT);  // set pin as an output
  digitalWrite(sensorSupplyPin, HIGH); 
    // In this sketch, we'll use the Arduino's serial port
  // to send text back to the main computer. For both sides to
  // communicate properly, they need to be set to the same speed.
  // We use the Serial.begin() function to initialize the port
  // and set the communications speed.
  
  // The speed is measured in bits per second, also known as
  // "baud rate". 9600 is a very commonly used baud rate,
  // and will transfer about 10 characters per second.
  
  Serial.begin(9600);
}


void loop()                    
{
  // Up to now we've only used integer ("int") values in our
  // sketches. Integers are always whole numbers (0, 1, 23, etc.).
  // In this sketch, we'll use doubleing-point values ("double").
  // doubles can be fractional numbers such as 1.42, 2523.43121, etc.

  // We'll declare three double precision variables
  // (We can declare multiple variables of the same type on one line:)

  double voltage, reference, gnd, degreesC, degreesF;

  // First we'll measure the voltage at the analog pin. Normally
  // we'd use analogRead(), which returns a number from 0 to 1023.
  // Here we've written a function (further down) called
  // getVoltage() that returns the true voltage (0 to 5 Volts)
  // present on an analog input pin.

  resetTemperatureSensor(); // Kludge to deal with drifting voltages
  // until I can install a capacitor
  
  voltage = getVoltage(temperaturePin);
  reference = getVoltage(referencePin);
  gnd = getVoltage(gndPin);
 // voltage = (5.0/reference)*voltage;  // correct for any voltage sag
  
  // Now we'll convert the voltage to degrees Celsius.
  // This formula comes from the temperature sensor datasheet:

  degreesC = (voltage - 0.5) * 100.0;
  
  // While we're at it, let's convert degrees Celsius to Fahrenheit.
  // This is the classic C to F conversion formula:
  
  degreesF = degreesC * (9.0/5.0) + 32.0;
  
  // Now we'll use the serial port to print these values
  // to the serial monitor!
  
  // To open the serial monitor window, upload your code,
  // then click the "magnifying glass" button at the right edge
  // of the Arduino IDE toolbar. The serial monitor window
  // will open.

  // (NOTE: remember we said that the communication speed
  // must be the same on both sides. Ensure that the baud rate
  // control at the bottom of the window is set to 9600. If it
  // isn't, change it to 9600.)
  
  // Also note that every time you upload a new sketch to the
  // Arduino, the serial monitor window will close. It does this
  // because the serial port is also used to upload code!
  // When the upload is complete, you can re-open the serial
  // monitor window.
  
  // To send data from the Arduino to the serial monitor window,
  // we use the Serial.print() function. You can print variables
  // or text (within quotes).

  Serial.print("Ground: ");
  Serial.print(gnd);
  Serial.print("   reference: ");
  Serial.print(reference);
  Serial.print("  voltage: ");
  Serial.print(voltage);
  Serial.print("  deg C: ");
  Serial.print(degreesC);
  Serial.print("  deg F: ");
  Serial.println(degreesF);

  // These statements will print lines of data like this:
  // "voltage: 0.73 deg C: 22.75 deg F: 72.96"

  // Note that all of the above statements are "print", except
  // for the last one, which is "println". "Print" will output
  // text to the SAME LINE, similar to building a sentence
  // out of words. "Println" will insert a "carriage return"
  // character at the end of whatever it prints, moving down
  // to the NEXT line.


//  digitalWrite(relayPin, HIGH);  // turn the relay on
  if (degreesF > 80) {
      digitalWrite(relayPin, HIGH); // turn relay on
  }
  else {
    digitalWrite(relayPin, LOW); // turn relay off
  }
  delay(timeDelay);              // wait for one second
  
//  digitalWrite(relayPin, LOW);   // turn the relay off
  
//  delay(timeDelay);              // wait for one second
} 

void resetTemperatureSensor() {
  digitalWrite(sensorSupplyPin, LOW); // turn sensor off
  delay(100);
  digitalWrite(sensorSupplyPin, HIGH); // turn sensor on
  delay(100);
}

double getVoltage(int pin)
{
  // This function has one input parameter, the analog pin number
  // to read. You might notice that this function does not have
  // "void" in front of it; this is because it returns a doubleing-
  // point value, which is the true voltage on that pin (0 to 5V).
  
  // You can write your own functions that take in parameters
  // and return values. Here's how:
  
    // To take in parameters, put their type and name in the
    // parenthesis after the function name (see above). You can
    // have multiple parameters, separated with commas.
    
    // To return a value, put the type BEFORE the function name
    // (see "double", above), and use a return() statement in your code
    // to actually return the value (see below).
  
    // If you don't need to get any parameters, you can just put
    // "()" after the function name.
  
    // If you don't need to return a value, just write "void" before
    // the function name.

  // Here's the return statement for this function. We're doing
  // all the math we need to do within this statement:

  // This hack is to deal with high impedence sensors.  Need a delay to let the Arduino's ADC switch over
  analogRead(pin);
  delay(100);
  
  return (analogRead(pin) * 0.004882814);
  
  // This equation converts the 0 to 1023 value that analogRead()
  // returns, into a 0.0 to 5.0 value that is the true voltage
  // being read at that pin.
}

// Other things to try with this code:

//   Turn on an LED if the temperature is above or below a value.

//   Read that threshold value from a potentiometer - now you've
//   created a thermostat!

