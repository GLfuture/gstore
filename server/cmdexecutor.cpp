#include "cmdexecutor.h"
#include "init.h"

std::string GS_Keys::name()
{
    return "keys";
}

std::string GS_Keys::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 1) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::string msg;
    std::vector<std::vector<std::string>> res = g_store->cmd_keys();
    for(int i = 0 ; i < STORE_SIZE ; i ++)
    {
        msg = msg + g_store_name[i] + '\n';
        for(int j = 0 ; j < res[i].size() ; j ++)
        {
            msg = msg + res[i][j] + '\n';
        }
    }
    msg.erase(msg.length() - 1);
    head.m_ret_code = RET_OK;
    return msg;
}

//set
std::string GS_Set::name()
{
    return "set";
}

std::string GS_Set::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if (cmds.size() != 3) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if (g_store->cmd_set(cmds[1], cmds[2]) == DB_OP_OK) head.m_ret_code = RET_OK;
    if(affairing) g_back_stack.push({"delete",cmds[1]});
    if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    return "";
}

//get
std::string GS_Get::name()
{
    return "get";
}

std::string GS_Get::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if (cmds.size() != 2)
    {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    auto str = g_store->cmd_get(cmds[1]);
    if (str.empty()) head.m_ret_code = RET_NO_KEY;
    else head.m_ret_code = RET_OK;
    return str;
}

//
std::string GS_Delete::name()
{
    return "delete";
}

std::string GS_Delete::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::string value;
    if(affairing) value = g_store->cmd_get(cmds[1]);
    int ret = g_store->cmd_delete(cmds[1]);
    if(ret == DB_OP_NO_KEY) head.m_ret_code = RET_NO_KEY;
    else {
        head.m_ret_code = RET_OK;
        if(affairing) g_back_stack.push({"set",cmds[1],value});
        if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    }
    return "";
}

std::string GS_Exist::name()
{
    return "exist";
}

std::string GS_Exist::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    head.m_ret_code = RET_OK;
    if(g_store->cmd_exist(cmds[1])) return g_ret_str[RET_TRUE_STR];
    return g_ret_str[RET_FALSE_STR];
}

std::string GS_Len::name()
{
    return "len";
}

std::string GS_Len::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    int ret = g_store->cmd_len(cmds[1]);
    if(ret == DB_OP_NO_KEY){
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    head.m_ret_code = RET_OK;
    return std::to_string(ret);
}

std::string GS_Expire::name()
{
    return "expire";
}

std::string GS_Expire::exec(galay::Scheduler_Base::wptr scheduler, GS_Proto_Head &head, std::vector<std::string> &&cmds,bool affairing)
{
    if(cmds.size() != 3) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(affairing) {
        head.m_ret_code = RET_ERR_CMD;
        return "";
    }
    int seconds;
    try
    {
        seconds = std::stoi(cmds[2]);
    }
    catch(const std::invalid_argument& e)
    {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(!g_store->cmd_exist(cmds[1])) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    auto timer = scheduler.lock()->get_timer_manager()->add_timer(seconds * 1000 , 1 , [cmds](){
        g_store->cmd_delete(cmds[1]);
    });
    g_store->set_timer(STRING_STORE,cmds[1],timer);
    head.m_ret_code = RET_OK;
    return "";
}

std::string GS_TTL::name()
{
    return "ttl";
}
std::string GS_TTL::exec(galay::Scheduler_Base::wptr scheduler, GS_Proto_Head &head, std::vector<std::string> &&cmds,bool affairing)
{
    if(cmds.size() != 2){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(!g_store->cmd_exist(cmds[1])) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    auto timer = g_store->get_timer(STRING_STORE,cmds[1]);
    if(timer == nullptr){
        head.m_ret_code = RET_OK;
        return "-1";
    }
    return std::to_string((timer->get_expired_time() - galay::Timer::get_current_time()) / 1000);

}

std::string GS_Vget::name()
{
    return "vget";
}

std::string GS_Vget::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::string msg;
    std::vector<std::string> res = g_store->cmd_vget(cmds[1]);
    if(res.empty()) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    else head.m_ret_code = RET_OK;
    for(const auto& v: res){
        msg = msg + v + '\n';
    }
    msg.erase(msg.length() - 1);
    return msg;
}

std::string GS_Vdelete::name()
{
    return "vdelete";
}

std::string GS_Vdelete::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::vector<std::string> value;
    if(affairing) value = g_store->cmd_vget(cmds[1]);
    if(g_store->cmd_vdelete(cmds[1]) == DB_OP_NO_KEY) head.m_ret_code = RET_NO_KEY;
    else {
        head.m_ret_code = RET_OK;
        if(affairing) {
            std::vector<std::string> arr{"vappend",cmds[1]};
            for(const auto &v:value)
            {
                arr.push_back(v);
            }
            g_back_stack.push(arr);
        }
        if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    }
    return "";
}

