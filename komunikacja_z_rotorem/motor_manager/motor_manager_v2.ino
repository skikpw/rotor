
#include <SoftwareSerial.h>

const int MOTOR_LEFT_AZIMUTH = 8;
const int MOTOR_RIGHT_AZIMUTH = 9;
const int MOTOR_LEFT_ELEVATION = 12;
const int MOTOR_RIGHT_ELEVATION = 11;
// CONSTANTS
int PULSE_PER_ROTATION_AZIMUTH = 3364;
int PULSE_PER_ROTATION_ELEVATION = 2540;
float angle_to_pulse_azimuth = 360.0/PULSE_PER_ROTATION_AZIMUTH;
float angle_to_pulse_elevation = 360.0/PULSE_PER_ROTATION_ELEVATION;
//measured in fotodetector pulses
//konfiguracja
float orientation_azimuth = 0.0;
float orientation_elevation = 0.0;

int current_position_elevation = 0;
int current_position_azimuth = 0;

int target_position_elevation = 0;
int target_position_azimuth = 0;


int margin_azimuth = 5;
int margin_elevation = 5;

struct Motor {
	const int leftPin;
	const int rightPin;
	int control;
	Motor(lpin, rpin): leftPin(lpin), rightPin(rpin) {
		control = 0;
	}
	void rotate(int new_control) { //  < 0 left, 0 stop, > 0 right
		control = new_control;
		if(control < 0) {
			digitalWrite(leftPin, HIGH);
			digitalWrite(rightPin, LOW);
		} else if (control > 0)) {
			digitalWrite(leftPin, LOW);
			digitalWrite(rightPin, HIGH);
		} else {
			digitalWrite(leftPin, LOW);
			digitalWrite(rightPin, LOW);
		}			
	}
};

Motor m_azimuth(MOTOR_LEFT_AZIMUTH, MOTOR_RIGHT_AZIMUTH), m_elevation(MOTOR_LEFT_ELEVATION, MOTOR_RIGHT_ELEVATION);

SoftwareSerial mySerial(10, 11); // RX, TX
String Data = "";
String Azymut = "";
String Elewacja = "";
float AZvalue = 0.0;
float ELvalue = 0.0;

void setup() {
  pinMode(MOTOR_LEFT_AZIMUTH, OUTPUT);
  pinMode(MOTOR_LEFT_ELEVATION, OUTPUT);
  pinMode(MOTOR_RIGHT_AZIMUTH, OUTPUT);
  pinMode(MOTOR_RIGHT_ELEVATION, OUTPUT);
  
  //Do testow
  pinMode(10, INPUT_PULLUP); //Przycisk
  digitalWrite(MOTOR_LEFT_AZIMUTH, LOW);
  digitalWrite(MOTOR_LEFT_ELEVATION, LOW);
  digitalWrite(MOTOR_RIGHT_AZIMUTH, LOW);
  digitalWrite(MOTOR_RIGHT_ELEVATION, LOW);

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
	
    if (mySerial.available()) // while czy if?
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
			target_position_azimuth = round((orientation_azimuth-AZvalue)/angle_to_pulse_azimuth);
			target_position_elevation = round((orientation_elevation-ELvalue)/angle_to_pulse_elevation);
			target_position_azimuth %= PULSE_PER_ROTATION_AZIMUTH;
			target_position_elevation %= PULSE_PER_ROTATION_ELEVATION;
        }
    }
	// powinny byc przerwania - wykrywanie zboczy enkoderow
	//TODO
	//sterowanie silinkiem
	int delta_azimuth = target_position_azimuth - current_position_azimuth;
	if (abs(delta_azimuth) <= margin_azimuth)
		m_azimuth.rotate(0);
	else
		m_azimuth.rotate(delta_azimuth);
	
	int delta_elevation = target_position_elevation - current_position_elevation;
	if (abs(delta_elevation) <= margin_elevation)
		m_elevation.rotate(0);
	else
		m_elevation.rotate(delta_elevation);
}
  

