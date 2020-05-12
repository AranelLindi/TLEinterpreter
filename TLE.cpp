// Standartbibliotheken
#include <iostream> // Konsolenausgabe ( hier mit printf() )
#include <ctype.h> // Prüfungen von Charactern
#include <cstring> // Länge von char-Arrays

// eigener Code
#include "TLE.h"
#include "cdefines.h"
#include "main.h"

extern bool SONATE_only; // Gibt an ob nur für Objekt SONATE Ausgabe vorgenommen werden soll

// Überladungen der in der Headerdatei (TLE.h) definierten Funktionen

TLE::TLE(char* line0, char* line1, char* line2) // Konstruktor
{
    // Variablen initialisieren (Arrays ausgenommen, (wurden schon))
    satelliteNr = 0;
    year = 0;
    dayFraction = 0;
    bStar = 0;
    inclination = 0;
    raan = 0;
    eccentricity = 0;
    argumentOfPerigee = 0;
    meanAnomaly = 0;
    meanMotion = 0;

    // weiterführende Funktion aufrufen
    this->populateTle(line0, line1, line2);
}

void TLE::populateTle(char* line0, char* line1, char* line2) // wird von Konstruktor aufgerufen
{
    // Daten aus Zeilen extrahieren und in entsprechenden Variablen speichern:

    // # LINE 0
    // Satelliten-Name:
    stringcopy(line0, this->satelliteName);
    // # LINE 1
    // Satelliten-Nummer:
    this->satelliteNr = getInteger(line1, Satellite_Number, Satellite_Number_End);
    // International Designator: (mehrere Informationen gebündelt!)
    char* intDesignator_ptr = getSubString(line1, International_Designator_Year, International_Designator_PieceOfLaunch_End);
    stringcopy(intDesignator_ptr, this->intDesignator);
    // year
    this->year = getInteger(line1, Epoch_Year, Epoch_Year_End) + 2000; // + 2000 für schönere Jahres-Darstellung
    // dayFraction
    this->dayFraction = getDouble(line1, Epoch_Day_Fraction, Epoch_Day_Fraction_End, false);
    // bStar
    this->bStar = getDouble(line1, BSTAR, BSTAR_End, true);
    // # LINE 2
    // Inklination
    this->inclination = getDouble(line2, Inclination, Inclination_End, false);
    // raan
    this->raan = getDouble(line2, Right_Ascension_Of_Ascending_Node, Right_Ascension_Of_Ascending_Node_End, false);
    // eccentricity
    this->eccentricity = getDouble(line2, Eccentricity, Eccentricity_End, true);
    // argumentOfPerigee
    this->argumentOfPerigee = getDouble(line2, Argument_Of_Perigee, Argument_Of_Perigee_End, false);
    // meanAnomaly
    this->meanAnomaly = getDouble(line2, Mean_Anomaly, Mean_Anomaly_End, false);
    // meanMotion
    this->meanMotion = getDouble(line2, Mean_Motion, Mean_Motion_End, false);

    // Prüfen ob eine Zeile ungültig ist (Aussagenlogik: !A || !B == !(A && B) )
    if ( ! (isTleLineValid(line1) && isTleLineValid(line2)))
    {
        this->valid = false;
    }
    else
    {
        this->valid = true;
    }
}

bool TLE::isTleLineValid(const char* line) // prüft für Zeile Gültigkeit
{
    int sum = 0; // summiert alles auf (Feldwerte)

    // Fallunterscheidung: Ziffern addieren, Minuszeichen mit '1' bewerten, restliche Zeichen ignorieren
    // Ergebnis modulo 10 nehmen und mit Checksum der Zeile vergleichen (69). Bool zurückgeben.
    for(int i = 0; i < 68; i++)
    {
        char chr = line[i]; // aktuelles Zeichen

        if (chr == ' ' || chr == '+' || chr == '.' || std::isalpha(chr)) continue; // isalpha() : bool IstBuchstabe()
        else if (chr == '-') sum++; // + 1
        else
        {
            int number = (int)(chr - '0'); // std::atoi() ist Schrott und hat hier Fehler verursacht! Besser so *char in int konvertieren!
            sum += number;
        }
    }

    sum %= 10; // modulo 10

    // Zwei Bedingungen prüfen: - Ist Zeilenlänge gleich mit 70 (insg. Länge einer Zeile)
    //                          - Ist Checksum Vergleich korrekt
    if ( (std::strlen(line) == 70) && (std::atoi(&line[68]) == sum) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void TLE::print(void) // Ausgabe gesamtes TLE
{
    std::cout.precision(15); // Ändert die Anzahl der ausgegebenen Nachkommastellen.

    // Info: Zeilenumbruch für nächstes Element wird stets im vorherigen Schritt durchgeführt
    int counter;
    std::cout << "satelliteName\t:\t";
    counter = 25;
    while (counter--)
    {
        std::cout << this->satelliteName[24 - counter];
    }
    std::cout << std::endl;
    // ##
    std::cout << "satelliteNr\t:\t" << this->satelliteNr << std::endl;
    // ##
    std::cout << "intDesignator\t:\t";
    counter = 9;
    while (counter--)
    {
        std::cout << this->intDesignator[8 - counter];
    }
    std::cout << std::endl;
    // ##
    std::cout << "year\t\t:\t" << this->year << std::endl;
    // ##
    std::cout << "dayFraction\t:\t" << this->dayFraction << std::endl;
    // ##
    std::cout << "bStar\t\t:\t" << this->bStar << std::endl;
    // ##
    std::cout << std::endl;
    // ##
    std::cout << "inclination\t:\t" << this->inclination << " [rad]" << std::endl;
    // ##
    std::cout << "raan\t\t:\t" << this->raan << " [rad]" << std::endl;
    // ##
    std::cout << "eccentricity\t:\t" << this->eccentricity << std::endl;
    // ##
    std::cout << "meanAnomaly\t:\t" << this->meanAnomaly << " [rad]" << std::endl;
    // ##
    std::cout << "meanMotion\t:\t" << this->meanMotion << " [rad/min]" << std::endl;
    // ##
    std::cout << std::endl;
    // ##
    std::cout << "valid\t\t:\t" << ((this->valid == true) ? "true" : "false") << std::endl;
    // ## ENDE ###
    std::cout << std::endl;

    if(SONATE_only)
    {
        // weitere Ausgabe: (siehe Aufgabe Übungsblatt)
        // - Große Halbachse a
        // - Wahre Anomalie

        double _a = a( this->meanMotion );
        double _ny = ny( this->eccentricity, this->meanAnomaly );

        std::cout << "Zusätzliche Bahnelemente:" << std::endl << std::endl;
        std::cout << "Große Halbachse\t:\t" << (_a / 1000) << " [km]" << std::endl; // Division durch 1000 erzeugt Wert in km
        std::cout << "Wahre Anomalie\t:\t" << _ny << " [rad]" << std::endl;
    }

    // Schönheitsabstand zu nächstem Datensatz:
    std::cout << std::endl << std::endl;
}
