/*******************************************
*** Dateiname: Motorcontrol.h
********************************************
***	Zweck: Einlesen, verarbeiten und
***			und Ausgeben von Steuersignalen an den Motor
***
***	Erstellt: 19.10.2016
******************************************
*** Geändert: XX.XX.XXXX
******************************************
*** Änderung:
*****************************************/
/*
 * In diesem Software-Modul, werden die Variablen
 * für die Kommunikation über Bluetooth erzeugt
 * sowie initialisiert.
 * Geplant ist die Kommunikation über 4
 * Charakteristika.
 */

#pragma once

#include <CurieBLE.h>                       // Einbinden der für den Genuino101 notwendigen BLE-Bibliothek

/********* Deklaration der in diesem Teil des Programms verwendeten Variablen ********/

extern BLEPeripheral Genuino101;
extern BLEService Positioniersystem; // BLE Posistioniersystem Service
extern BLEUnsignedCharCharacteristic	Positionierbefehl;
extern BLEUnsignedIntCharacteristic		Sollposition;
extern BLEUnsignedIntCharacteristic		Istposition;
extern BLEUnsignedCharCharacteristic	Zustand_BLE;

/******* Deklaration der Funktionen der Bluetooth-Verbindung *******/

int intitialiseBLE();

int communicate(char &command,int &target,int &actual,char &state );
