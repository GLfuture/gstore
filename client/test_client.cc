#include <galay/factory/factory.h>
#include "../common/gscmds.h"
#include <algorithm>

#define THREAD_NUM      8
#define MAX_CMD_LENGTH  1500
using namespace galay;

Task<> func(Scheduler_Base::wptr scheduler,int times,char* argv)
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
    for(int i = 0 ; i < times ; i ++)
    {
        bzero(buffer,MAX_CMD_LENGTH);
        bzero(msg,MAX_CMD_LENGTH+4);
        std::string cmd(argv);
        req->get_body() = cmd;
        head.m_length = cmd.length();
        ret = co_await client->request(req,resp);
        if(ret == -1) {
            std::cout << strerror(errno) <<'\n';
            printf("request fail\n");
            break;
        }
    }
    scheduler.lock()->stop();
    
    co_return;
}

void thread_func(char* argv[])
{
    auto scheduler = Scheduler_Factory::create_epoll_scheduler(1024,10);
    auto task = func(scheduler,atoi(argv[1]),argv[2]);
    scheduler->start();
}


int main(int argc,char*argv[])
{
    if(argc != 3) return 0;
    std::vector<std::thread> ths(THREAD_NUM);
    auto start = std::chrono::system_clock::now();
    for(int i = 0 ; i < THREAD_NUM ; i ++)
    {
        ths[i] = std::thread(thread_func,argv);
    }
    for(int i = 0 ;i < THREAD_NUM ;i ++)
    {
        if(ths[i].joinable()) ths[i].join();
    }
    auto end = std::chrono::system_clock::now();
    printf("qps : %lf \n",static_cast<double>(atoi(argv[1]) * THREAD_NUM * 1.0) /std::chrono::duration_cast<std::chrono::seconds>(end-start).count());
    printf("finish test\n");
    return 0;
}