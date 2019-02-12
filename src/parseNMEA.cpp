#include <vector>
#include <string>
#include "parseNMEA.h"
#include "position.h"
#include "types.h"

bool GPS::isValidSentence(const std::string &) 
{
    return false;
}

GPS::NMEAPair GPS::decomposeSentence(const std::string & nmeaSentence) 
{
    std::vector<std::string> tempVect;
    GPS::NMEAPair temp = {"Temp", tempVect};
    
    return temp;
}

GPS::Position GPS::extractPosition(const NMEAPair &)
{
    return GPS::Position(1, 1, 1);
}

std::vector<GPS::Position> routeFromNMEALog(const std::string & filepath)
{
    std::vector<GPS::Position> tempVect;
    
    return tempVect;
}
