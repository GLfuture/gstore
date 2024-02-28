#include "dbstore.h"

const char *g_store_name[] = {
    "[String]",
    "[Vector]",
    "[List]",
    "[Rbtree]",
    "[Hash]",
};

std::vector<std::vector<std::string>> DB_Store::cmd_keys()
{
    std::vector<std::vector<std::string>> res(STORE_SIZE);
    for (const auto &[k, v] : m_stringstore)
    {
        res[STRING_STORE].push_back(k);
    }
    for (const auto &[k, v] : m_vectorstore)
    {
        res[VECTOR_STORE].push_back(k);
    }
    for (const auto &[k, v] : m_liststore)
    {
        res[LIST_STORE].push_back(k);
    }
    for (const auto &[k, v] : m_rbtreestore)
    {
        res[RBTREE_STORE].push_back(k);
    }
    for (const auto &[k, v] : m_setstore)
    {
        res[HASH_STORE].push_back(k);
    }
    return res;
}

galay::Timer::ptr DB_Store::get_timer(Store_Type type, std::string key)
{
    switch (type)
    {
    case STRING_STORE:
    {
        auto it = m_stringstore.find(key);
        if (it == m_stringstore.end())
            return nullptr;
        return it->second.m_timer;
    }
    break;
    case VECTOR_STORE:
    {
        auto it = m_vectorstore.find(key);
        if (it == m_vectorstore.end())
            return nullptr;
        return it->second.m_timer;
    }
    break;
    case LIST_STORE:
    {
        auto it = m_liststore.find(key);
        if (it == m_liststore.end())
            return nullptr;
        return it->second.m_timer;
    }
    break;
    case RBTREE_STORE:
    {
        auto it = m_rbtreestore.find(key);
        if (it == m_rbtreestore.end())
            return nullptr;
        return it->second.m_timer;
    }
    break;
    case HASH_STORE:
    {
        auto it = m_setstore.find(key);
        if (it == m_setstore.end())
            return nullptr;
        return it->second.m_timer;
    }
    break;
    }
    return nullptr;
}

int DB_Store::set_timer(Store_Type type, std::string key, galay::Timer::ptr timer)
{
    switch (type)
    {
    case STRING_STORE:
    {
        auto it = m_stringstore.find(key);
        if (it == m_stringstore.end())
            return DB_OP_NO_KEY;
        it->second.m_timer = timer;
    }
    break;
    case VECTOR_STORE:
    {
        auto it = m_vectorstore.find(key);
        if (it == m_vectorstore.end())
            return DB_OP_NO_KEY;
        it->second.m_timer = timer;
    }
    break;
    case LIST_STORE:
    {
        auto it = m_liststore.find(key);
        if (it == m_liststore.end())
            return DB_OP_NO_KEY;
        it->second.m_timer = timer;
    }
    break;
    case RBTREE_STORE:
    {
        auto it = m_rbtreestore.find(key);
        if (it == m_rbtreestore.end())
            return DB_OP_NO_KEY;
        it->second.m_timer = timer;
    }
    break;
    case HASH_STORE:
    {
        auto it = m_setstore.find(key);
        if (it == m_setstore.end())
            return DB_OP_NO_KEY;
        it->second.m_timer = timer;
    }
    break;
    }
    return DB_OP_OK;
}

// string
int DB_Store::cmd_set(std::string key, std::string value)
{
    auto it = m_stringstore.find(key);
    if (it == m_stringstore.end())
    {
        String_Item item;
        item.m_item = value;
        m_stringstore.emplace(key, std::move(item));
    }
    else
        it->second.m_item = value;
    return DB_OP_OK;
}

std::string DB_Store::cmd_get(std::string key)
{
    auto it = m_stringstore.find(key);
    if (it == m_stringstore.end())
        return "";
    return it->second.m_item;
}

// need to cancle timer
int DB_Store::cmd_delete(std::string key)
{
    auto it = m_stringstore.find(key);
    if (it == m_stringstore.end())
        return DB_OP_NO_KEY;
    if (it->second.m_timer)
    {
        it->second.m_timer->cancle();
        it->second.m_timer = nullptr;
    }
    m_stringstore.erase(it);
    return DB_OP_OK;
}

bool DB_Store::cmd_exist(std::string key)
{
    return m_stringstore.contains(key);
}

int DB_Store::cmd_append(std::string key, std::string value)
{
    auto it = m_stringstore.find(key);
    if (it == m_stringstore.end())
        return DB_OP_NO_KEY;
    it->second.m_item.append(value);
    return DB_OP_OK;
}

int DB_Store::cmd_len(std::string key)
{
    auto it = m_stringstore.find(key);
    if (it == m_stringstore.end())
        return DB_OP_NO_KEY;
    return it->second.m_item.length();
}

