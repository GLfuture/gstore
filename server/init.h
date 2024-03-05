#ifndef GS_INIT_H
#define GS_INIT_H

#include "dbstore.h"
#include "../common/gsproto.h"
#include "cmdparser.h"
#include "cmdexecutor.h"
#include "aof.h"
#include <queue>
#include <stack>
#include <galay/factory/factory.h>

#define GSTORE_AOF_FILE     "gstore.aof"
#define GSTORE_CONFIG_PATH  "../conf/gstore.conf"

extern DB_Store::uptr g_store;

extern std::queue<std::vector<std::string>> g_affair_queue;

extern std::stack<std::vector<std::string>> g_back_stack;

extern GS_Executor::uptr g_executor;


//aof
extern bool g_aof_on;

extern Aof_Queue::uptr g_aof_queue;

extern Aof_Executor_Base::uptr g_aof_executor;

extern std::thread g_aof_th;

extern galay::Tcp_Server<GS_Proto_Request,GS_Proto_Response>::uptr g_server;

extern void init_conf();

#endif