std::string GS_Vindx::name()
{
    return "vindx";
}

std::string GS_Vindx::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 3) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::vector<int> res = g_store->cmd_vindx(cmds[1],cmds[2]);
    std::string msg;
    if(res.empty()) {
        head.m_ret_code = RET_OK;
        return std::to_string(-1);
    }else{
        if(res[0] == -1){
            head.m_ret_code = RET_NO_KEY;
            return "";
        }else{
            head.m_ret_code = RET_OK;
            for (const auto &v : res)
            {
                msg = msg + std::to_string(v) + '\n';
            }
            msg.erase(msg.length() -1);
        }
    }
    return msg;
}

std::string GS_Verase::name()
{
    return "verase";
}

std::string GS_Verase::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if (cmds.size() != 3)
    {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    int indx;
    try
    {
        indx = std::stoi(cmds[2]);
    }
    catch(const std::invalid_argument& e)
    {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::string value;
    if(affairing) value = g_store->cmd_vget(cmds[1],indx);
    switch (g_store->cmd_verase(cmds[1],indx))
    {
    case DB_OP_INDX_OVERFLOW:
    {
        head.m_ret_code = RET_OK;
        return g_ret_str[RET_INDX_OVERFLOW_STR];
    }
    case DB_OP_NO_KEY:
    {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    break;
    case DB_OP_OK:
    {
        head.m_ret_code = RET_OK;
        if(affairing) g_back_stack.push({"vinsert",cmds[1],cmds[2],value});
        if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    }
    break;
    }
    return "1";
}

std::string GS_Vinsert::name()
{
    return "vinsert";
}

std::string GS_Vinsert::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 4) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    int indx;
    try
    {
        indx = std::stoi(cmds[2]);
    }
    catch(const std::invalid_argument& e)
    {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    switch (g_store->cmd_vinsert(cmds[1],cmds[3],indx))
    {
    case DB_OP_INDX_OVERFLOW:
    {
        head.m_ret_code = RET_OK;
        return g_ret_str[RET_INDX_OVERFLOW_STR];
    }
        break;
    case DB_OP_NO_KEY:
        head.m_ret_code = RET_NO_KEY;
        break;
    case DB_OP_OK:
    {
        head.m_ret_code = RET_OK; 
        if(affairing) g_back_stack.push({"verase",cmds[1],cmds[2]});
        if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    }
        break;
    default:
        break;
    };
    return "";
}

std::string GS_Vexist::name()
{
    return "vexist";
}

std::string GS_Vexist::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2 && cmds.size() != 3) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    head.m_ret_code = RET_OK;
    if(cmds.size() == 2) {
        if(g_store->cmd_vexist(cmds[1])) return g_ret_str[RET_TRUE_STR];
    }else{
        if(g_store->cmd_vexist(cmds[1],cmds[2])) return g_ret_str[RET_TRUE_STR];
    }
    return g_ret_str[RET_FALSE_STR];
}

std::string GS_Vappend::name()
{
    return "vappend";
}

