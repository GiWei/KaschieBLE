/*******************************************
*** Dateiname: Motorcontrol.cpp
********************************************
***	Zweck: Einlesen, verarbeiten und
***			und Ausgeben von Steuersignalen an den Motor
***
***	Erstellt: 19.10.2016
******************************************
*** Geändert: 26.10.2016
******************************************
*** Änderung: Anpassung der command-Werte auf ASCII-Code
*****************************************/

#include "Motorcontrol.h"

int motormain(char &command, int &target, int &actual, char &state) { // Auswerten des aktuellen Befehlszustand
                                                                      // und aufrufen der entsprechenden Funktion
  switch (command)
  {
    case 50:
      stoppe();
      break;
    case 48:                               //Linkslauf 100%
      tipp(false, true);
      break;
    case 51:                              //Linkslauf 50%
      tipp(false, false);
      break;
    case 49:                               //Rechtslauf 100%
      tipp(true, true);
      break;
    case 52:                               //Rechtslauf 50%
      tipp(true, false);
      break;
    default:
      break;
  }

  return 0;

};


int stoppe()
{

  analogWrite(PinGeschwindigkeit, 0);                 //Setze PWM-Signal auf 0; -> Motor stoppt

  return 0;

};


int tipp(bool Richtung, bool GeschwTipp)
{
  if (Richtung && EndlageL)                           //Auswerten der gewünschten Verfahrrichtung und ob eine freigabe hierfür besteht
  {
    digitalWrite(PinRichtung, true);                  //"HIGH" auf den Richtungspin für Drehrichtung rechtslauf
    if (GeschwTipp) {
      analogWrite(PinGeschwindigkeit, 255);           // PWM auf Duty-Cycle 100% setzen
    }
    else {
      analogWrite(PinGeschwindigkeit, 127);           // PWM auf Duty-Cycle 50% setzen
    }
  }
  else if (EndlageR && Richtung==false) {             //Auswerten der gewünschten Verfahrrichtung und ob eine freigabe hierfür besteht
    digitalWrite(PinRichtung, LOW);                   //"LOW" auf den Richtungspin für Drehrichtung linkslauf
    if (GeschwTipp) {
      analogWrite(PinGeschwindigkeit, 255);           // PWM auf Duty-Cycle 100% setzen
    }
    else {
      analogWrite(PinGeschwindigkeit, 127);           // PWM auf Duty-Cycle 50% setzen
    }
  }
  else {
    //Verbotene Richtung
  }
  
return 0;


}


/* 
 * Die nachfolgenden Funktionen  wurden noch nicht vollständig implementiert,
 *  da die App noch nicht den vollständigen Funktionsumfang bereitstellt.
 *  Die Funktionen können als Gerüst für mögliche nachfolge Arbeiten
 *  genutzt werden.
 */

int fahr(bool Richtung, int &Geschw) {

  if (Richtung)
  {
    digitalWrite(PinRichtung, true);                   //"HIGH" auf den Richtungspin für Drehrichtung rechtslauf
    analogWrite(PinGeschwindigkeit, Geschw);           // Weise Geschwindigkeitspin, Wert von Geschwindigkeit zu
  }
  else {
    digitalWrite(PinRichtung, LOW);                     //"LOW" auf den Richtungspin für Drehrichtung linkslauf
    analogWrite(PinGeschwindigkeit, Geschw);           // Weise Geschwindigkeitspin, Wert von Geschwindigkeit zu
  }

  return 0;
};
int fahr(int &target, int &actual, int &Geschw) {

  bool atPos = false;

  while (atPos <= 0) {

    if ((target - actual) > 0)
    {
      //Bewege in die eine Richtung
    }
    else if ((target - actual) < 0)
    {
      //Bewege in die andere Richtung
    }
    else
    {
      // Ziel erreicht
      atPos = true;
    }
  }

  return 0;
};


int fahrref(char &state) {

  if (state < 1) {
    while (EndlageL) {

      digitalWrite(PinRichtung, true); //"HIGH" auf den Richtungspin für Drehrichtung vorwärts
      analogWrite(PinGeschwindigkeit, RefGeschw);

    } ;                  //digitalRead(EndschR) != false

    encoder0Pos = 0;                    // Bei erreichen des Endschalters "nullsetzen" des Encoderwerts
    do {
      digitalWrite(PinRichtung, false);
    } while (encoder0Pos >= 100);
    analogWrite(PinGeschwindigkeit, 0);
    state = 1;
    return 0;
  }
  else
  {
    return -1;
  }

};
