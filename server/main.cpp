#include "init.h"
#include <signal.h>

galay::Task<> func(galay::Task_Base::wptr task)
{
    auto request = std::dynamic_pointer_cast<GS_Proto_Request>(task.lock()->get_req());
    auto response = std::dynamic_pointer_cast<GS_Proto_Response>(task.lock()->get_resp());
    auto& head = response->get_head();
    head.m_version = VERSION1_0;
    auto cmds = GS_Parser::parse(request->get_body());
    std::string retstr;
    if(!task.lock()->get_ctx().has_value()) task.lock()->get_ctx() = false;
    if(GS_Executor::affairing() && !std::any_cast<bool>(task.lock()->get_ctx())){
        head.m_ret_code = RET_AFFAIRING;
        retstr = g_ret_str[RET_AFFATING_STR];
    }else{
        if(cmds[0].compare("affair") == 0) task.lock()->get_ctx() = true;
        retstr = g_executor->execute(task.lock()->get_scheduler(),head, std::move(cmds));
    }
    head.m_length = retstr.length();
    response->get_body() = retstr;
    task.lock()->control_task_behavior(galay::GY_TASK_WRITE);
    return {};
}

void sig_handle(int sig)
{
    printf("stop......\n");
    g_store.reset();
    g_server->stop();
    g_aof_executor->stop();
    if(g_aof_th.joinable()){
        g_aof_th.join();
    };
    printf("stop success\n");
}

int main()
{
    signal(SIGINT,sig_handle);
    init_conf();
    g_server->start(func);
    return 0;
}