std::string GS_Vappend::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() < 3) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    int begidx = g_store->cmd_vsize(cmds[1]) < 0 ? 0 : g_store->cmd_vsize(cmds[1]);
    for(int i = 2 ; i < cmds.size() ; i++)
    {
        if(g_store->cmd_vappend(cmds[1],cmds[i]) == DB_OP_NO_KEY) {
            head.m_ret_code = RET_NO_KEY;
            return "";
        }
    }
    if (affairing)
    {
        for (int i = 2; i < cmds.size(); i++)
        {
            g_back_stack.push({"verase", cmds[1], std::to_string(begidx)});
        }
    }
    head.m_ret_code = RET_OK;
    if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    return "";
}

std::string GS_Vsize::name()
{
    return "vsize";
}

std::string GS_Vsize::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    int ret = g_store->cmd_vsize(cmds[1]);
    if( ret == DB_OP_NO_KEY) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    head.m_ret_code = RET_OK;
    return std::to_string(ret);
}

std::string GS_Vexpire::name()
{
    return "vexpire";
}

std::string GS_Vexpire::exec(galay::Scheduler_Base::wptr scheduler, GS_Proto_Head &head, std::vector<std::string> &&cmds,bool affairing)
{
    if(cmds.size() != 3) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(affairing) {
        head.m_ret_code = RET_ERR_CMD;
        return "";
    }
    int seconds;
    try
    {
        seconds = std::stoi(cmds[2]);
    }
    catch(const std::invalid_argument& e)
    {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(!g_store->cmd_vexist(cmds[1])) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    auto timer = scheduler.lock()->get_timer_manager()->add_timer(seconds * 1000 , 1 , [cmds](){
        g_store->cmd_vdelete(cmds[1]);
    });
    g_store->set_timer(VECTOR_STORE,cmds[1],timer);
    head.m_ret_code = RET_OK;
    return "";
}

std::string GS_Vttl::name()
{
    return "vttl";
}
std::string GS_Vttl::exec(galay::Scheduler_Base::wptr scheduler, GS_Proto_Head &head, std::vector<std::string> &&cmds,bool affairing)
{
    if(cmds.size() != 2){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(!g_store->cmd_vexist(cmds[1])) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    auto timer = g_store->get_timer(VECTOR_STORE,cmds[1]);
    if(timer == nullptr){
        head.m_ret_code = RET_OK;
        return "-1";
    }
    return std::to_string((timer->get_expired_time() - galay::Timer::get_current_time()) / 1000);

}

std::string GS_Lget::name()
{
    return "lget";
}

std::string GS_Lget::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::string msg;
    auto l = g_store->cmd_lget(cmds[1]);
    if(l.empty()){
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    for(const auto& item: l){
        msg = msg + item + '\n';
    }
    head.m_ret_code = RET_OK;
    msg.erase(msg.length()-1);
    return msg;
}

std::string GS_Ldelete::name()
{
    return "ldelete";
}

std::string GS_Ldelete::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::list<std::string> value;
    if(affairing) value = g_store->cmd_lget(cmds[1]);
    if(g_store->cmd_ldelete(cmds[1])==DB_OP_NO_KEY) head.m_ret_code = RET_NO_KEY;
    else {
        head.m_ret_code = RET_OK;
        if(affairing){
            std::vector<std::string> arr = {"rpush", cmds[1]};
            for (const auto &v : value)
            {
                arr.push_back(v);
            }
            g_back_stack.push(arr);
        }
        if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    }
    return "";
}

std::string GS_Lerase::name()
{
    return "lerase";
}

std::string GS_Lerase::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() < 3){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    int num = 0;
    std::list<std::string> value;
    if(affairing) value = g_store->cmd_lget(cmds[1]);
    for(int i = 2 ; i < cmds.size() ; i++)
    {
        int ret = g_store->cmd_lerase(cmds[1],cmds[i]);
        if(ret == DB_OP_NO_KEY){
            head.m_ret_code = RET_NO_KEY;
            return "";
        }else{
            num += ret;
        }
    }
    head.m_ret_code = RET_OK;
    if(affairing){
        g_back_stack.push({"ldelete",cmds[1]});
        std::vector<std::string> arr = {"rpush",cmds[1]};
        for(const auto& v: value)
        {
            arr.push_back(v);
        }
        g_back_stack.push(arr);
    }
    if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    return std::to_string(num);
}

