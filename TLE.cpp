#include "process.h" // für getSubString()
#include "TLE.h" // HEADER

extern bool SONATE_only; // Gibt an ob nur für Objekt SONATE Ausgabe vorgenommen werden soll, für regulären Betrieb nicht beachten

// Überladungen der in der Headerdatei (TLE.h) definierten Funktionen

Tle::Tle(char* line0, char* line1, char* line2) // Konstruktor
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

void Tle::populateTle(char* line0, char* line1, char* line2) // wird von Konstruktor aufgerufen
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
    this->year = checkyear(getInteger(line1, Epoch_Year, Epoch_Year_End));
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
    this->meanMotion = getconversion(getDouble(line2, Mean_Motion, Mean_Motion_End, false));

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

// Getter
std::string Tle::getSatelliteName() { return std::string(this->satelliteName); }
int Tle::getSatelliteNr() { return this->satelliteNr; }
std::string Tle::getintDesignator() { return std::string(this->intDesignator); }
int Tle::getYear() { return this->year; }
double Tle::getDayFraction() { return this->dayFraction; }
double Tle::getBstar() { return this->bStar; }

double Tle::getInclination() { return this->inclination; }
double Tle::getRaan() { return this->raan; }
double Tle::getEccentricity() { return this->eccentricity; }
double Tle::getArgumentOfPerigee() { return this->argumentOfPerigee; }
double Tle::getMeanAnomaly() { return this->meanAnomaly; }
double Tle::getMeanMotion() { return this->meanMotion; }

bool Tle::isTleLineValid(const char* line) // prüft für Zeile Gültigkeit
{
    // Prüfen ob die Zeilen länge genau 69 Zeichen lang ist:
    //if (strlen(line) != 69) return false;


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
    
    // rechnet Character auf Position 68 mit - '0' in eine Zahl um:
    return (line[68] - '0') == sum;
}

void Tle::print(void) // Ausgabe gesamtes TLE
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
    std::cout << "satelliteNr\t:\t" << this->satelliteNr << '\n';
    // ##
    std::cout << "intDesignator\t:\t";
    counter = 9;
    while (counter--)
    {
        std::cout << this->intDesignator[8 - counter];
    }
    std::cout << std::endl;
    // ##
    std::cout << "year\t\t:\t" << this->year << '\n';
    // ##
    std::cout << "dayFraction\t:\t" << this->dayFraction << '\n';
    // ##
    std::cout << "bStar\t\t:\t" << this->bStar << '\n';
    // ##
    std::cout << std::endl;
    // ##
    std::cout << "inclination\t:\t" << this->inclination << " [rad]" << '\n';
    // ##
    std::cout << "raan\t\t:\t" << this->raan << " [rad]" << '\n';
    // ##
    std::cout << "eccentricity\t:\t" << this->eccentricity << '\n';
    // ##
    std::cout << "meanAnomaly\t:\t" << this->meanAnomaly << " [rad]" << '\n';
    // ##
    std::cout << "meanMotion\t:\t" << this->meanMotion << " [rad/min]" << '\n';
    // ##
    std::cout << std::endl;
    // ##
    std::cout << "valid\t\t:\t" << ((this->valid == true) ? "true" : "false") << '\n';
    // ## ENDE ###
    std::cout << '\n';

    if(SONATE_only)
    {
        // weitere Ausgabe: (siehe Aufgabe Übungsblatt)
        // - Große Halbachse a
        // - Wahre Anomalie

        double _a = a( this->meanMotion );
        double _ny = ny( this->eccentricity, this->meanAnomaly );

        std::cout << "Zusätzliche Bahnelemente:" << "\n\n";
        std::cout << "Große Halbachse\t:\t" << (_a / 1000) << " [km]" << '\n'; // Division durch 1000 erzeugt Wert in km
        std::cout << "Wahre Anomalie\t:\t" << _ny << " [rad]" << '\n';
    }

    // Schönheitsabstand zu nächstem Datensatz:
    std::cout << "\n\n" << std::flush; // erst zum Ende flushen, kostet sonst viel Leistung
}
