# TLEinterpreter
*Interpreter für TLE (Two Line Element), die verschiedene Informationen eines Objektes im Low Earth Orbit enthalten.*

## tleinterpreter.h ##
Muss bei Projekten includet werden, um alle HEADER des Interpreters einzubinden.

## reader/reader.h & reader.cpp ##
Interne Funktionen für den TLEInterpreter um Informationen aus TLE korrekt zu extrahieren und ggf. in andere Einheit umzurechnen.

## tle/TLE.h & TLE.cpp ##
Stellt den Container eines TLE dar und enthält Funktionen für eine Ausgabe auf der Konsole

## main.cpp ##
Muss bei neuen Projekten nicht kompiliert werden, enthält aber einen beispielhaften Aufruf des Interpreters.


-----------------
Stefan Lindörfer
 02. Juni 2020