std::string GS_Lexist::name()
{
    return "lexist";
}

std::string GS_Lexist::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2 && cmds.size() != 3){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    head.m_ret_code = RET_OK;
    if(cmds.size() == 2){
        if(g_store->cmd_lexist(cmds[1])) return g_ret_str[RET_TRUE_STR];
    }else{
        if(g_store->cmd_lexist(cmds[1],cmds[2])) return g_ret_str[RET_TRUE_STR];
    }
    return g_ret_str[RET_FALSE_STR];
}

std::string GS_Lpush::name()
{
    return "lpush";
}

std::string GS_Lpush::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() < 3){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::list<std::string> value;
    if(affairing) value = g_store->cmd_lget(cmds[1]);
    std::string msg;
    for(int i = 2 ; i < cmds.size() ; i ++)
    {
        g_store->cmd_lpush(cmds[1],cmds[i]); 
    }
    head.m_ret_code = RET_OK;
    if(affairing){
        g_back_stack.push({"ldelete",cmds[1]});
        std::vector<std::string> arr = {"rpush",cmds[1]};
        for(const auto& v: value)
        {
            arr.push_back(v);
        }
        g_back_stack.push(arr);
    }
    if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    return "";
}

std::string GS_Rpush::name()
{
    return "rpush";
}

std::string GS_Rpush::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() < 3){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::list<std::string> value;
    if(affairing) value = g_store->cmd_lget(cmds[1]);
    std::string msg;
    for(int i = 2 ; i < cmds.size() ; i ++)
    {
        g_store->cmd_rpush(cmds[1],cmds[i]); 
    }
    if(affairing){
        g_back_stack.push({"ldelete",cmds[1]});
        std::vector<std::string> arr = {"rpush",cmds[1]};
        for(const auto& v: value)
        {
            arr.push_back(v);
        }
        g_back_stack.push(arr);
    }
    head.m_ret_code = RET_OK;
    if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    return "";
}

std::string GS_Lsize::name()
{
    return "lsize";
}

std::string GS_Lsize::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    int ret = g_store->cmd_lsize(cmds[1]);
    if(ret == DB_OP_NO_KEY) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    head.m_ret_code = RET_OK;
    return std::to_string(ret);
}

std::string GS_Lexpire::name()
{
    return "lexpire";
}

std::string GS_Lexpire::exec(galay::Scheduler_Base::wptr scheduler, GS_Proto_Head &head, std::vector<std::string> &&cmds,bool affairing)
{
    if(cmds.size() != 3) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    int seconds;
    try
    {
        seconds = std::stoi(cmds[2]);
    }
    catch(const std::invalid_argument& e)
    {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(!g_store->cmd_lexist(cmds[1])) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    auto timer = scheduler.lock()->get_timer_manager()->add_timer(seconds * 1000 , 1 , [cmds](){
        g_store->cmd_ldelete(cmds[1]);
    });
    g_store->set_timer(LIST_STORE,cmds[1],timer);
    head.m_ret_code = RET_OK;
    return "";
}

std::string GS_Lttl::name()
{
    return "lttl";
}
std::string GS_Lttl::exec(galay::Scheduler_Base::wptr scheduler, GS_Proto_Head &head, std::vector<std::string> &&cmds,bool affairing)
{
    if(cmds.size() != 2){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(affairing) {
        head.m_ret_code = RET_ERR_CMD;
        return "";
    }
    if(!g_store->cmd_lexist(cmds[1])) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    auto timer = g_store->get_timer(LIST_STORE,cmds[1]);
    if(timer == nullptr){
        head.m_ret_code = RET_OK;
        return "-1";
    }
    return std::to_string((timer->get_expired_time() - galay::Timer::get_current_time()) / 1000);

}

std::string GS_Rbget::name()
{
    return "rbget";
}

std::string GS_Rbget::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() < 3){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::string res;
    for(int i = 2 ; i < cmds.size() ; i++){
        int ret;
        std::string temp = g_store->cmd_rbget(cmds[1],cmds[i],ret);
        if(ret == DB_OP_NO_KEY) {
            head.m_ret_code = RET_NO_KEY;
            return "";
        }
        res = res + cmds[i]  + ": " + temp + '\n';
    }
    head.m_ret_code = RET_OK;
    res.erase(res.length()-1);
    return res;
}

std::string GS_Rbdelete::name()
{
    return "rbdelete";
}

std::string GS_Rbdelete::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::map<std::string,std::string> value;
    if(affairing) value = g_store->cmd_rbget(cmds[1]);
    if(g_store->cmd_rbdelete(cmds[1]) == DB_OP_NO_KEY) head.m_ret_code = RET_NO_KEY;
    else {
        head.m_ret_code = RET_OK;
        if(affairing){
            std::vector<std::string> arr = {"rbappend",cmds[1]};
            for(const auto&[k,v]:value)
            {
                arr.push_back(k);
                arr.push_back(v);
            }
            g_back_stack.push(arr);
        }
        if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    }
    return "";
}