// vector
std::vector<std::string> DB_Store::cmd_vget(std::string key)
{
    auto it = m_vectorstore.find(key);
    if (it == m_vectorstore.end())
        return {};
    return it->second.m_item;
}

std::string DB_Store::cmd_vget(std::string key,int indx)
{
    auto it = m_vectorstore.find(key);
    if (it == m_vectorstore.end()) return "";
    if(indx > it->second.m_item.size()) return "";
    return it->second.m_item[indx];
}

int DB_Store::cmd_vdelete(std::string key)
{
    auto it = m_vectorstore.find(key);
    if (it == m_vectorstore.end())
        return DB_OP_NO_KEY;
    if (it->second.m_timer)
    {
        it->second.m_timer->cancle();
        it->second.m_timer = nullptr;
    }
    m_vectorstore.erase(it);
    return DB_OP_OK;
}

int DB_Store::cmd_vinsert(std::string key,std::string value, int indx)
{
    auto it = m_vectorstore.find(key);
    if (it == m_vectorstore.end()) return DB_OP_NO_KEY;
    if(it->second.m_item.begin() + indx > it->second.m_item.end()) return DB_OP_INDX_OVERFLOW;
    it->second.m_item.insert(it->second.m_item.begin() + indx , value);
    return DB_OP_OK;
}

std::vector<int> DB_Store::cmd_vindx(std::string key, std::string value)
{
    auto it = m_vectorstore.find(key);
    if (it == m_vectorstore.end())
        return {-1};
    std::vector<int> res;
    for (int i = 0; i < it->second.m_item.size(); i++)
    {
        if (it->second.m_item[i].compare(value) == 0)
        {
            res.push_back(i);
        }
    }
    return res;
}

int DB_Store::cmd_verase(std::string key, int indx)
{
    auto it = m_vectorstore.find(key);
    if (it == m_vectorstore.end())
        return DB_OP_NO_KEY;
    if (indx >= it->second.m_item.size() || indx < 0)
        return DB_OP_INDX_OVERFLOW;
    it->second.m_item.erase(it->second.m_item.begin() + indx);
    if (it->second.m_item.empty())
    {
        if (it->second.m_timer)
        {
            it->second.m_timer->cancle();
            it->second.m_timer = nullptr;
        }
        m_vectorstore.erase(it);
    }
    return DB_OP_OK;
}

bool DB_Store::cmd_vexist(std::string key, std::string value)
{
    if (value.empty())
    {
        return m_vectorstore.contains(key);
    }
    auto it = m_vectorstore.find(key);
    if (it == m_vectorstore.end())
        return false;
    return !(std::find(it->second.m_item.begin(), it->second.m_item.end(), value) == it->second.m_item.end());
}

int DB_Store::cmd_vappend(std::string key, std::string value)
{
    m_vectorstore[key].m_item.push_back(value);
    return DB_OP_OK;
}

int DB_Store::cmd_vsize(std::string key)
{
    auto it = m_vectorstore.find(key);
    if (it == m_vectorstore.end())
        return DB_OP_NO_KEY;
    return it->second.m_item.size();
}

// list

std::list<std::string> DB_Store::cmd_lget(std::string key)
{
    auto it = m_liststore.find(key);
    if (it == m_liststore.end())
        return {};
    return it->second.m_item;
}

int DB_Store::cmd_ldelete(std::string key)
{
    auto it = m_liststore.find(key);
    if (it == m_liststore.end())
        return DB_OP_NO_KEY;
    if (it->second.m_timer)
    {
        it->second.m_timer->cancle();
        it->second.m_timer = nullptr;
    }
    m_liststore.erase(it);
    return DB_OP_OK;
}

// return erase num
int DB_Store::cmd_lerase(std::string key, std::string value)
{
    auto it = m_liststore.find(key);
    if (it == m_liststore.end())
        return DB_OP_NO_KEY;
    int num = 0;
    for (auto lit = it->second.m_item.begin(); lit != it->second.m_item.end(); lit++)
    {
        if (lit->compare(value) == 0)
        {
            num++;
            lit = it->second.m_item.erase(lit);
            if (lit == it->second.m_item.end())
                break;
            lit--;
        }
    }
    if (it->second.m_item.empty())
    {
        if (it->second.m_timer)
        {
            it->second.m_timer->cancle();
            it->second.m_timer = nullptr;
        }
        m_liststore.erase(it);
    }
    return num;
}

bool DB_Store::cmd_lexist(std::string key, std::string value)
{
    if (value.empty())
        return m_liststore.contains(key);
    auto it = m_liststore.find(key);
    return !(std::find(it->second.m_item.begin(), it->second.m_item.end(), value) == it->second.m_item.end());
}

int DB_Store::cmd_lpush(std::string key, std::string value)
{
    m_liststore[key].m_item.push_front(value);
    return DB_OP_OK;
}

