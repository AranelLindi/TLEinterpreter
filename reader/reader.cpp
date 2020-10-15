// eigener Code
#include "reader.h" // HEADER

// GLOBALE VARIABLEN
// Ist absichltich hier deklariert und im Header mit extern definiert! (Standartvorgehensweise)
const constexpr bool SONATE_ONLY{false}; // Gibt an, ob nur für Objekt SONATE Ausgabe vorgenommen werden soll. Für regulären Betrieb nicht beachten.

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
            //char *lineptr[3] = {nullptr}; // Hilfszeiger (zeigen je auf eine der drei Zeilen eines einzelnen TLEs)
            std::array<std::unique_ptr<std::string>, 3> lineptr;

            std::string line_str; // temporärer Zwischenspeicher für immer jeweils eine Zeile (Strings sind veränderbar)

            int32_t key{0}; // enthält später die Satelliten-Nr

            for (int i = 0; i < 3; i++) // gleiche Prodzedur für jede der drei Zeilen des TLE
            {
                std::getline(file, line_str); // schreibt Zeile in string Variable

                // Abbruchbedingung der Iteration!
                if (line_str == "")
                    lastTle = true;
                // Ist eine der drei Zeilen eines TLEs leer, kann davon ausgegangen werden, dass das
                // Dateiende erreicht ist. (Zur Info: Hier hat (! (file.eof()) ) nicht funktioniert!)

                // Wichtig: string.c_str() liefert einen (const char*), benötigt wird aber ein (char*), daher umwandeln:
                //lineptr[i] = convertConstCharPtrToCharPtr(line_str);
                lineptr[i] = std::make_unique<std::string>(line_str);
            }

            if (lastTle == true)
                break; // Gehört zur Abbruchbedingung: while-Schleife beenden

            key = getInteger(*lineptr[1].get(), Satellite_Number_pos, Satellite_Number_length); // Satelliten-Nr. aus zweiten Zeile extrahieren

            const Tle tle {Tle(*lineptr[0].get(), *lineptr[1].get(), *lineptr[2].get())}; // neues TLE Objekt anlegen & Zeilen an Konstruktor übergeben...

            // prüfen ob Zeile 2 und 3 gültig sind, falls nicht, nicht parsen und TLE nicht aufnehmen:
            if (tle.isTleLineValid(*lineptr[1].get()) && tle.isTleLineValid(*lineptr[2].get()))
            {
                std::pair<int, Tle> _pair(key, tle); // ... neues pair Objekt anlegen mit TLE Objekt füllen, Zähler inkremntieren...

                TLEs.insert(_pair); // ... pair Objekt in map einfügen
            }

            //break; // zu Testzwecken entkommentieren, dann wird nur ein (gültiges) TLE gelesen
        }; // smart pointer objects are destroyed here... in next iteration, new unique_ptrs will be created... => no memory leaks
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    // Auch nach Exception wird dieser Bereich hier noch ausgeführt. Bereinigungen vornehmen und Rückgabe der TLEs:
    file.close();

    return TLEs; // fertige map zurückgeben
}

int32_t getInteger(const std::string &source, int32_t start, int32_t len) // Gibt Zahl aus TLE als Integer zurück
{
    //char *str_number = getSubString(source, start, end); // Zeiger auf Teilstring
    std::string str_number{source.substr(start, len)};

    // ******************************** WICHTIG ! *******************************
    // Prüfen ob Zeichen im String stehen, die bei einer Konvertierung Fehler verursachen würden:
    uint16_t counter{0}; // Zählt Position mit

    while (!isdigit(str_number[counter]) | (str_number[counter] == '-'))
        counter++; // Aussage ( !A | B ) validiert!

    // wurde in obiger Schleife unzulässige Zeichen gefunden, neuen Index Teilstring bekommen, dazu "Anzahl counter"-Felder vorrücken:
    if (counter > 0)
        str_number = str_number.substr(counter, len); // sind strings änderbar?
    //str_number = getSubString(source, start + counter, end);
    // **************************************************************************

    const int32_t number{std::stoi(str_number)}; // string in int konvertieren

    return number;
}

