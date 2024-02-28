#include "init.h"
#include <galay/kernel/callback.h>
#include <signal.h>
using namespace galay;

Tcp_Server<GS_Proto_Request,GS_Proto_Response>::uptr server;


Task<> func(Task_Base::wptr task)
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
    task.lock()->control_task_behavior(GY_TASK_WRITE);
    return {};
}

void sig_handle(int sig)
{
    printf("stop......\n");
    g_store.reset();
    server->stop();
    printf("stop success\n");
}

int main()
{
    signal(SIGINT,sig_handle);
    Callback_ConnClose::set([](int fd){
        while(!g_affair_queue.empty()){
            g_affair_queue.pop();
        }
        while(!g_back_stack.empty()){
            g_back_stack.pop();
        }
        GS_Executor::affairing() = false;
    });
    auto config = Config_Factory::create_tcp_server_config(8080, Engine_Type::ENGINE_EPOLL, 5, -1, 1);
    server = std::make_unique<Tcp_Server<GS_Proto_Request,GS_Proto_Response>>(config);
    server->start(func);
    return 0;
}