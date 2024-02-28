#ifndef GS_CMDPARSER_H
#define GS_CMDPARSER_H

#include <vector>
#include <string>

class GS_Parser
{
public:
    static std::vector<std::string> parse(const std::string& cmd);
};



#endif