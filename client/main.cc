#include <galay/factory/factory.h>
#include "../common/gscmds.h"
#include <algorithm>

#define MAX_CMD_LENGTH  1500
using namespace galay;

Task<> func(Scheduler_Base::wptr scheduler)
{
    auto client = Client_Factory::create_tcp_self_define_client(scheduler);
    int ret = co_await client->connect("127.0.0.1",8080);
    if(ret == -1) {
        printf("connect fail\n");
        co_return;
    }else{
        printf("connect success\n");
    }
    char buffer[MAX_CMD_LENGTH] = {0};
    char msg[MAX_CMD_LENGTH + 4] = {0};
    GS_Proto_Request::ptr req = std::make_shared<GS_Proto_Request>();
    GS_Proto_Response::ptr resp = std::make_shared<GS_Proto_Response>();
    auto& head = req->get_head();
    head.m_version = VERSION1_0;
    while(1)
    {
        bzero(buffer,MAX_CMD_LENGTH);
        bzero(msg,MAX_CMD_LENGTH+4);
        printf("kvstore > ");
        std::cin.getline(buffer,MAX_CMD_LENGTH);
        std::string cmd(buffer);
        //real cmd
        std::vector<std::string> subcmds = galay::StringUtil::Spilt_With_Char_Connect_With_Quote(cmd,' ');
        std::transform(subcmds[0].begin(),subcmds[0].end(),subcmds[0].begin(),[](unsigned char c){
            return std::tolower(c);
        });
        if(subcmds[0].compare("quit") == 0) break;
        //cmd 规范化
        cmd.clear();
        for(auto s:subcmds){
            cmd = cmd + s + ' ';
        }
        cmd.erase(cmd.length()-1);
        req->get_body() = cmd;
        head.m_length = cmd.length();
        ret = co_await client->request(req,resp);
        if(ret == -1) {
            std::cout << strerror(errno) <<'\n';
            printf("request fail\n");
        }
        switch (resp->get_head().m_ret_code)
        {
        case RET_OK:
        {
            if (resp->get_head().m_length == 0)
                std::cout << "OK\n";
            else
                std::cout << resp->get_body() << '\n';
        }
            break;
        case RET_ERR_CMD:
            std::cout << g_ret_str[RET_ERR_CMD_STR] << '\n';
            break;
        case RET_FAIL:
            std::cout << g_ret_str[RET_FAIL_STR] << '\n';
            break;
        case RET_INVAILD_ARG:
            std::cout << g_ret_str[RET_INVAILD_ARG_STR] << '\n';
            break;
        case RET_NO_KEY:
            std::cout << g_ret_str[RET_NO_KEY_STR] << '\n';
            break;
        case RET_AFFAIRING:
            std::cout << g_ret_str[RET_AFFATING_STR] << '\n';
            break;
        default:
            break;
        }
    }
    scheduler.lock()->stop();
    printf("Bye\n");
    co_return;
}

int main()
{
    auto scheduler = Scheduler_Factory::create_epoll_scheduler(1024,10);
    auto task = func(scheduler);
    scheduler->start();
    return 0;
}