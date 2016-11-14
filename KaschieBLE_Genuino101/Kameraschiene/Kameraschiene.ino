/*******************************************
*** Dateiname: Kameraschien.ino
********************************************
***  Zweck: Steuerung eines Positioniersystem
***         mittels einer Bluetooth-Applikation
*** Erstellt: 19.10.2016
******************************************
*** Geändert: 07.11.2016
******************************************
*** Änderung: Implementierung der Endschalter-Interrupts
*****************************************/

/*
 * Dieses Programm wurde für einen Genuino101 entwickelt.
 * 
 * Aufgabe des Programms:
 * Das Programm hat den Zweck eine Verbindung über Bluetooth
 * zu einem Perephärie-Gerät herzustellen. Dieses verfügt über eine Applikation,
 * mit welcher die Schiene gesteuert werden kann.
 * Momentan steuerbar, ist der Links- und Rechtslauf in zwei Geschwindigkeitsstufen,
 * sowie die Stopp-Funktionalität.
 * 
 * Ablauf des Programms:
 * - Initialisierung aller Variablen und Konstanten
 * - Initialiserung der Bluetooth-Verbindung und parametrieren dieser
 * - Konfigurieren der Ein- und Ausgänge, zuweisen der Interrupt-Service-Routinen
 * - Zyklisch wird danach abgearbeitet, ob etwas per Bluetooth geschickt wurde,
 *   dass die momentan aktiven Befehle an die Motorsteuerung weitergeleitet werden,
 *   falls eine Fahrtrichtung durch eine Endlagen ISR gesperrt wurde, ob der Abstand
 *   zu dieser Endlage bereits wieder groß genung ist um diese Fahrtrichtung wieder freizugeben.
 *   
 *   
 * Anmerkungen zum Programm-Code:
 * Die noch im Code befindlichen Funktionen der Seriellen Schnittstelle wurden zu Diagnosezwecken,
 * dort belassen. Jedoch wurde alle zum Zweck eines schneller Ansprechverhaltens, auf Signale
 * , sowie eine schneller Verarbeitungsgeschwindigkeit, auskommentiert.
 */




#include "Bluetooth.h"                  // Einbinden der Header-Datei für die Bluetooth-Funktionalität
#include "Encoder.h"                      // Einbinden der Header-Datei für die Encoder-Funktionalität
#include "Motorcontrol.h"                 // Einbinden der Header-Datei für die Motorsteuerung



BLEPeripheral Genuino101;
BLEService Positioniersystem("180A"); // BLE Posistioniersystem Service
BLEUnsignedCharCharacteristic  Positionierbefehl("180B", BLERead | BLEWrite);
BLEUnsignedIntCharacteristic    Sollposition("180C", BLERead | BLEWrite);
BLEUnsignedIntCharacteristic    Istposition("180D", BLERead | BLENotify);
BLEUnsignedCharCharacteristic Zustand_BLE("180E", BLERead | BLENotify);



/******Encodervariablen********/
const uint8_t encoder0PinA = 2;               //Definieren der Pins für die Encodereingänge
const uint8_t encoder0PinB = 4;

volatile unsigned int encoder0Pos = 2000;    //Deklarieren der Zählvariable des Encoders

/******Motorvariablen**********/
const uint8_t PinRichtung = 12;               //Definieren des Pins für die Drehrichtungssteuerung
const uint8_t PinGeschwindigkeit = 3;         //Definieren des Pins für die Verfahrgeschwindigkeit


const uint8_t RefGeschw = 255;                //Definieren der Verfahrgeschwindigkeit für die Referenzfahrt
/***** Endschalterparameter *******/
const uint8_t EndschL = 6;
const uint8_t EndschR = 7;                     //Definieren des Pins für das Endschalter/Referenzsignal

bool EndlageL = true;                        // Variable zum Speichern einer Erreichten Endlage
bool EndlageR = true;

unsigned int LastISR = 0;

bool Endl_ISR1 = false;                       //Hilfsvariable zur Erfassung ob EndlagenISR bereits ausgelöst wurde
const uint8_t Hyst=10;                        // Definieren der Strecke die vom Encoder erfasst werden muss,. bis eine Fahrtrichtung wieder
                                              // freigegeben wird.

const uint8_t Wartezeit = 50;

void ISR_EndschL();
void ISR_EndschR();

