Ist lediglich die Ausgabe des SONATE (44400) gewünscht, in der Datei main.cpp, die globale Variable SONATE_only auf ‚true‘ setzen.
Für Ausgabe, aller TLEs, Wert auf ‚false‘ setzen.

Um die Code Dateien einzubinden genügt ein include auf main.h !

Build mit Kommando:
g++ -o [execute-Name] main.cpp TLE.cpp process.cpp -std=c++17


- Stefan Lindörfer
----------------
26. Mai 2020: Korrekturänderungen (Uni) vorgenommen
Main Funktion ausgelagert, Dateien umbenannt
