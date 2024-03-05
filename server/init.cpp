#include "init.h"
#include <galay/kernel/callback.h>
#include <galay/util/conf_parser.h>

DB_Store::uptr g_store = std::make_unique<DB_Store>();

std::queue<std::vector<std::string>> g_affair_queue;

std::stack<std::vector<std::string>> g_back_stack;

GS_Executor::uptr g_executor = std::make_unique<GS_Executor>();

Aof_Queue::uptr g_aof_queue = std::make_unique<Aof_Queue>();

Aof_Executor_Base::uptr g_aof_executor;

std::thread g_aof_th;

bool g_aof_on = false;

galay::Tcp_Server<GS_Proto_Request,GS_Proto_Response>::uptr g_server;

void init_conf()
{
    Callback_ConnClose::set([](int fd){
        while(!g_affair_queue.empty()){
            g_affair_queue.pop();
        }
        while(!g_back_stack.empty()){
            g_back_stack.pop();
        }
        GS_Executor::affairing() = false;
    });
    galay::Conf_Parser parser;
    parser.parse(GSTORE_CONFIG_PATH);
    auto config = galay::Config_Factory::create_tcp_server_config(std::stoi(parser.get_value(std::string("port"))), galay::Engine_Type::ENGINE_EPOLL
        , std::stoi(parser.get_value(std::string("scheduler_checktime")))\
        ,std::stoi(parser.get_value(std::string("connection_timeout"))));
    
    //To do recovery （must be here）
    Aof_Executor_Base::recovery(GSTORE_AOF_FILE);

    switch (std::stoi(parser.get_value(std::string("aof"))))
    {
    case 0:
        g_aof_on = false;
        break;
    case 1:
    {
        g_aof_on = true;
        g_aof_executor = std::make_unique<Sync_Per_Second_Aof_Executor>();
        g_aof_th = std::thread([](){
            g_aof_executor->exec();
        });
    }
        break;
    case 2:
    {
        g_aof_on = true;
        g_aof_executor = std::make_unique<Sync_Per_Write_Aof_Executor>();
        g_aof_th = std::thread([](){
            g_aof_executor->exec();
        });
    }
        break;
    }
    g_server = std::make_unique<galay::Tcp_Server<GS_Proto_Request,GS_Proto_Response>>(config);
}