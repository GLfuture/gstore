#include "init.h"

DB_Store::uptr g_store = std::make_unique<DB_Store>();

std::queue<std::vector<std::string>> g_affair_queue;

std::stack<std::vector<std::string>> g_back_stack;

GS_Executor::uptr g_executor = std::make_unique<GS_Executor>();