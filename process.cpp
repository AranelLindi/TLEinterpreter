// eigener Code
#include "process.h" // HEADER

// GLOBALE VARIABLEN
// Ist absichltich hier deklariert und im Header mit extern definiert! (Standartvorgehensweise)
bool SONATE_only = false; // Gibt an, ob nur für Objekt SONATE Ausgabe vorgenommen werden soll. Für regulären Betrieb nicht beachten.

std::map<int, Tle> readTlesFromFile(const char *fileName)
{
    std::ifstream file; // Datei mit TLEs

    std::map<int, Tle> TLEs; // Map anlegen

    bool lastTle = false; // für Abbruchbedingung

    try // wg. Dateioperationen
    {
        file.open(fileName);

        while (1) // Schleife läuft so lange, bis Abbruchbedingung (siehe unten) erreicht ist
        {
            char *lineptr[3] = {nullptr}; // Hilfszeiger (zeigen je auf eine der drei Zeilen eines einzelnen TLEs)

            std::string line_str; // temporärer Zwischenspeicher für immer jeweils eine Zeile (Strings sind veränderbar)

            int key = 0; // enthält später die Satelliten-Nr

            for (int i = 0; i < 3; i++) // gleiche Prodzedur für jede der drei Zeilen des TLE
            {
                std::getline(file, line_str); // schreibt Zeile in string Variable

                // Abbruchbedingung der Iteration!
                if (line_str == "")
                    lastTle = true;
                // Ist eine der drei Zeilen eines TLEs leer, kann davon ausgegangen werden, dass das
                // Dateiende erreicht ist. (Zur Info: Hier hat (! (file.eof()) ) nicht funktioniert!)

                // Wichtig: string.c_str() liefert einen (const char*), benötigt wird aber ein (char*), daher umwandeln:
                lineptr[i] = convertConstCharPtrToCharPtr(line_str);
            }

            if (lastTle == true)
                break; // Gehört zur Abbruchbedingung: while-Schleife beenden

            key = getInteger(lineptr[1], Satellite_Number, Satellite_Number_End); // Satelliten-Nr. aus zweiten Zeile extrahieren

            Tle tle = Tle(lineptr[0], lineptr[1], lineptr[2]); // neues TLE Objekt anlegen & Zeilen an Konstruktor übergeben...

            // prüfen ob Zeile 2 und 3 gültig sind, falls nicht, nicht parsen und TLE nicht aufnehmen:
            if (tle.isTleLineValid(lineptr[1]) && tle.isTleLineValid(lineptr[2]))
            {

                std::pair<int, Tle> _pair(key, tle); // ... neues pair Objekt anlegen mit TLE Objekt füllen, Zähler inkremntieren...

                TLEs.insert(_pair); // ... pair Objekt in map einfügen
            }

            //break; // zu Testzwecken entkommentieren, dann wird nur ein (gültiges) TLE gelesen
        };
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    // Auch nach Exception wird dieser Bereich hier noch ausgeführt. Bereinigungen vornehmen und Rückgabe der TLEs:
    file.close();

    return TLEs; // fertige map zurückgeben
}

char *getSubString(const char *source, int start, int end) // Gibt einen Zeiger auf einen Teilstring einer Zeile zurück.
{
    int delta = end - start + 1; // Länge des zurückgegebenen Strings. +1 folgt, da statt [start, end) dann [start, end] zurückgegeben wird

    //if ( delta < 1 ) return nullptr; // Es muss mindestens ein Zeichen gelesen werden

    char *subString = new char[(delta + 1)]; // Feldlänge + 1 für Stringabschlusszeichen

    std::strncpy(subString, source + (start - 1), delta); // kopiert Teilstring aus Zeile in subString (Nichts mehr an Konstanten (Additionen/Subtraktionen) verändern!!)

    subString[delta] = '\0'; // Stringabschlusszeichen hinzufügen

    return subString;
}

int getInteger(const char *source, int start, int end) // Gibt Zahl aus TLE als Integer zurück
{
    char *str_number = getSubString(source, start, end); // Zeiger auf Teilstring

    // ******************************** WICHTIG ! *******************************
    // Prüfen ob Zeichen im String stehen, die bei einer Konvertierung Fehler verursachen würden:
    int counter = 0; // Zählt Position mit
    while (!isdigit(str_number[counter]) | (str_number[counter] == '-'))
        counter++; // Aussage ( !A | B ) validiert!

    // wurde in obiger Schleife unzulässige Zeichen gefunden, neuen Index Teilstring bekommen, dazu counter-Felder vorrücken:
    if (counter > 0)
        str_number = getSubString(source, start + counter, end);
    // **************************************************************************

    int number = std::stoi(str_number); // string in int konvertieren

    return number;
}

double getDouble(const char *source, int start, int end, bool leadingdecimalpointassumed) // Gibt Nummer aus TLE als Double zurück (mit Column-Daten aus Definition arbeiten!)
{
    char *str_number = getSubString(source, start, end); // Zeiger auf Teilstring

    // ******************************** WICHTIG ! *******************************
    // Prüfen ob Zeichen im String stehen, die bei einer Konvertierung Fehler verursachen würden
    int counter = 0;
    while (!isdigit(str_number[counter]) | (str_number[counter] == '-'))
        counter++; // Aussage ( !A | B ) validiert!

    // wurde in obiger Schleife unzulässige Zeichen gefunden, neuen Index Teilstring bekommen, dazu counter-Felder vorrücken:
    if (counter > 0)
        str_number = getSubString(source, start + counter, end);
    // **************************************************************************

    // ************************ Führender Dezimalpunkt **************************
    char *str_number_helper = str_number; // Hilfszeiger

    if (leadingdecimalpointassumed) // Soll an erster Stelle noch ein Komma eingefügt werden:
    {
        str_number_helper = (char *)malloc((strlen(str_number) + 1) * sizeof(char)); // Ursprüngliche Feldlänge + 1 anlegen um in erstes Feld...

        str_number_helper[0] = '.'; // ... Dezimalpunkt zu schreiben

        stringcopy(str_number, &str_number_helper[1]); // String kopieren, aber im Ziel erst ab zweitem Feld einfügen

        str_number = str_number_helper; // Rückzuweisung um auf gleicher Gleis zu sein
    }
    // ***************************************************************************

    double number = 0; // enthält später Gleitkommazahl

    // ############################################################################
    // Hier darauf achten, dass Gleitkommazahlen ihren Exponenten bekommen!
    // Diesen hier rausfiltern:

    // Alle Zeichen einzeln scannen und nach Exp.-Anteil suchen: (Format: -NNNNNN-N)
    int pos = 0;
    // von hinten nach vorne nach einem Minus suchen (das ist dann der Exponent). Erste Stelle auslassen, könnte ein normales Vorzeichen sein!
    // Letzte Stelle ebenfalls auslassen, ein Minus dort ergäbe keinen Sinn. (NNNN-) (Deswegen i=...-1 !)
    for (int i = (std::strlen(str_number) - 1); i > 1; i--)
    {
        if (str_number[i] == '-' || str_number[i] == '+')
        {
            pos = i; // Position des Minus merken

            break;
        }
    }

    if (!(pos == 0)) // wird nur ausgeführt, wenn ein Exponent erkannt wurde (Minus inmitten der Zahl)
    {
        // Zahl hinter Minus extrahieren: (das ist der Exponent)
        int exponent_length = (end - start + 1) - pos; // (end - start + 1) entspricht der Länge der Information (delta), "- pos" ergibt die Anzahl Stellen des Exponenten
        char exponent[exponent_length];                // Array mit Größe der Länge des Exponenten anlegen

        // Ziffern des Exponenten in Array verfrachten:
        for (int i = 0; i < exponent_length; i++)
        {
            exponent[i] = str_number[pos + i + 1]; // pos + 1 ist erstes Zeichen nach Vorzeichen
        }

        // Exponenten als Integer darstellen:
        int exp = std::stoi(exponent);
        if (str_number[pos] == '-')
            exp = -exp; // ggf. Vorzeichenwechsel

        // Mantisse in double konvertieren
        char *mantisseptr = getSubString(str_number, 1, pos); // +1 da in SubString wieder abgezogen!

        // Mantisse zusammensetzen: (ruft Lambda auf!)
        double mantisse = std::stod(mantisseptr);

        // Komplette Zahl konstruieren: (Mantisse, Exponenten)
        number = mantisse * std::pow(10, exp);
    }
    else
    {
        // Falls kein Exponent gefunden wurde, kann die Zahl ganz normal behandelt werden.

        // Zahl zusammensetzen und in Double konvertieren:
        number = std::stod(str_number);
    }
    // ############################################################################

    return number;
}

char *convertConstCharPtrToCharPtr(std::string sstring) // string.c_str() liefert einen (const char*), benötigt wird aber (char*), daher umwandeln nötig
{
    char *writeable = new char[sstring.size() + 1]; // erstellt char-Zeiger (inkl. allokiertem Speicher) mit Länge von line + 1 (für Stringabschlusszeichen)

    std::copy(sstring.begin(), sstring.end(), writeable); // kopiert line auf neu allokierten Speicher auf den writeable zeigt
    writeable[sstring.size()] = '\0';                     // fügt C Stringabschlusszeichen hinten an
    // writeable *NICHT* freigeben, da sonst gerade kopierter String ebenfalls freigegeben wird!
    return writeable;
}

void stringcopy(char *source, char *dest) // einfache Funktion um char-Array zu kopieren
{
    // Wichtig! source muss größer gleich dest sein (gemeint ist Feldlänge auf Stack) und letztes Zeichen von dest muss '\0' sein! (Abbruchbedingung)
    while ((*dest++ = *source++))
        ;
}

double a(double f) // Berechnet über die mittlere Bewegungszeit und der Konstanten GM die große Halbachse a in Metern
{
    if (f == 0)
        return 0; // Fall T = 0 abdecken (durch Null teilen!)

    double T_in_s = 2 * M_PI * 60 * (1 / f); // Enthält Anzahl Sekunden pro Umlauf (T)

    return ( cbrt( 0.25 * GM * ( powf( T_in_s / M_PI, 2 ) ) ) );
    //return cbrt(GM * pow(T_in_s, 2) * 1 / (4 * pow(M_PI, 2))); // (cbrt ist die kubische Wurzel)
}

double getTrueAnomaly(double e, double M) // Berechnet mittels Newton-Verfahren und numerische Exzentrizität sowie mittlerer Anomalie die wahre Anomalie (Newton-Verfahren)
{
    // Kepler-Gleichung: M = E - e * sin( E )
    // -> wird umgestellt zu: 0 = E - e * sin( E ) - M

    double x0 = M; // Startwert

    // Lambda-Funktionen: Stellen die Keplergleichung sowie ihre Ableitung dar
    const auto F = [e, M](double x) -> double { return (x - e * sin(x) - M); };
    const auto f = [e](double x) -> double { return (1 - e * cos(x)); };

    // Newton Verfahren
    for(uint8_t i = 0; i < 10; i++) {
        if (f(x0) == 0) {
            break; // Division durch Null abfangen
        }

        x0 = x0 - ( F(x0) / f(x0) ); // Newton-Verfahren
    }// while (counter < 5);//std::fabs(F(x0)) > 1e-10);

    // exzentrische Anomalie in wahre Anomalie umrechnen:
    double trueAnomaly = 2 * atan2( ( tan(x0 / 2) * sqrt((1 + e) / (1 - e)) ), 1 );

    // True Anomaly

    return (trueAnomaly < 0 ? trueAnomaly + (2 * M_PI) : trueAnomaly);
}

double convertRevPerDayInRadPerMin(double value) // Wandelt Zahl von [rev/day] in [rad/min] um
{
    return (value * (2 * M_PI) / (60 * 24));
}

double convertRadPerMinInRevPerDay(double value) // Wandelt Zahl von [rad/min] in [rev/day] um
{
    return (value * (60 * 24) / 2 * M_PI);
}

double convertDegreeInRadian(double angle) // Wandelt [degree] in [rad] um
{
    return (angle * M_PI / 180.0);
}

double convertInDegree(double rad) // Wandelt [rad] in [degree] um
{
    return (rad * 180.0 / M_PI);
}

int checkyear(unsigned int value)
{
    // NORAD definiert Epochen wie folgt:
    // 57 - 99 entspricht 1957 - 1999
    // 00 - 56 entspricht 2000 - 2056
    if (value < 57)
    {
        return 2000 + value;
    }
    else if (value > 56)
    {
        return 1900 + value;
    }
    else
    {
        return value;
    }
}