/*******************************************
***	Ersteller:  Philipp Gimbel
***				      Christian Weißenbacher
*******************************************
*** Dateiname: Motorcontrol.h
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

/*
  Eingehende Variablen:
  char command	= 50  // Stoppe
				        = 48	 // Fahre nach links
				        = 49	 // Fahre nach rechts
				        = 51	 // Fahre mit 50% nach links
                = 52   // Fahre mit 50% nach rechts

  target			= X // Sollposition = X

  actual			= Y // Istposition = Y

  state			= 0 // nicht referenziert
    				= 1 // Postion erreicht
*/

#pragma once

#include <Arduino.h>

/********* Deklaration der in diesem Teil des Programms verwendeten Variablen ********/

extern const uint8_t PinRichtung;
extern const uint8_t PinGeschwindigkeit;

extern bool EndlageL;
extern bool EndlageR;

extern const uint8_t RefGeschw;

extern volatile unsigned int encoder0Pos;

/******* Deklaration der Funktionen der Motorsteuerung *******/

int motormain(char &command, int &target, int &actual, char &state);

int tipp(bool Richtung, bool GeschwTipp);
int stoppe();
int fahr(bool Richtung, int &Geschw);
int fahr(int &target, int &actual, int &Geschw);
int fahrref(char &state);