std::string GS_Rberase::name()
{
    return "rberase";
}

std::string GS_Rberase::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() < 3) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    int num = 0;
    for(int i = 2 ; i < cmds.size() ; i ++)
    {
        int temp = g_store->cmd_rberase(cmds[1],cmds[i]);
        if(temp == DB_OP_NO_KEY) {
            head.m_ret_code = RET_NO_KEY;
            return "";
        }else if (temp == DB_OP_OK){
            if(affairing) {
                int ret;
                std::string value = g_store->cmd_rbget(cmds[1],cmds[i],ret);
                g_back_stack.push({"rbappend",cmds[1],cmds[i],value});
            }
            num ++;
        }
    }
    head.m_ret_code = RET_OK;
    if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    return std::to_string(num);
}

std::string GS_Rbexist::name()
{
    return "rbexist";
}

std::string GS_Rbexist::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2 && cmds.size() != 3){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    head.m_ret_code = RET_OK;
    if(cmds.size() == 2){
        if(g_store->cmd_rbexist(cmds[1])) return g_ret_str[RET_TRUE_STR];
    }else{
        if(g_store->cmd_rbexist(cmds[1],cmds[2])) return g_ret_str[RET_TRUE_STR];
    }
    return g_ret_str[RET_FALSE_STR];
}

std::string GS_Rbappend::name()
{
    return "rbappend";
}

std::string GS_Rbappend::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() < 3 || cmds.size() % 2 != 0 ) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    for(int i = 2 ; i < cmds.size() ; i+=2)
    {
        g_store->cmd_rbappend(cmds[1],cmds[i],cmds[i+1]);
        if(affairing) {
            g_back_stack.push({"rberase",cmds[1],cmds[i]});
        }
    }
    head.m_ret_code = RET_OK;
    if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    return "";
}

std::string GS_Rbsize::name()
{
    return "rbsize";
}

std::string GS_Rbsize::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    int ret = g_store->cmd_rbsize(cmds[1]);
    if(ret == DB_OP_NO_KEY) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    head.m_ret_code = RET_OK;
    return std::to_string(ret);
}

std::string GS_Rbkeys::name()
{
    return "rbkeys";
}

std::string GS_Rbkeys::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::vector<std::string> keys = g_store->cmd_rbkeys(cmds[1]);
    if(keys.empty()) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    head.m_ret_code = RET_OK;
    std::string msg;
    for(const auto& key: keys){
        msg = msg + key + '\n';
    }
    msg.erase(msg.length() - 1);
    return msg;
}

std::string GS_Rbexpire::name()
{
    return "rbexpire";
}

