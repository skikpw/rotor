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

void setup() {
  Serial.begin(9600); //Uruchomienie komunikacji
  pinMode(lewa, OUTPUT);
  pinMode(prawa, OUTPUT);
  pinMode(10, INPUT_PULLUP); //Przycisk
  digitalWrite(lewa, LOW);
  digitalWrite(prawa, LOW);
  Serial.print("krec silnikami");
  Serial.print(impuls);
  Serial.print("\n");
  Serial.println(aktualny_kat_poziom);
}



void loop(){
    
if(Serial.available()>0){
  
   zadany_kat_poziom = Serial.parseFloat();
   delay(5000);
   zadany_kat_pion = Serial.parseFloat();
   //Serial.println(zadany_kat_poziom);
   //Serial.println(zadany_kat_pion);

   //jezeli kat wiekszy niz 360 w poziomie i 180 w pionie skaluje
   while(zadany_kat_poziom>360){
    zadany_kat_poziom = zadany_kat_poziom - 360;
   }
   Serial.println(zadany_kat_poziom);
   while(zadany_kat_pion>180){
    zadany_kat_pion = zadany_kat_pion - 180;
   }
    
    //sprawdza w ktora strone powinien skrecac
    //nie przechodzi nigdy orzez 0 zeby nie splatac kabla

    Serial.println("skrecam w poziomie");
    wynik = aktualny_kat_poziom - zadany_kat_poziom;
    Serial.println(wynik);
    if ( wynik > dokladnosc ){
        skrecaj_w_prawo(wynik);
    }else{
        skrecaj_w_lewo(wynik);
    }
    Serial.println("skrecam w pionie");
    //w pionie
    wynik = aktualny_kat_pion - zadany_kat_pion;
    if(wynik < dokladnosc){
        skrecaj_w_gore(wynik);
    }else{
        skrecaj_w_dol(wynik);
    }
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
