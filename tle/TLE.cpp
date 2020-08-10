#include "../reader/reader.h" // für getSubString()
#include "TLE.h"              // HEADER

extern const bool SONATE_ONLY; // Gibt an ob nur für Objekt SONATE Ausgabe vorgenommen werden soll, für regulären Betrieb nicht beachten

// Überladungen der in der Headerdatei (TLE.h) definierten Funktionen

Tle::Tle() {} // Hier fehlt eventuell noch etwas!

Tle::Tle(const std::string& line0, const std::string& line1, const std::string& line2) // Konstruktor
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

void Tle::populateTle(const std::string& line0, const std::string& line1, const std::string& line2) // wird von Konstruktor aufgerufen
{
    // Daten aus Zeilen extrahieren und in entsprechenden Variablen speichern:

    // # LINE 0
    // Satelliten-Name:
    strcpy(this->satelliteName, line0.substr(Satellite_Name_pos, Satellite_Name_length).c_str());
    // # LINE 1
    // Satelliten-Nummer:
    this->satelliteNr = getInteger(line1, Satellite_Number_pos, Satellite_Number_length);
    // International Designator: (mehrere Informationen gebündelt!)
    strcpy(this->intDesignator, line1.substr(International_Designator_Year_pos, International_Designator_Year_length + International_Designator_PieceOfLaunch_length).c_str());
    // year
    this->year = checkyear(getInteger(line1, Epoch_Year_pos, Epoch_Year_length));
    // dayFraction
    this->dayFraction = getDouble(line1, Epoch_Day_Fraction_pos, Epoch_Day_Fraction_length, false);
    // bStar
    this->bStar = getDouble(line1, BSTAR_pos, BSTAR_length, true);
    // # LINE 2
    // Inklination
    this->inclination = deg2rad(getDouble(line2, Inclination_pos, Inclination_length, false));
    // raan
    this->raan = deg2rad(getDouble(line2, Right_Ascension_Of_Ascending_Node_pos, Right_Ascension_Of_Ascending_Node_length, false));
    // eccentricity
    this->eccentricity = getDouble(line2, Eccentricity_pos, Eccentricity_length, true);
    // argumentOfPerigee
    this->argumentOfPerigee = deg2rad(getDouble(line2, Argument_Of_Perigee_pos, Argument_Of_Perigee_length, false));
    // meanAnomaly
    this->meanAnomaly = deg2rad(getDouble(line2, Mean_Anomaly_pos, Mean_Anomaly_length, false));
    // meanMotion
    this->meanMotion = revPerDay2RadPerMin(getDouble(line2, Mean_Motion_pos, Mean_Motion_length, false));

    // Prüfen ob eine Zeile ungültig ist (Aussagenlogik: !A || !B == !(A && B) )
    if (!(isTleLineValid(line1) && isTleLineValid(line2)))
    {
        this->valid = false;
    }
    else
    {
        this->valid = true;
    }
}

// GETTER
std::string Tle::getSatelliteName() { return std::string(this->satelliteName); }
int Tle::getSatelliteNr() { return this->satelliteNr; }
std::string Tle::getintDesignator() { return std::string(this->intDesignator); }
int Tle::getYear() { return this->year; }
double Tle::getDayFraction() { return this->dayFraction; }
double Tle::getBstar() { return this->bStar; }

// Rückgabe erfolgt stets in [rad] bzw. [rad/min]! (siehe 'Einlesen'!)
double Tle::getInclination() { return this->inclination; }
double Tle::getRaan() { return this->raan; }
double Tle::getEccentricity() { return this->eccentricity; }
double Tle::getArgumentOfPerigee() { return this->argumentOfPerigee; }
double Tle::getMeanAnomaly() { return this->meanAnomaly; }
double Tle::getMeanMotion() { return this->meanMotion; }