std::string GS_Rbexpire::exec(galay::Scheduler_Base::wptr scheduler, GS_Proto_Head &head, std::vector<std::string> &&cmds,bool affairing)
{
    if(cmds.size() != 3) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(affairing) {
        head.m_ret_code = RET_ERR_CMD;
        return "";
    }
    int seconds;
    try
    {
        seconds = std::stoi(cmds[2]);
    }
    catch(const std::invalid_argument& e)
    {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(!g_store->cmd_rbexist(cmds[1])) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    auto timer = scheduler.lock()->get_timer_manager()->add_timer(seconds * 1000 , 1 , [cmds](){
        g_store->cmd_rbdelete(cmds[1]);
    });
    g_store->set_timer(RBTREE_STORE,cmds[1],timer);
    head.m_ret_code = RET_OK;
    return "";
}

std::string GS_Rbttl::name()
{
    return "rbttl";
}
std::string GS_Rbttl::exec(galay::Scheduler_Base::wptr scheduler, GS_Proto_Head &head, std::vector<std::string> &&cmds,bool affairing)
{
    if(cmds.size() != 2){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(!g_store->cmd_rbexist(cmds[1])) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    auto timer = g_store->get_timer(RBTREE_STORE,cmds[1]);
    if(timer == nullptr){
        head.m_ret_code = RET_OK;
        return "-1";
    }
    return std::to_string((timer->get_expired_time() - galay::Timer::get_current_time()) / 1000);

}

std::string GS_Hget::name()
{
    return "hget";
}

std::string GS_Hget::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::string msg;
    std::unordered_set<std::string> values = g_store->cmd_hget(cmds[1]);
    if(values.empty()) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    for(const auto& value: values){
        msg = msg + value + '\n';
    }
    msg.erase(msg.length() - 1);
    return msg;
}

std::string GS_Hdelete::name()
{
    return "hdelete";
}

std::string GS_Hdelete::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    std::unordered_set<std::string> value;
    if(affairing) value = g_store->cmd_hget(cmds[1]);
    if(g_store->cmd_hdelete(cmds[1]) == DB_OP_NO_KEY) head.m_ret_code = RET_NO_KEY;
    else {
        head.m_ret_code = RET_OK;
        if(affairing){
            std::vector<std::string> arr = {"happend",cmds[1]};
            for(const auto& v: value)
            {
                arr.push_back(v);
            }
            g_back_stack.push(arr);
        }
        if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    }
    return "";
}

std::string GS_Herase::name()
{
    return "herase";
}

std::string GS_Herase::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() < 3) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    int num = 0 ;
    for(int i = 2 ; i < cmds.size() ; i ++)
    {
        int ret = g_store->cmd_herase(cmds[1],cmds[i]);
        if(ret == DB_OP_NO_KEY) {
            head.m_ret_code = RET_NO_KEY;
            return "";
        }else if(ret == DB_OP_OK){
            num ++;
            if(affairing) {
                g_back_stack.push({"happend",cmds[1],cmds[i]});
            }
        }
    }
    head.m_ret_code = RET_OK;
    if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    return std::to_string(num);
}

std::string GS_Hexist::name()
{
    return "hexist";
}

std::string GS_Hexist::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2 && cmds.size() != 3) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    head.m_ret_code = RET_OK;
    if(cmds.size() == 2){
        if(g_store->cmd_hexist(cmds[1])) return g_ret_str[RET_TRUE_STR];
    }else{
        if(g_store->cmd_hexist(cmds[1],cmds[2])) return g_ret_str[RET_TRUE_STR];
    }
    return g_ret_str[RET_FALSE_STR];
}

std::string GS_Happend::name()
{
    return "happend";
}

std::string GS_Happend::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() < 3){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    for(int i = 2 ; i < cmds.size() ; i ++){
        if(g_store->cmd_happend(cmds[1],cmds[i]) == DB_OP_NO_KEY) {
            head.m_ret_code = RET_NO_KEY;
            return "";
        }else{
            if(affairing){
                g_back_stack.push({"herase",cmds[1],cmds[i]});
            }
        }
    }
    head.m_ret_code = RET_OK;
    if(g_aof_on) g_aof_queue->push(std::forward<std::vector<std::string>&&>(cmds));
    return "";
}

std::string GS_Hsize::name()
{
    return "hsize";
}

std::string GS_Hsize::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(cmds.size() != 2) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    int ret = g_store->cmd_hsize(cmds[1]);
    if(ret == DB_OP_NO_KEY) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    head.m_ret_code = RET_OK;
    return std::to_string(ret);
}

std::string GS_Hexpire::name()
{
    return "hexpire";
}