double getDouble(const std::string &source, int32_t start, int32_t len, bool leadingdecimalpointassumed) // Gibt Nummer aus TLE als Double zurück (mit Column-Daten aus Definition arbeiten!)
{
    std::string str_number{source.substr(start, len)}; // getSubString(source, start, end); // Zeiger auf Teilstring

    {
        // ******************************** WICHTIG ! *******************************
        // Prüfen ob Zeichen im String stehen, die bei einer Konvertierung Fehler verursachen würden:
        uint16_t counter{0};

        while (!(isdigit(str_number[counter])) | (str_number[counter] == '-'))
            counter++; // Aussage ( !A | B ) validiert!

        // wurde in obiger Schleife unzulässige Zeichen gefunden, neuen Index Teilstring bekommen, dazu [Anzahl counter]-Felder vorrücken:
        if (counter > 0)
            str_number = str_number.substr(counter, len);
        // **************************************************************************
    }

    {
        // ************************ Führender Dezimalpunkt **************************

        if (leadingdecimalpointassumed) // Soll an erster Stelle noch ein Komma eingefügt werden:
        {
            ////str_number = '.' + str_number;
            str_number.insert(str_number.begin(), '.');
        }
        // ***************************************************************************
    }
    // ############################################################################
    // Hier darauf achten, dass Gleitkommazahlen ihren Exponenten bekommen!
    // Diesen hier rausfiltern:

    // Alle Zeichen einzeln scannen und nach Exp.-Anteil suchen: (Format: -NNNNNN-N)
    uint16_t pos{0};
    // von hinten nach vorne nach einem Minus suchen (das ist dann der Exponent). Erste Stelle auslassen, könnte ein normales Vorzeichen sein!
    // Letzte (und erste) Stelle ebenfalls auslassen, ein Minus dort ergäbe keinen Sinn. (NNNN-) (Deswegen i=...-1 !)
    for (uint32_t i {(str_number.length()) - 1}; i > 1; i--)
    {
        if (str_number[i] == '-' || str_number[i] == '+')
        {
            pos = i; // Position des Minus merken

            break;
        }
    }

    double number{0.0}; // enthält später Gleitkommazahl

    if (pos != 0) // wird nur ausgeführt, wenn ein Exponent erkannt wurde (Minus inmitten der Zahl)
    {
        std::string exponent {str_number.substr(pos + 1, len - pos)};

        // Exponenten als Integer darstellen:
        int32_t exp = (std::stoi(exponent));
        if (str_number[pos] == '-')
            exp = -exp; // ggf. Vorzeichenwechsel

        // Mantisse in double konvertieren
        std::string mantisseptr {str_number.substr(0, pos - 1)};

        // Mantisse zusammensetzen: (ruft Lambda auf!)
        double mantisse {std::stod(mantisseptr)};

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

double get_a(double f) // Berechnet über die mittlere Bewegungszeit und der Konstanten GM die große Halbachse a in Kilometern [km] !
{
    if (f == 0)
        return 0; // Fall T = 0 abdecken (durch Null teilen!)

    const double T_in_s{2 * M_PI * 60 * (1 / f)}; // Enthält Anzahl Sekunden pro Umlauf (T)

    return (cbrt(0.25 * GM * (powf(T_in_s / M_PI, 2)))) / 1000; // Berechnung in km!
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
    for (uint8_t i = 0; i < 10; i++)
    {
        if (f(x0) == 0)
            break; // Division durch Null abfangen

        x0 = x0 - (F(x0) / f(x0)); // Newton-Verfahren
    }                              // while (counter < 5);//std::fabs(F(x0)) > 1e-10);

    // exzentrische Anomalie in wahre Anomalie umrechnen:
    const double trueAnomaly{2 * atan2((tan(x0 / 2) * sqrt((1 + e) / (1 - e))), 1)};

    // True Anomaly

    return (trueAnomaly < 0 ? trueAnomaly + (2 * M_PI) : trueAnomaly);
}

double revPerDay2RadPerMin(double value) // Wandelt Zahl von [rev/day] in [rad/min] um
{
    return (value * (2 * M_PI) / (60 * 24));
}

double radPerMin2RevPerDay(double value) // Wandelt Zahl von [rad/min] in [rev/day] um
{
    return (value * (60 * 24) / (2 * M_PI));
}

double deg2rad(double angle) // Wandelt [degree] in [rad] um
{
    return (angle * M_PI / 180.0);
}

double rad2deg(double rad) // Wandelt [rad] in [degree] um
{
    return (rad * 180.0 / M_PI);
}

int32_t checkyear(unsigned int value)
{
    // NORAD definiert Epochen wie folgt:
    // 57 - 99 entspricht 1957 - 1999
    // 00 - 56 entspricht 2000 - 2056
    if (value < 57)
        return (2000 + value);
    else if (value > 56)
        return (1900 + value);
    else
        return -1;
}