/*****Positionierparameter*****/

char Befehl;                              //Deklarieren der Variablen für die Kommunikation und Motorsteuerung
int Sollpos;
int Istpos;
char Zustand;

bool ref = true;                          //Deklarieren und Initialisieren der Variable zur Überprüfung ob bereits refeneziert wurde


/*************** Initiaisierung ***********/


void setup() {

  intitialiseBLE();

  /**** Konfiguration Encoder Pins und Interrupts *****/
  pinMode(encoder0PinA, INPUT);
  digitalWrite(encoder0PinA, HIGH);                                        // Aktivieren des pull-up Widerstandes
  pinMode(encoder0PinB, INPUT);
  digitalWrite(encoder0PinB, HIGH);                                        // Aktivieren des pull-up Widerstandes

  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoder, CHANGE); // Zuweisen des Interrupts der auf einen Pegelwechsel am EncoderPin A wartet
  
  /**** Konfiguration Motor Pins *****/
  pinMode(PinRichtung, OUTPUT);                                            // Den Richtungspin als Ausgang definieren
  digitalWrite(PinRichtung, LOW);                                          // Den Richtungspin in einen definierten Zustand versetzen
  pinMode(PinGeschwindigkeit, OUTPUT);                                     // Den Geschwindigkeitspin als Ausgang definieren
  analogWrite(PinGeschwindigkeit, 0);                                      // Das Initial-PWM-Signal auf Geschwindigkeit = "0" setzen

  /*** Konfiguration Endschalter Pins ****/
  pinMode(EndschR, INPUT);
  digitalWrite(EndschR, HIGH);                                              // Aktivieren des pull-up Widerstandes
  attachInterrupt(digitalPinToInterrupt(EndschR), ISR_EndschR, CHANGE);     // Zuweisen der ISR auf den Interrupt-Pin

  pinMode(EndschL, INPUT);
  digitalWrite(EndschL, HIGH);                                              // Aktivieren des pull-up Widerstandes
  attachInterrupt(digitalPinToInterrupt(EndschL), ISR_EndschL, CHANGE);     // Zuweisen der ISR auf den Interrupt-Pin

  /***** Initialisierung Positionierparameter *****/
  Befehl = 0;                                                               // Initialisierung der Parameter welche für die 
  Sollpos = 0;                                                              // Positionierung und Motorsteuerung notwendig sind
  Istpos = Sollpos;
  Zustand = 0;

  //Serial.begin (9600);

}


/************* Schleife ***************/
void loop() {



  communicate(Befehl, Sollpos, Istpos, Zustand);                            // Zyklisches Abrufen der via Bluetooth übermittelten Werte
  motormain(Befehl, Sollpos, Istpos, Zustand);                       // Schreiben der ermittelten Werte in die Motorsteuerung


  if ((EndlageL == false) || (EndlageR == false))                           // Überprüfen ob Endlagen Flag gesetzt wurde
  {

    if (((LastISR + Hyst) < encoder0Pos) || ((LastISR - Hyst) > encoder0Pos))   // Wenn Endlagenflag gesetzt wurde und die Ist-Position ausreichend von
                                                                            // der Endlage entfernt ist, freigeben des Positionierbetriebs in Richtung
    { EndlageL = true;                                                      // dieser Endlage
      EndlageR = true;
//      Serial.println("Reset Endlage:");
    }
  }

}

/**********Interrupt Service Routinen der Endschalter*************/

void ISR_EndschL(){                                                        // ISR des linken Endschalters           
  if (EndlageL)
  {
    EndlageL = false;                                                      // Setzten des linken Endlagen-Flags
    Befehl = 50;                                                           // Verfahrbefehl auf Stopp ändern
    LastISR = encoder0Pos;                                                 // Speichern der aktuellen Position in LastISR
//    Serial.println("Last ISR:");
//    Serial.println(LastISR, DEC);
  }

};

void ISR_EndschR(){                                                        // ISR des rechten Endschalters      
  if (EndlageR)
  {
    EndlageR = false;                                                      // Setzten des rechten Endlagen-Flags
    Befehl = 50;                                                           // Verfahrbefehl auf Stopp ändern
    LastISR = encoder0Pos;                                                 // Speichern der aktuellen Position in LastISR
//    Serial.println(LastISR, DEC);
  }
};