std::string GS_Hexpire::exec(galay::Scheduler_Base::wptr scheduler, GS_Proto_Head &head, std::vector<std::string> &&cmds,bool affairing)
{
    if(cmds.size() != 3) {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(affairing) {
        head.m_ret_code = RET_ERR_CMD;
        return "";
    }
    int seconds;
    try
    {
        seconds = std::stoi(cmds[2]);
    }
    catch(const std::invalid_argument& e)
    {
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(!g_store->cmd_hexist(cmds[1])) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    auto timer = scheduler.lock()->get_timer_manager()->add_timer(seconds * 1000 , 1 , [cmds](){
        g_store->cmd_hdelete(cmds[1]);
    });
    g_store->set_timer(HASH_STORE,cmds[1],timer);
    head.m_ret_code = RET_OK;
    return "";
}

std::string GS_Httl::name()
{
    return "httl";
}
std::string GS_Httl::exec(galay::Scheduler_Base::wptr scheduler, GS_Proto_Head &head, std::vector<std::string> &&cmds,bool affairing)
{
    if(cmds.size() != 2){
        head.m_ret_code = RET_INVAILD_ARG;
        return "";
    }
    if(!g_store->cmd_hexist(cmds[1])) {
        head.m_ret_code = RET_NO_KEY;
        return "";
    }
    auto timer = g_store->get_timer(HASH_STORE,cmds[1]);
    if(timer == nullptr){
        head.m_ret_code = RET_OK;
        return "-1";
    }
    return std::to_string((timer->get_expired_time() - galay::Timer::get_current_time()) / 1000);

}

std::string GS_Affair::name()
{
    return "affair";
}

std::string GS_Affair::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(!GS_Executor::affairing()) {
        while(!g_back_stack.empty()){
            g_back_stack.pop();
        }
        head.m_ret_code = RET_OK;
        GS_Executor::affairing() = true;
    }
    else head.m_ret_code = RET_FAIL;
    return "";
}

std::string GS_Commit::name()
{
    return "commit";
}

std::string GS_Commit::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if (GS_Executor::affairing())
    {
        GS_Executor::affairing() = false;
        while (!g_affair_queue.empty())
        {
            auto affair = g_affair_queue.front();
            g_affair_queue.pop();
            g_executor->execute(scheduler, head, std::move(affair),affairing);
            if (head.m_ret_code != RET_OK) {
                head.m_ret_code = RET_FAIL;
                while (!g_affair_queue.empty())
                {
                    g_affair_queue.pop();
                }
                return "";
            }
        }
        head.m_ret_code = RET_OK;
    }else head.m_ret_code = RET_ERR_CMD;
    return "";
}

std::string GS_Rollback::name()
{
    return "rollback";
}

std::string GS_Rollback::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    while(!g_back_stack.empty())
    {
        auto back_cmd = g_back_stack.top();
        g_back_stack.pop();
        g_executor->execute(scheduler,head,std::move(back_cmd));
    }
    head.m_ret_code = RET_OK;
    return "";
}

std::string GS_Discard::name()
{
    return "discard";
}

std::string GS_Discard::exec(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    while(!g_affair_queue.empty())
    {
        g_affair_queue.pop();
    }
    while(!g_back_stack.empty())
    {
        g_back_stack.pop();
    }
    GS_Executor::affairing() = false;
    head.m_ret_code = RET_OK;
    return "";
}

bool GS_Executor::m_affair = false;

