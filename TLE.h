#ifndef TLE_H
#define TLE_H

#include <string>

struct Tle
{
    private:
    // Variablen
    char satelliteName[25] = { '\0' }; ///< name of the satellite
    int satelliteNr; ///< satellite catalog nr
    char intDesignator[9] = { '\0' }; ///< international designator
    int year; ///< year of the epoch (e. g. 2020)
    double dayFraction; ///< day fraction of the epoch
    double bStar; ///< drag term

    double inclination; ///< inclination [rad]
    double raan; ///< right ascension of ascending node [rad]
    double eccentricity; ///< eccentricity of the orbit
    double argumentOfPerigee; ///< argument of perigee [rad]
    double meanAnomaly; ///< mean anomaly [rad]
    double meanMotion; ///< mean motion [rad/min]

    bool valid = false; ///< indicates of the TLE is valid

    public:
    std::string getSatelliteName();
    int getSatelliteNr();
    std::string getintDesignator();
    int getYear();
    double getDayFraction();
    double getbStar();

    double getInclination();
    double getRaan();
    double getEccentricity();
    double getArgumentOfPerigee();
    double getMeanAnomaly();
    double getMeanMotion();


    // ******

    Tle(char* line0, char* line1, char* line2); // Konstruktor
    
    // Funktionen
    void populateTle(char* line0, char* line1, char* line2); // wird von Konstruktor aufgerufen
    bool isTleLineValid(const char* line); // prüft für übergebene Zeile Gültigkeit (Prüfsumme)
    void print(void); // gibt gesamtes TLE auf Konsole aus
};

#endif // TLE_H