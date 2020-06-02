#ifndef PROCESS_H
#define PROCESS_H

// Standartbibliotheken
#include <fstream>   // um txt-Datei zu lesen
#include <cstring>   // bequemere String-Verarbeitung
#include <cmath>     // für Potenzierung & trig. Fkt
#include <algorithm> // für for_each
#include <map>       // für map<> Objekt

// eigener Code
#include "cdefines.h" // Spaltendefinitionen TLE
#include "TLE.h"      // für Objekt Tle

extern const bool SONATE_ONLY;

// Konstanten (für Aufgabe 1.2)
const float GM = 3.985892856e14; // Gravitationskonstante * Masse d. Erde

// Funktionenrümpfe
std::map<int, Tle> readTlesFromFile(const char *filename); // Aufgabenstellung

char *getSubString(const char *source, int start, int end);                                // Substring aus Zeile
int getInteger(const char *source, int start, int end);                                    // Integer aus Substring
double getDouble(const char *source, int start, int end, bool leadingdecimalpointassumed); // Double aus Substring

char *convertConstCharPtrToCharPtr(std::string sstring); // string.c_string (const char*) in char* konvertieren

void stringcopy(char *source, char *dest); // kopiert ein char-Array in ein anderes

double convertRevPerDayInRadPerMin(double value); // Wandelt Einheit [rev/day] in [rad/min] um

double convertRadPerMinInRevPerDay(double value); // Wandelt Einheit [rad/min] in [rev/day] um

double convertDegreeInRadian(double angle); // Wandelt Winkel in [rad] um

double convertInDegree(double rad); // Wandelt Rad in Winkel um

int checkyear(unsigned int value);

// ################################
// zu Aufgabe 1.2:

double get_a(double T);                    // Berechnet große Halbachse in [km]
double getTrueAnomaly(double e, double M); // Berechnet Wahre Anomalie

// ################################

#endif /// PROCESSING_H