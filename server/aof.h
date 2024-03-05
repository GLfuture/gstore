#ifndef GS_AOF_H
#define GS_AOF_H
#include <queue>
#include <vector>
#include <string>
#include <memory>
#include <condition_variable>
#include <thread>
#include <mutex>

class Aof_Queue
{
public:
    using uptr = std::unique_ptr<Aof_Queue>;
    void push(std::vector<std::string>&& cmd);

    std::vector<std::string> pop();

    std::queue<std::vector<std::string>> m_cmds;
    std::condition_variable m_cond;
    std::mutex m_mtx;
};

class Aof_Executor_Base{
public:
    using uptr = std::unique_ptr<Aof_Executor_Base>;
    virtual void exec() = 0;
    virtual void stop() = 0;
    static bool recovery(std::string path);
    bool is_finish();
protected:
    int write();
protected:
    bool m_stop = false;
    bool m_finish = false;
};

class Sync_Per_Second_Aof_Executor : public Aof_Executor_Base
{
public:
    using uptr = std::unique_ptr<Sync_Per_Second_Aof_Executor>;
    void exec() override;
    void stop() override;
};

class Sync_Per_Write_Aof_Executor : public Aof_Executor_Base
{
public:
    using uptr = std::unique_ptr<Sync_Per_Write_Aof_Executor>;
    void exec() override;
    void stop() override;
};

#endif