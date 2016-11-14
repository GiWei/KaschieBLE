/*******************************************
*** Dateiname: Motorcontrol.h
********************************************
***	Zweck: Einlesen, verarbeiten und
***			und Ausgeben von Steuersignalen an den Motor
***
***	Erstellt: 23.10.2016
******************************************
*** Ge�ndert: XX.XX.XXXX
******************************************
*** �nderung:
*****************************************/


#include "Bluetooth.h"

int intitialiseBLE(){                                             // Initalisierung der Bluetooth-Parameter

	// set advertised local name and service UUID:
	Genuino101.setLocalName("Encoder-Maschine");                     // Zuweisen eines Gerätenamens
	Genuino101.setAdvertisedServiceUuid(Positioniersystem.uuid());  //  Zuweisen der UUID des Services überwelchen Kommuniziert werden sollen

	// add service and characteristic:
	Genuino101.addAttribute(Positioniersystem);                      // Zuordenen des Services und der Charakteristika zum Gerät.
	Genuino101.addAttribute(Positionierbefehl);                      // Somit lassen sich diese via Bluetooth finden
	Genuino101.addAttribute(Sollposition);
	Genuino101.addAttribute(Istposition);
	Genuino101.addAttribute(Zustand_BLE);


	// set the initial value of each characeristic
	Positionierbefehl.setValue(0);                                   // Initialisieren aller Charakteristika mit "0", somit
	Sollposition.setValue(0);                                        // werden nach der Initialisierung kein Befehle ausgelöst.
	Istposition.setValue(0);
	Zustand_BLE.setValue(0);

	// begin advertising BLE service:
	Genuino101.begin();                                             // Beginne mit dem "Advertising"
                                                                  // --> Aktivieren des Bluetooths, erreichbarmachen für andere Geräte
	return 0;
};

int communicate(char &command, int &target, int &actual, char &state)
{

BLECentral central = Genuino101.central();                        // Warte auf verbindung mit Geräten

	if (central)                                                    // Wenn Verbindung hergestellt ist....
	{
		if (Positionierbefehl.written())                              // wenn etwas auf die UUID von Positionierbefehl geschrieben wurde..
		{
			command = Positionierbefehl.value();                        // weise "command" den geschriebenen Wert zu.
//      Serial.println("Geschrieben:");                           // Schreibe "Geschrieben: " + den neuen Wert von Command auf die serielle Schnittstelle
//      Serial.println(command, DEC);
		}
		if (Sollposition.written())
		{
			target = Sollposition.value();
//      Serial.println("Geschrieben:");                           // Analog zu den Kommentaren bei Positionierbefehl
//      Serial.println(target, DEC);
		}
		if (Istposition.written())
		{
			actual = Istposition.value();
//      Serial.println("Geschrieben:");                           // Analog zu den Kommentaren bei Positionierbefehl
//      Serial.println(actual, DEC);
		}
		if (Zustand_BLE.written())
		{
			state = Zustand_BLE.value();
//      Serial.println("Geschrieben:");                           // Analog zu den Kommentaren bei Positionierbefehl
//      Serial.println(state, DEC);
		}
		return 0;
	}
	else
	{
		//do something
		return -1;
	}

};
