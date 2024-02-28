#ifndef GS_CMDS_H
#define GS_CMDS_H

#include <memory>
#include <string>
#include <vector>
#include <galay/factory/factory.h>
#include "gsproto.h"

enum Ret_Code{
    RET_OK = 0,
    RET_FAIL = 1,
    RET_NO_KEY = 4,
    RET_ERR_CMD = 5,
    RET_INVAILD_ARG = 6,
    RET_AFFAIRING = 8,
};


enum Ret_Str{
    RET_OK_STR,
    RET_FAIL_STR,
    RET_TRUE_STR,
    RET_FALSE_STR,
    RET_NO_KEY_STR,
    RET_ERR_CMD_STR,
    RET_INVAILD_ARG_STR,
    RET_INDX_OVERFLOW_STR,
    RET_AFFATING_STR,
    RET_QUEUED_STR,
};

extern const char* g_ret_str[];

enum Cmd_Type{
    //string
    CMD_SET,
    CMD_GET,
    CMD_DELETE,
    CMD_EXIST,
    CMD_APPEND,
    CMD_LEN,
    //vector
    CMD_VSET,
    CMD_VGET,
    CMD_VDELETE,
    CMD_VINDX,
    CMD_VERASE,
    CMD_VEXIST,
    CMD_VAPPEND,
    CMD_VSIZE,
    //list
    CMD_LSET,
    CMD_LGET,
    CMD_LDELETE,
    CMD_LERASE,
    CMD_LEXIST,
    CMD_LPUSH,
    CMD_RPUSH,
    CMD_LSIZE,
    //rbstree
    CMD_RBSET,
    CMD_RBGET,
    CMD_RBDELETE,
    CMD_RBERASE,
    CMD_RBEXIST,
    CMD_RBAPPEND,
    CMD_RBSIZE,
    //hash
    CMD_HSET,
    CMD_HGET,
    CMD_HDELETE,
    CMD_HERASE,
    CMD_HEXIST,
    CMD_HAPPEND,
    CMD_HSIZE,
    
    CMD_END
};

class GS_Cmd_Base{
public:
    using ptr = std::shared_ptr<GS_Cmd_Base>;
    using uptr = std::unique_ptr<GS_Cmd_Base>;
    virtual std::string name() = 0;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing = false) = 0;
    virtual ~GS_Cmd_Base(){};
};

#endif