int DB_Store::cmd_rpush(std::string key, std::string value)
{
    m_liststore[key].m_item.push_back(value);
    return DB_OP_OK;
}

int DB_Store::cmd_lsize(std::string key)
{
    auto it = m_liststore.find(key);
    if (it == m_liststore.end())
        return DB_OP_NO_KEY;
    return it->second.m_item.size();
}

// rbtree
std::vector<std::string> DB_Store::cmd_rbkeys(std::string key)
{
    std::vector<std::string> res;
    auto it = m_rbtreestore.find(key);
    if (it == m_rbtreestore.end())
        return {};
    for (const auto &[k, v] : it->second.m_item)
    {
        res.push_back(k);
    }
    return res;
}

std::map<std::string,std::string> DB_Store::cmd_rbget(std::string key)
{
    auto it = m_rbtreestore.find(key);
    if (it == m_rbtreestore.end())  return {};
    return it->second.m_item;
}

std::string DB_Store::cmd_rbget(std::string key, std::string key1, int &ret)
{
    auto it = m_rbtreestore.find(key);
    if (it == m_rbtreestore.end())
    {
        ret = DB_OP_NO_KEY;
        return "";
    }
    ret = DB_OP_OK;
    auto lit = it->second.m_item.find(key1);
    if (lit == it->second.m_item.end())
        return "";
    return lit->second;
}

int DB_Store::cmd_rbdelete(std::string key)
{
    auto it = m_rbtreestore.find(key);
    if (it == m_rbtreestore.end())
        return DB_OP_NO_KEY;
    if (it->second.m_timer)
    {
        it->second.m_timer->cancle();
        it->second.m_timer = nullptr;
    }
    m_rbtreestore.erase(it);
    return DB_OP_OK;
}

int DB_Store::cmd_rberase(std::string key, std::string field)
{
    auto it = m_rbtreestore.find(key);
    if (it == m_rbtreestore.end())
        return DB_OP_NO_KEY;
    auto lit = it->second.m_item.find(field);
    if (lit == it->second.m_item.end())
        return DB_OP_NO_VALUE;
    it->second.m_item.erase(lit);
    if (it->second.m_item.empty())
    {
        if (it->second.m_timer)
        {
            it->second.m_timer->cancle();
            it->second.m_timer = nullptr;
        }
        m_rbtreestore.erase(it);
    }
    return DB_OP_OK;
}

bool DB_Store::cmd_rbexist(std::string key, std::string key1)
{
    if (key1.empty())
        return m_rbtreestore.contains(key);
    auto it = m_rbtreestore.find(key);
    if (it == m_rbtreestore.end())
        return false;
    return it->second.m_item.contains(key1);
}

int DB_Store::cmd_rbappend(std::string key, std::string key1, std::string value)
{
    m_rbtreestore[key].m_item[key1] = value;
    return DB_OP_OK;
}

int DB_Store::cmd_rbsize(std::string key)
{
    auto it = m_rbtreestore.find(key);
    if (it == m_rbtreestore.end())
        return DB_OP_NO_KEY;
    return it->second.m_item.size();
}

std::unordered_set<std::string> DB_Store::cmd_hget(std::string key)
{
    auto it = m_setstore.find(key);
    if (it == m_setstore.end())
        return {};
    return it->second.m_item;
}

int DB_Store::cmd_hdelete(std::string key)
{
    auto it = m_setstore.find(key);
    if (it == m_setstore.end())
        return DB_OP_NO_KEY;
    if (it->second.m_timer)
    {
        it->second.m_timer->cancle();
        it->second.m_timer = nullptr;
    }
    m_setstore.erase(it);
    return DB_OP_OK;
}

int DB_Store::cmd_herase(std::string key, std::string value)
{
    auto it = m_setstore.find(key);
    if (it == m_setstore.end())
        return DB_OP_NO_KEY;
    auto lit = it->second.m_item.find(value);
    if (lit == it->second.m_item.end())
        return DB_OP_NO_VALUE;
    it->second.m_item.erase(lit);
    if (it->second.m_item.empty())
    {
        if (it->second.m_timer)
        {
            it->second.m_timer->cancle();
            it->second.m_timer = nullptr;
        }
        m_setstore.erase(it);
    }
    return DB_OP_OK;
}

bool DB_Store::cmd_hexist(std::string key, std::string value)
{
    auto it = m_setstore.find(key);
    if (it == m_setstore.end())
        return false;
    if (value.empty())
    {
        return true;
    }
    return it->second.m_item.contains(value);
}

int DB_Store::cmd_happend(std::string key, std::string value)
{
    m_setstore[key].m_item.insert(value);
    return DB_OP_OK;
}

int DB_Store::cmd_hsize(std::string key)
{
    auto it = m_setstore.find(key);
    if (it == m_setstore.end())
        return DB_OP_NO_KEY;
    return it->second.m_item.size();
}