/*******************************************
*** Dateiname: Encoder.h
********************************************
***	Zweck: Einlesen, verarbeiten und
***			und Ausgeben von Steuersignalen an den Motor
***
***	Erstellt: 20.10.2016
******************************************
*** Geändert: 27.10.2016
******************************************
*** Änderung: Variablen als Extern deklartiert
*****************************************/
/*
 * Header-Datei des Encoder-Software-Moduls:
 * In diesem Teil wird der aktuelle Wert des Encoders
 * ermittelt bzw. hoch oder runter gezählt.
 */

#pragma once

#include <Arduino.h>

/********* Deklaration der in diesem Teil des Programms verwendeten Variablen ********/
extern const uint8_t encoder0PinA;
extern const uint8_t encoder0PinB;

extern volatile unsigned int encoder0Pos;

/******* Deklaration der Funktionen des Encoders *******/

void doEncoder();
