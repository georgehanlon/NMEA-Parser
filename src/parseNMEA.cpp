#include <sstream>
#include <algorithm>
#include <fstream>
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
                for (int i = 1; i < checksumChars.length(); i++)
                {
                    checksum = checksum ^ checksumChars[i+1];
                }

                std::string log_hex_checksum = s.substr(s.size()-2,2);

                transform(log_hex_checksum.begin(), log_hex_checksum.end(), log_hex_checksum.begin(), ::tolower);

                std::stringstream hexVal;
                hexVal << std::hex << checksum;
                std::string result (hexVal.str());

                if (result == log_hex_checksum)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

GPS::NMEAPair GPS::decomposeSentence(const std::string &nmeaSentence)
{
    std::vector<std::string> fields;
    std::string type = nmeaSentence.substr(1,5);
    std::string sentenceContents = nmeaSentence.substr(7, nmeaSentence.size()-10); //Making temporary variable as passed value is constant
    std::string word;
    std::stringstream stream(sentenceContents);
    while(std::getline(stream, word, ','))
    {
        fields.push_back(word);
    }

    GPS::NMEAPair result = {type, fields};

    return result;
}

GPS::Position GPS::extractPosition(const NMEAPair &p)
{
    std::string lat, lon, ele;
    ele = "0";
    char northing, easting;

    if (p.second.size() < 5)
    {
        throw std::invalid_argument("Ill-formed or unsupported sentence types (less than 5 elements in sentence).");
    }

    for (int i = 0; i < p.second.size(); i++)
    {
        if (p.second[i][0] == 'N' || p.second[i][0] == 'S')
        {
            northing = p.second[i][0];
            lat = p.second[i-1];
        }
        if (p.second[i][0] == 'E' || p.second[i][0] == 'W')
        {
            easting = p.second[i][0];
            lon = p.second[i-1];
        }
        if (p.second[i][0] == 'M')
        {
            ele = p.second[i-1];
            break;
        }
    }

    return GPS::Position(lat, northing, lon, easting, ele);
}

std::vector<GPS::Position> GPS::routeFromNMEALog(const std::string &filepath)
{
    GPS::NMEAPair tempPair;
    std::vector<GPS::Position> positionsVector;
    std::ifstream sentenceFile(filepath);
    for (std::string line; std::getline(sentenceFile, line);)
    {
        if (GPS::isValidSentence(line) == true)
        {
            tempPair = GPS::decomposeSentence(line);
            positionsVector.push_back(GPS::extractPosition(tempPair));
        }
    }

    return positionsVector;
}
