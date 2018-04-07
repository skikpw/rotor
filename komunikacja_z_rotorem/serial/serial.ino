/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
String Data = "";
String Azymut = "";
String Elewacja = "";
float AZvalue = 0.0;
float ELvalue = 0.0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Serial connection success!");
  Serial.println("");
  mySerial.begin(9600);  // set the data rate for the SoftwareSerial port
}

void loop() {

    while (mySerial.available())
    {
        char character = mySerial.read(); // Receive a single character from the software serial port
        Data.concat(character); // Add the received character to the receive buffer
        if (character == '\n')
        {
            Serial.print("Received: " + Data);

            // Add your code to parse the received line here....
            int AZtag = Data.indexOf("AZ");
            int ELtag = Data.indexOf("EL");
            int firstDot = Data.indexOf(".");
            int secondDot = Data.indexOf('.', firstDot + 1);

            //Serial.println("The index of the AZ tag in the string " + Data + " is " + AZtag);
            //Serial.println("The index of the EL tag in the string " + Data + " is " + ELtag);
            //Serial.println("The index of . in the string " + Data + " is " + firstDot);
            //Serial.println("The index of second . in the string " + Data + " is " + secondDot);

            Azymut = Data.substring(AZtag+2, firstDot+2);
            Elewacja = Data.substring(ELtag+2, secondDot+2);
            AZvalue = Azymut.toFloat();
            ELvalue = Elewacja.toFloat();
            
            Serial.println("");
            Serial.println("Azymut: ");
            Serial.println(AZvalue);
            Serial.println("Elewacja: ");
            Serial.println(ELvalue);
            Serial.println("");
      
            // Clear receive buffer so we're ready to receive the next line
            Data = "";
            Azymut = "";
            Elewacja = "";
        }
    }

  
}
