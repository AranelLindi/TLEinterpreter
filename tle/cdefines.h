#ifndef CDEFINES_H
#define CDEFINES_H

// --LINE 0

#define Satellite_Name_pos 0
#define Satellite_Name_length 24

// --LINE 1

#define Line_Number_pos 0
#define Line_Number_length 1

#define Satellite_Number_pos 2
#define Satellite_Number_length 5

#define Classification_pos 7
#define Classification_length 1

#define International_Designator_Year_pos 9
#define International_Designator_Year_length 2

#define International_Designator_NoOfYear_pos 11
#define International_Designator_NoOfYear_length 3

#define International_Designator_PieceOfLaunch_pos 14
#define International_Designator_PieceOfLaunch_length 3

#define Epoch_Year_pos 18
#define Epoch_Year_length 2

#define Epoch_Day_Fraction_pos 20
#define Epoch_Day_Fraction_length 12

#define First_Time_Derivative_Mean_Motion_pos 33
#define First_Time_Derivative_Mean_Motion_length 10

#define Second_Time_Derivative_Mean_Motion_pos 44
#define Second_Time_Derivative_Mean_Motion_length 8

#define BSTAR_pos 53
#define BSTAR_length 8

#define Ephemeris_pos 62
#define Ephereris_length 1

#define Element_Number_pos 64
#define Element_Number_length 4

#define Checksum_pos 68
#define Checksum_length 1

// --LINE 2
// ## Line Number identisch zu Line 1!

// ## Satellite Number identisch zu Line 1!

#define Inclination_pos 8
#define Inclination_length 8

#define Right_Ascension_Of_Ascending_Node_pos 17
#define Right_Ascension_Of_Ascending_Node_length 8

#define Eccentricity_pos 26
#define Eccentricity_length 7

#define Argument_Of_Perigee_pos 34
#define Argument_Of_Perigee_length 8

#define Mean_Anomaly_pos 43
#define Mean_Anomaly_length 8

#define Mean_Motion_pos 52
#define Mean_Motion_length 11

#define Revolution_Number_At_Epoch_pos 63
#define Revolution_Number_At_Epoch_length 5

// ## Checksum identisch zu Line 1!

// Aufgabe 1.2 (Sonstiges)
#define SONATE_NUMBER 44400

#endif // CDEFINES_H
