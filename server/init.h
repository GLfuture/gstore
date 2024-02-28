#ifndef GS_INIT_H
#define GS_INIT_H

#include "dbstore.h"
#include "../common/gsproto.h"
#include "cmdparser.h"
#include "cmdexecutor.h"
#include <queue>
#include <stack>

extern DB_Store::uptr g_store;

extern std::queue<std::vector<std::string>> g_affair_queue;

extern std::stack<std::vector<std::string>> g_back_stack;

extern GS_Executor::uptr g_executor;

#endif