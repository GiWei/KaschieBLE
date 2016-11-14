/*******************************************
*** Dateiname: Encoder.cpp
********************************************
***	Zweck: Einlesen, verarbeiten und
***			und Ausgeben von Steuersignalen an den Motor
***
***	Erstellt: 20.10.2016
******************************************
*** Ge�ndert: XX.XX.XXXX
******************************************
*** �nderung:
*****************************************/


/* Auswerten eines Drehencoders mit Interrupts
 *  wenn beide Signale zur gleichen Zeit "TURE" sind,
 *  dreht sich der Motor "vorwärts". Falls nicht, dreht
 *  er in die entgegengesetzte Richtung.
*/


#include <Arduino.h>
#include "Encoder.h"


void doEncoder() {

	if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
		encoder0Pos++;
	}
	else {
		encoder0Pos--;
	}

	Serial.println(encoder0Pos, DEC);               //Serielle Ausgabe des gelesenen Encoderwerts, wird im fertigen Programm via Bluetooth verarbeitet
}

