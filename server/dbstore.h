#ifndef GS_DB_STORE_H
#define GS_DB_STORE_H
#include <galay/kernel/timer.h>
#include <vector>
#include <string>  
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>

enum OP_RES
{
    DB_OP_OK = -100,
    DB_OP_NO_KEY,
    DB_OP_NO_VALUE,
    DB_OP_INDX_OVERFLOW,
};

enum Store_Type
{
    STRING_STORE,
    VECTOR_STORE,
    LIST_STORE,
    RBTREE_STORE,
    HASH_STORE,
    STORE_SIZE
};

extern const char* g_store_name[];

struct Item{
    virtual ~Item()
    {
        if(m_timer) {
            m_timer->cancle();
            m_timer = nullptr;
        }
    }
    galay::Timer::ptr m_timer = nullptr;
};

struct String_Item : public Item
{
    std::string m_item = "";
};

struct Vector_Item : public Item
{
    std::vector<std::string> m_item = {};
};

struct List_Item : public Item
{
    std::list<std::string> m_item = {};
};

struct Rbtree_Item : public Item
{
    std::map<std::string, std::string> m_item = {};
};

struct Hash_Item : public Item
{
    std::unordered_set<std::string> m_item = {};
};

class DB_Store
{
public:
    using uptr = std::unique_ptr<DB_Store>;

    //返回所有key
    std::vector<std::vector<std::string>> cmd_keys();
    //get key's timer
    galay::Timer::ptr get_timer(Store_Type type, std::string key);
    int set_timer(Store_Type type, std::string key , galay::Timer::ptr timer);

    //string
    //return OK
    int cmd_set(std::string key,std::string value);
    //if key is exist return value else return ""
    std::string cmd_get(std::string key);
    //return OK or No Key
    int cmd_delete(std::string key);
    //return true or false
    bool cmd_exist(std::string key);
    //return OK or No key
    int cmd_append(std::string key,std::string value);
    //return no key or length
    int cmd_len(std::string key);

    //vector
    //if key is exist return vector else return {}
    std::vector<std::string> cmd_vget(std::string key);
    std::string cmd_vget(std::string key,int indx);
    //return OK or No key
    int cmd_vdelete(std::string key);
    //if both key and value are exist return all values' index  else return {} 
    std::vector<int> cmd_vindx(std::string key,std::string value);
    //if indx > vector's size return DB_OP_INDX_OVERFLOW or return no key 
    int cmd_verase(std::string key,int indx);
    int cmd_vinsert(std::string key,std::string value, int indx);
    bool cmd_vexist(std::string key,std::string value = "");
    int cmd_vappend(std::string key,std::string value);
    //return no key or size
    int cmd_vsize(std::string key);

    //list
    std::list<std::string> cmd_lget(std::string key);
    int cmd_ldelete(std::string key);
    int cmd_lerase(std::string key,std::string value);
    bool cmd_lexist(std::string key,std::string value = "");
    int cmd_lpush(std::string key,std::string value);
    int cmd_rpush(std::string key,std::string value);
    //return no key or size
    int cmd_lsize(std::string key);

    //rbtree
    std::vector<std::string> cmd_rbkeys(std::string key);
    std::map<std::string,std::string> cmd_rbget(std::string key);
    std::string cmd_rbget(std::string key,std::string key1,int &ret);
    int cmd_rbdelete(std::string key);
    int cmd_rberase(std::string key,std::string key1);
    bool cmd_rbexist(std::string key,std::string key1="");
    int cmd_rbappend(std::string key,std::string key1,std::string value);
    int cmd_rbsize(std::string key);

    //hash
    std::unordered_set<std::string> cmd_hget(std::string key);
    int cmd_hdelete(std::string key);
    int cmd_herase(std::string key,std::string value);
    bool cmd_hexist(std::string key,std::string value = "");
    int cmd_happend(std::string key,std::string value);
    int cmd_hsize(std::string key);

    ~DB_Store()
    {
        m_stringstore.clear();
        m_vectorstore.clear();
        m_liststore.clear();
        m_rbtreestore.clear();
        m_setstore.clear();
    }

private:
    std::unordered_map<std::string,String_Item> m_stringstore;
    std::unordered_map<std::string,Vector_Item> m_vectorstore;
    std::unordered_map<std::string,List_Item> m_liststore;
    std::unordered_map<std::string,Rbtree_Item> m_rbtreestore;
    std::unordered_map<std::string,Hash_Item> m_setstore;
};



#endif