GS_Executor::GS_Executor()
{
    m_cmds["keys"] = std::make_unique<GS_Keys>();
    m_cmds["affair"] = std::make_unique<GS_Affair>();
    m_cmds["commit"] = std::make_unique<GS_Commit>();
    m_cmds["rollback"] = std::make_unique<GS_Rollback>();
    m_cmds["discard"] = std::make_unique<GS_Discard>();
    m_cmds["set"] = std::make_unique<GS_Set>();
    m_cmds["get"] = std::make_unique<GS_Get>();
    m_cmds["delete"] = std::make_unique<GS_Delete>();
    m_cmds["exist"] = std::make_unique<GS_Exist>();
    m_cmds["len"] = std::make_unique<GS_Len>();
    m_cmds["expire"] = std::make_unique<GS_Expire>();
    m_cmds["ttl"] = std::make_unique<GS_TTL>();
    m_cmds["vget"] = std::make_unique<GS_Vget>();
    m_cmds["vdelete"] = std::make_unique<GS_Vdelete>();
    m_cmds["vexist"] = std::make_unique<GS_Vexist>();
    m_cmds["vindx"] = std::make_unique<GS_Vindx>();
    m_cmds["verase"] = std::make_unique<GS_Verase>();
    m_cmds["vinsert"] = std::make_unique<GS_Vinsert>();
    m_cmds["vappend"] = std::make_unique<GS_Vappend>();
    m_cmds["vsize"] = std::make_unique<GS_Vsize>();
    m_cmds["vexpire"] = std::make_unique<GS_Vexpire>();
    m_cmds["vttl"] = std::make_unique<GS_Vttl>();
    m_cmds["lget"] = std::make_unique<GS_Lget>();
    m_cmds["ldelete"] = std::make_unique<GS_Ldelete>();
    m_cmds["lerase"] = std::make_unique<GS_Lerase>();
    m_cmds["lexist"] = std::make_unique<GS_Lexist>();
    m_cmds["lpush"] = std::make_unique<GS_Lpush>();
    m_cmds["rpush"] = std::make_unique<GS_Rpush>();
    m_cmds["lsize"] = std::make_unique<GS_Lsize>();
    m_cmds["lexpire"] = std::make_unique<GS_Lexpire>();
    m_cmds["lttl"] = std::make_unique<GS_Lttl>();
    m_cmds["rbget"] = std::make_unique<GS_Rbget>();
    m_cmds["rbdelete"] = std::make_unique<GS_Rbdelete>();
    m_cmds["rberase"] = std::make_unique<GS_Rberase>();
    m_cmds["rbexist"] = std::make_unique<GS_Rbexist>();
    m_cmds["rbappend"] = std::make_unique<GS_Rbappend>();
    m_cmds["rbsize"] = std::make_unique<GS_Rbsize>();
    m_cmds["rbkeys"] = std::make_unique<GS_Rbkeys>();
    m_cmds["rbexpire"] = std::make_unique<GS_Rbexpire>();
    m_cmds["rbttl"] = std::make_unique<GS_Rbttl>();
    m_cmds["hget"] = std::make_unique<GS_Hget>();
    m_cmds["hdelete"] = std::make_unique<GS_Hdelete>();
    m_cmds["happend"] = std::make_unique<GS_Happend>();
    m_cmds["herase"] = std::make_unique<GS_Herase>();
    m_cmds["hexist"] = std::make_unique<GS_Hexist>();
    m_cmds["hsize"] = std::make_unique<GS_Hsize>();
    m_cmds["hexpire"] = std::make_unique<GS_Hexpire>();
    m_cmds["httl"] = std::make_unique<GS_Httl>();
}

std::string GS_Executor::execute(galay::Scheduler_Base::wptr scheduler,GS_Proto_Head& head, std::vector<std::string>&& cmds,bool affairing)
{
    if(GS_Executor::affairing()){
        if(cmds[0].compare("commit") == 0) return m_cmds[cmds[0]]->exec(scheduler,head,std::move(cmds),true);
        else if(cmds[0].compare("discard") == 0) return m_cmds[cmds[0]]->exec(scheduler,head,std::move(cmds),false);
        else{
            g_affair_queue.push(std::move(cmds));
            head.m_ret_code = RET_OK;
            return g_ret_str[RET_QUEUED_STR];
        }
    }
    auto it = m_cmds.find(cmds[0]);
    if(it == m_cmds.end()) {
        head.m_ret_code = RET_ERR_CMD;
        return "";
    }
    return it->second->exec(scheduler,head,std::forward<std::vector<std::string>&&>(cmds),affairing);
}

bool& GS_Executor::affairing()
{
    return GS_Executor::m_affair;
}

GS_Executor::~GS_Executor()
{
    m_cmds.clear();
}
