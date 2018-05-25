
#include <SoftwareSerial.h>

const int lewa = 8;
const int prawa = 9;
float aktualny_kat_poziom =0, zadany_kat_poziom =0, aktualny_kat_pion=0, zadany_kat_pion=0;
float dokladnosc = 0.8;
int ile_impulsow=0;
bool zapetlaj = true;
int czytaj_dane;
float wynik=0;
float impuls = 5.7; // wartosc bedzie odpowiadala katowi o jaki obroci sie stacja jezeli silnik przekreci sie o jeden impuls

void skrecaj_w_prawo(float zmiana_kata);
void skrecaj_w_lewo(float zmiana_kata);
void skrecaj_w_gore(float zmiana_kata);
void skrecaj_w_dol(float zmiana_kata);
void wroc_do_stanu0(){}

SoftwareSerial mySerial(10, 11); // RX, TX
String Data = "";
String Azymut = "";
String Elewacja = "";
float AZvalue = 0.0;
float ELvalue = 0.0;

void setup() {
  pinMode(lewa, OUTPUT);
  pinMode(prawa, OUTPUT);
  
  //Do testow
  pinMode(10, INPUT_PULLUP); //Przycisk
  digitalWrite(lewa, LOW);
  digitalWrite(prawa, LOW);

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
			zadany_kat_poziom = AZvalue;
			zadany_kat_pion = ELvalue;
        }
    }
	//jezeli kat wiekszy niz 360 w poziomie i 180 w pionie skaluje
	zadany_kat_poziom %= 360;
	// Serial.println(zadany_kat_poziom);
	zadany_kat_pion %= 180;
	
	//sprawdza w ktora strone powinien skrecac
	//nie przechodzi nigdy orzez 0 zeby nie splatac kabla

	Serial.println("skrecam w poziomie");
	wynik = aktualny_kat_poziom - zadany_kat_poziom;
	Serial.println(wynik);
	if ( wynik > dokladnosc){
		skrecaj_w_prawo(wynik);
	} else if (wynik < -dokladnosc) {
		skrecaj_w_lewo(wynik);
	} 
	
	Serial.println("skrecam w pionie");
	//w pionie
	wynik = aktualny_kat_pion - zadany_kat_pion;
	if(wynik < dokladnosc){
		skrecaj_w_gore(wynik);
	}else if (wynik >){
		skrecaj_w_dol(wynik);
	}
}
  


void skrecaj_w_prawo(float wynik){                         
    wynik = abs(wynik);                
    ile_impulsow = wynik / impuls;
    Serial.print("skrecam w prawo o ");
    Serial.println(ile_impulsow);
    int ile = 0;
    while(ile!=ile_impulsow){           //silnik kreci sie az nie obroci sie o odpowiednia ilosc impulsow i pela skonczy dzialanie
      digitalWrite(prawa, HIGH);
      if(digitalRead(10) == LOW){
      ile++;                            //gdy odbierze impuls z czujnika inkrementuje wartosc az do uzyskania odpowiedniej liczby obrotow
      Serial.println(ile);
      delay(100);
      }
    }
    digitalWrite(prawa, LOW);
    aktualny_kat_poziom = zadany_kat_poziom;
}

//dziala analogicznie jak w prawo
void skrecaj_w_lewo(float wynik){
    wynik = abs(wynik);
    ile_impulsow = wynik / impuls;
    //Serial.print(ile_impulsow);
    Serial.print("skrecam w lewo o ");
    Serial.println(ile_impulsow);
    int ile = 0;
    while(ile!=ile_impulsow){
      digitalWrite(lewa, HIGH);
      delay(100);
      if(digitalRead(10) == LOW){
      ile++;
      Serial.println(ile);
      delay(100);
      }
    }
    digitalWrite(lewa, LOW);
    aktualny_kat_poziom = zadany_kat_poziom;
}

void skrecaj_w_gore(float wynik){
    wynik = abs(wynik);
    ile_impulsow = wynik / impuls;
    Serial.print("skrecam w gore o ");
    Serial.println(ile_impulsow);
    int ile = 0;
    while(ile!=ile_impulsow){
      digitalWrite(prawa, HIGH);
      if(digitalRead(10) == LOW){
      ile++;
      Serial.println(ile);
      delay(100);
      }
    }
    digitalWrite(prawa, LOW);
    aktualny_kat_poziom = zadany_kat_poziom;
}

void skrecaj_w_dol(float wynik){
    wynik = abs(wynik);
    ile_impulsow = wynik / impuls;
    //Serial.print(ile_impulsow);
    Serial.print("skrecam w dol o ");
    Serial.println(ile_impulsow);
    int ile = 0;
    while(ile!=ile_impulsow){
      digitalWrite(lewa, HIGH);
      delay(100);
      if(digitalRead(10) == LOW){
      ile++;
      Serial.println(ile);
      delay(100);
      }
    }
    digitalWrite(lewa, LOW);
    aktualny_kat_poziom = zadany_kat_poziom;
}
