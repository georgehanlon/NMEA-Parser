#include <vector>
#include <string>
#include "parseNMEA.h"
#include "position.h"
#include "types.h"

bool GPS::isValidSentence(const std::string &s)
{
    if (s.substr(0,3) == "$GP")
    {
        std::string type = s.substr(3,3);
        if (type == "GLL" || type == "GGA" || type == "RMC")
        {
            int endPos = s.size()-3;
            if (s[endPos] == '*')
            {
                std::string checksumChars = s.substr(1, endPos-1);
                int checksum = checksumChars[0] ^ checksumChars[1];
                for (int i = 1; i < checksumChars[checksumChars.length()-1]; i++)
                {
                    checksum = checksum ^ checksumChars[i+1];
                }
                if (std::to_string(checksum) == s.substr(s.size()-2,2))
                {
                    return true;
                }
            }
        }
    }

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

std::vector<GPS::Position> GPS::routeFromNMEALog(const std::string & filepath)
{
    std::vector<GPS::Position> tempVect;
    
    return tempVect;
}
