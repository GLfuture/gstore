#include "cmdparser.h"
#include <galay/util/stringutil.h>

std::vector<std::string> GS_Parser::parse(const std::string& cmd)
{
    return galay::StringUtil::Spilt_With_Char_Connect_With_Quote(cmd,' ');
}
