#include <string>
#include <cmath>

#include "format.h"

using std::string;

// Format time in seconds to HH:MM:SS format.
// Input (long)
// Output (string)
string Format::ElapsedTime(long seconds) { 
    long HH, MM, SS; 

    if(seconds > 0){
        HH = floor(seconds / 3600);
        MM = (seconds / 60) % 60;
        SS = seconds % 60;
    } else {
        return string();
    }
    return TimeToString(HH) + ":" + TimeToString(MM) + ":" + TimeToString(SS); 
}

// Check if time is less than 10 then convert to a string
string Format::TimeToString(long time) {
    if(time < 10){
        return "0" + std::to_string(time);
    }
    return std::to_string(time);
}