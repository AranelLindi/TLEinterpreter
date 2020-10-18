#pragma once

// Standartbibliotheken
#include <iostream> // Konsolenausgabe ( hier mit printf() )
#include <ctype.h>  // Prüfungen von Charactern
#include <string>   // Länge von char-Arrays
#include <cstdint>  // für int-Types

// eigener Code
#include "cdefines.h" // Spaltennummern (TLE Definition)

class Tle
{
    //private: // Private ist Standartzugriffslevel bei Klassen!

    // Variablen
    char satelliteName[25]{'\0'}; ///< name of the satellite
    int32_t satelliteNo;          ///< satellite catalog nr
    char intDesignator[9]{'\0'};  ///< international designator
    int32_t year;                 ///< year of the epoch (e. g. 2020)
    double dayFraction;           ///< day fraction of the epoch
    double bStar;                 ///< drag term

    double inclination;       ///< inclination [rad]
    double raan;              ///< right ascension of ascending node [rad]
    double eccentricity;      ///< eccentricity of the orbit
    double argumentOfPerigee; ///< argument of perigee [rad]
    double meanAnomaly;       ///< mean anomaly [rad]
    double meanMotion;        ///< mean motion [rad/min]

    bool valid{false}; ///< indicates of the TLE is valid

public:
    // Getter
    std::string getSatelliteName() const noexcept;
    int32_t getSatelliteNo() const noexcept;
    std::string getintDesignator() const noexcept;
    int32_t getYear() const noexcept;
    double getDayFraction() const noexcept;
    double getBstar() const noexcept;

    double getInclination() const noexcept;
    double getRaan() const noexcept;
    double getEccentricity() const noexcept;
    double getArgumentOfPerigee() const noexcept;
    double getMeanAnomaly() const noexcept;
    double getMeanMotion() const noexcept;

    // ******
    Tle() noexcept;                                                                    // Standartkonstruktor (wird von Interpreter nicht verwendet!)
    Tle(const std::string &line0, const std::string &line1, const std::string &line2); // Konstruktor

    // Funktionen
    void populateTle(const std::string &line0, const std::string &line1, const std::string &line2); // wird von Konstruktor aufgerufen
    bool isTleLineValid(const std::string &line) const noexcept;                                    // prüft für übergebene Zeile Gültigkeit (Prüfsumme)
    void print(void) const noexcept;                                                                // gibt gesamtes TLE auf Konsole aus
};