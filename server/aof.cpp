#include "aof.h"
#include "init.h"
#include <filesystem>
#include <fstream>
#include <chrono>

void Aof_Queue::push(std::vector<std::string> &&cmd)
{
    std::lock_guard lock(m_mtx);
    m_cmds.push(cmd);
    m_cond.notify_one();
}

std::vector<std::string> Aof_Queue::pop()
{
    std::lock_guard lock(m_mtx);
    auto res = m_cmds.front();
    m_cmds.pop();
    return std::move(res);
}

bool Aof_Executor_Base::recovery(std::string path)
{
    if(!std::filesystem::exists(path)) return false;
    std::ifstream in(path);
    long size = std::filesystem::file_size(path);
    char* temp = new char[size];
    in.read(temp,size);
    in.close();
    std::string res(temp,size);
    delete[] temp;
    std::vector<std::string> cmds = galay::StringUtil::Spilt_With_Char(res,'\n');
    galay::Scheduler_Base::ptr nulls = nullptr;
    GS_Proto_Head head;
    for(int i = 0 ;i < cmds.size() ; i ++){
        std::vector<std::string> cmd = galay::StringUtil::Spilt_With_Char(cmds[i],' ');
        g_executor->execute(nulls,head,std::move(cmd));
    }
    return true;
}


int Aof_Executor_Base::write()
{
    std::string res;
    if(g_aof_queue->m_cmds.empty()) return 0;
    while(!g_aof_queue->m_cmds.empty()){
        auto cmd = g_aof_queue->pop();
        for(const auto& v: cmd){
            res = res + v + ' ';
        }
        res[res.size()-1] = '\n';
    }
    std::ofstream out(GSTORE_AOF_FILE,std::ios_base::app);
    if(out.fail()){
        perror("open");
        return -1;
    }
    out << res;
    out.flush();
    out.close();
    return 0;
}

bool Aof_Executor_Base::is_finish()
{
    return m_finish;
}

void Sync_Per_Second_Aof_Executor::exec()
{
    std::mutex mtx;
    std::unique_lock lock(mtx);
    while(!m_stop)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        write();
    }
    m_finish = true;
}

void Sync_Per_Second_Aof_Executor::stop() 
{
    m_stop = true;
}



void Sync_Per_Write_Aof_Executor::exec()
{
    std::mutex mtx;
    std::unique_lock lock(mtx);
    while (!m_stop)
    {
        g_aof_queue->m_cond.wait(lock, [this]()
                                 { return m_stop || !g_aof_queue->m_cmds.empty(); });
        if (m_stop)
            break;
        write();
    }
    m_finish = true;
}

void Sync_Per_Write_Aof_Executor::stop()
{
    m_stop = true;
    g_aof_queue->m_cond.notify_all();
}
