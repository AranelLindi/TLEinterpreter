executable:
		g++ -o executable main.cpp tle/TLE.cpp reader/reader.cpp -std=c++2a

#library:
#		g++ -o tle_lib TLE.cpp process.cpp -std=c++20

# Library ist nach jetzigem Kenntnisstand nicht möglich, da dann eine main-Funktion fehlen würde.