bool Tle::isTleLineValid(const std::string& line) // prüft für Zeile Gültigkeit
{
    // Prüfen ob die Zeilen länge genau 69 Zeichen lang ist:
    //if (strlen(line) != 69) return false;

    int sum = 0; // summiert alles auf (Feldwerte)

    // Fallunterscheidung: Ziffern addieren, Minuszeichen mit '1' bewerten, restliche Zeichen ignorieren
    // Ergebnis modulo 10 nehmen und mit Checksum der Zeile vergleichen (69). Bool zurückgeben.
    for (int i = 0; i < 68; i++)
    {
        char chr = line[i]; // aktuelles Zeichen

        if (chr == ' ' || chr == '+' || chr == '.' || std::isalpha(chr))
            continue; // isalpha() : bool IstBuchstabe()
        else if (chr == '-')
            sum++; // + 1
        else
        {
            int number = (int)(chr - '0'); // std::atoi() ist Schrott und hat hier Fehler verursacht! Besser so *char in int konvertieren!
            sum += number;
        }
    }

    sum %= 10; // modulo 10

    // rechnet Character auf Position 68 mit - '0' in eine Zahl um:
    return (line[68] - '0') == sum;
}

void Tle::print(void) // Ausgabe gesamtes TLE
{
    std::cout.precision(8); // Ändert die Anzahl der ausgegebenen Nachkommastellen.

    // Info: Zeilenumbruch für nächstes Element wird stets im vorherigen Schritt durchgeführt

    int counter; // Temporärer Counter für mehrere Zwischenoperationen
    // ##
    std::cout << "satelliteName\t:\t";
    counter = 25;
    while (counter--)
    {
        std::cout << this->satelliteName[24 - counter];
    }
    std::cout << '\n';
    // ##
    std::cout << "satelliteNr\t:\t" << this->satelliteNr << '\n';
    // ##
    std::cout << "intDesignator\t:\t";
    counter = 9;
    while (counter--)
    {
        std::cout << this->intDesignator[8 - counter];
    }
    std::cout << '\n';
    // ##
    std::cout << "year\t\t:\t" << this->year << '\n';
    // ##
    std::cout << "dayFraction\t:\t" << this->dayFraction << '\n';
    // ##
    std::cout << "Bstar\t\t:\t" << this->bStar << '\n';
    // ##
    std::cout << '\n';
    std::cout << std::fixed; // ab hier werden Zahlen bündig dargestellt
    // ##
    std::cout << "inclination\t:\t" << this->inclination << " [rad]" << '\t' << rad2deg(this->inclination) << " [deg]" << '\n';
    // ##
    std::cout << "raan\t\t:\t" << this->raan << " [rad]" << '\t' << rad2deg(this->raan) << " [deg]" << '\n';
    // ##
    std::cout << "eccentricity\t:\t" << this->eccentricity << '\n';
    // ##
    std::cout << "argOfPerigee\t:\t" << this->argumentOfPerigee << " [rad]" << '\t' << rad2deg(this->argumentOfPerigee) << " [deg]" << '\n';
    // ##
    std::cout << "meanAnomaly\t:\t" << this->meanAnomaly << " [rad]" << '\t' << rad2deg(this->meanAnomaly) << " [deg]" << '\n';
    // ##
    std::cout << "meanMotion\t:\t" << this->meanMotion << " [rad/min]" << '\t' << rad2deg(this->meanMotion) << " [deg/min]" << '\n';
    // ##
    std::cout << '\n';
    // ##
    std::cout << "valid\t\t:\t" << ((this->valid == true) ? "true" : "false") << '\n';
    // ## ENDE ###
    std::cout << '\n';

    if (SONATE_ONLY)
    {
        // weitere Ausgabe: (siehe Aufgabe Übungsblatt)
        // - Große Halbachse a
        // - Wahre Anomalie

        double _a = get_a(this->getMeanMotion()); // Einheit: [km]
        double _ny = getTrueAnomaly(this->getEccentricity(), this->getMeanAnomaly());

        std::cout << "Zusätzliche Bahnelemente:"
                  << "\n\n";
        std::cout << "Große Halbachse\t:\t" << _a << " [km]" << '\n';
        std::cout << "Wahre Anomalie\t:\t" << _ny << " [rad]" << '\n';
    }

    // Schönheitsabstand zu nächstem Datensatz:
    std::cout << "\n\n"
              << std::flush; // erst zum Ende flushen, ist sonst zu teuer
}