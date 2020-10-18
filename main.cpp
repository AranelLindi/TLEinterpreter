// Standartbibliothek (können in der .cpp Datei bleiben, da sie nicht für den generellen
// Betrieb des TLEInterpreter benötigt werden, sondern nur für die Ausgabe. Daher von üblicher
// Praxis absehen)
#include <iostream>   // für Konsolen Ein- & Ausgabe
#include <filesystem> // c++17 expansion
#include <string>     // std::strings

#include "tleinterpreter.h" // HEADER

extern const bool SONATE_ONLY; //

int main(void)
{
    // Standartausgabe & Einlesen des Dateipfades inkl. -name
    std::cout << "\n R A U M F L U G B E T R I E B\n\t************************\n\t\t    ************\n\t\t\t  ******\n Dateipfad TLEs: ";
    std::string filename;

    // Pfad einlesen und ...
    /*filename = "tle.txt";*/ std::cin >> filename; // Zu Debugging-Zwecken
    std::cout << '\n'
              << std::endl;

    // ... pro forma auf zero-length string prüfen
    if (filename.length() == 0 || !(std::filesystem::exists(filename)))
        return 1; // EXIT_FAILURE

    // Daten aus Datei einlesen und map erzeugen:
    const std::map<int, Tle> TLEs{readTlesFromFile(filename.c_str())};

    // Iteriert durch map und löst für jedes Element Funktion print() aus. (Lambda-Ausdruck! Erfordert C++11!)
    std::for_each(TLEs.begin(), TLEs.end(), [](std::pair<int, Tle> element) {
        Tle currentTLE = element.second; // gibt zweiten Typ der map zurück (TLE)
        if (SONATE_ONLY && (currentTLE.getSatelliteNo() != SONATE_NUMBER))
            goto next;
        currentTLE.print(); // ruft print()-Funktion in struct auf
    next:;
    });

    return 0;
}

// to build with: $ g++ -o [exec.-Name] main.cpp TLE.cpp process.cpp -std=c++2a