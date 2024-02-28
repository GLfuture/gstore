#ifndef GS_CMD_EXECUTER_H
#define GS_CMD_EXECUTER_H

#include <vector>
#include <string>
#include "../common/gscmds.h"

class GS_Keys: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Set: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Get: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Delete: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Exist: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Len: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Expire: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_TTL: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Vget: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Vdelete: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Vindx: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Verase: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Vinsert: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Vexist: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Vappend: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Vsize: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Vexpire: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Vttl: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Lget: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Ldelete: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Lerase: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Lexist: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Lpush: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Rpush: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Lsize: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Lexpire: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Lttl: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};


class GS_Rbget: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Rbdelete: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Rberase: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Rbexist: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Rbappend: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Rbsize: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Rbkeys: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Rbexpire: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Rbttl: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Hget: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Hdelete: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};


class GS_Herase: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Hexist: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Happend: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Hsize: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Hexpire: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Httl: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Affair: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Commit: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Rollback: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

//To do rollback queue clear
class GS_Discard: public GS_Cmd_Base
{
public:
    virtual std::string name() override;
    virtual std::string exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing) override;
};

class GS_Executor
{
public:
    using uptr = std::unique_ptr<GS_Executor>;
    GS_Executor();
    //return body
    std::string execute(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing = false);
    static bool& affairing();
    ~GS_Executor();
private:
    std::unordered_map<std::string,GS_Cmd_Base::uptr> m_cmds;
    static bool m_affair;
};


#endif