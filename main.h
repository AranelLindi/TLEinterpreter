#ifndef MAIN_H
#define MAIN_H

// Standartbibliotheken
#include <map>

// eigener Code
#include "TLE.h"

// Konstanten (für Aufgabe 1.2)
const double GM = 3.986005e14; // Gravitationskonstante * Masse d. Erde

// Funktionenrümpfe
std::map<int, TLE> readTLesFromFile(const char* filename); // Aufgabenstellung

char* getSubString(const char* source, int start, int end); // Substring aus Zeile
int getInteger(const char* source, int start, int end); // Integer aus Substring
double getDouble(const char* source, int start, int end, bool leadingdecimalpointassumed); // Double aus Substring

char* convertConstCharPtrToCharPtr(std::string sstring); // string.c_string (const char*) in char* konvertieren

void stringcopy(char* source, char* dest); // kopiert ein char-Array in ein anderes

// ################################
// zu Aufgabe 1.2:
double a(double T); // Berechnet große Halbachse
double ny(double e, double M); // Berechnet Wahre Anomalie
// ################################

int main(void); // klar

#endif /// MAIN_H