#pragma once

// Standartbibliotheken
#include <fstream>   // um txt-Datei zu lesen
#include <cstring>   // strcpy
#include <cmath>     // für Potenzierung & trig. Fkt
#include <algorithm> // für for_each
#include <map>       // für map<> Objekt
//#include <array>  // std::array<T>
#include <memory>  // smart pointer
#include <string>  // std::string
#include <cstdint> // uint-types

// eigener Code
#include "../tle/cdefines.h" // Spaltendefinitionen TLE
#include "../tle/TLE.h"      // für Objekt Tle

extern const bool SONATE_ONLY;

// Konstanten (für Aufgabe 1.2)
const float GM{3.985892856e14f}; // Gravitationskonstante * Masse d. Erde

// Funktionenrümpfe
std::map<int32_t, Tle> readTlesFromFile(const char *filename); // Aufgabenstellung

int32_t getInteger(const std::string &source, int32_t start, int32_t end);                                     // Integer aus Substring
double getDouble(const std::string_view &source, int32_t start, int32_t end, bool leadingdecimalpointassumed); // Double aus Substring

double revPerDay2RadPerMin(double value) noexcept; // Wandelt Einheit [rev/day] in [rad/min] um

double radPerMin2RevPerDay(double value) noexcept; // Wandelt Einheit [rad/min] in [rev/day] um

double deg2rad(double angle) noexcept; // Wandelt Winkel in [rad] um

double rad2deg(double rad) noexcept; // Wandelt Rad in Winkel um

int32_t checkyear(uint32_t value) noexcept;

// ################################
// zu Aufgabe 1.2:

double get_a(double n) noexcept;                    // Berechnet große Halbachse in [km]
double getTrueAnomaly(double e, double M) noexcept; // Berechnet Wahre Anomalie

// ################################