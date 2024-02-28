#include <gtest/gtest.h>
#include "../common/gscmds.h"
#include "../server/cmdparser.h"
#include "../server/cmdexecutor.h"
#include "../server/init.h"

auto scheduler = galay::Scheduler_Factory::create_epoll_scheduler(1024,5);

bool test_func(std::string req_cmd, int expect_retcode, std::string expect_retbody)
{
    GS_Proto_Response expect;
    expect.get_head().m_ret_code = expect_retcode;
    expect.get_head().m_version = VERSION1_0;
    expect.get_head().m_length = expect_retbody.length();
    expect.get_body() = expect_retbody;

    GS_Proto_Response resp;
    resp.get_head().m_version = VERSION1_0;
    std::string body = g_executor->execute(scheduler , resp.get_head(), GS_Parser::parse(req_cmd));
    resp.get_head().m_length = body.length();
    resp.get_body() = std::move(body);
    return expect.encode().compare(resp.encode()) == 0;
}

TEST(Get, Normal)
{
    
    ASSERT_TRUE(test_func("rpush name gong lu king", RET_OK, ""));
    ASSERT_TRUE(test_func("lget name", RET_OK, "gong\nlu\nking"));
}

TEST(Get, Non_Normal)
{
    
    ASSERT_TRUE(test_func("lget name pth", RET_INVAILD_ARG, ""));
    ASSERT_TRUE(test_func("llget name", RET_ERR_CMD, ""));
    ASSERT_TRUE(test_func("lget teacher", RET_NO_KEY, ""));
}

TEST(Erase,Normal)
{
    
    ASSERT_TRUE(test_func("lpush name gong",RET_OK,""));
    ASSERT_TRUE(test_func("lerase name gong lu",RET_OK,"3"));
    ASSERT_TRUE(test_func("lget name",RET_OK,"king"));
}



TEST(Erase,Non_Normal)
{
    
    ASSERT_TRUE(test_func("learse name king",RET_ERR_CMD,""));
    ASSERT_TRUE(test_func("lerase teacher st",RET_NO_KEY,""));
    ASSERT_TRUE(test_func("lerase name",RET_INVAILD_ARG,""));
}

TEST(Exist,Normal)
{
    
    ASSERT_TRUE(test_func("lexist name",RET_OK,"True"));
    ASSERT_TRUE(test_func("lexist name king",RET_OK,"True"));
    ASSERT_TRUE(test_func("lexist teacher",RET_OK,"False"));
}

TEST(Exist,Non_Normal)
{
    
    ASSERT_TRUE(test_func("lexist name gong si",RET_INVAILD_ARG,""));
    ASSERT_TRUE(test_func("lexists name",RET_ERR_CMD,""));
}

TEST(Lpush,Normal)
{
    
    ASSERT_TRUE(test_func("lpush name gong lu",RET_OK,""));
    ASSERT_TRUE(test_func("lget name",RET_OK,"lu\ngong\nking"));
}

TEST(Lpush,Non_Normal)
{
    
    ASSERT_TRUE(test_func("lpush teacher gong",RET_OK,""));
    ASSERT_TRUE(test_func("lpush teacher",RET_INVAILD_ARG,""));
    ASSERT_TRUE(test_func("lpushh name s",RET_ERR_CMD,""));
}

TEST(Rpush,Normal)
{
    
    ASSERT_TRUE(test_func("rpush name gong lu",RET_OK,""));
    ASSERT_TRUE(test_func("lget name",RET_OK,"lu\ngong\nking\ngong\nlu"));
}

TEST(Rpush,Non_Normal)
{
    
    ASSERT_TRUE(test_func("rpush teacher tt",RET_OK,""));
    ASSERT_TRUE(test_func("rpushh name gong",RET_ERR_CMD,""));
    ASSERT_TRUE(test_func("rpush name",RET_INVAILD_ARG,""));

}

TEST(Lsize,Normal)
{
    
    ASSERT_TRUE(test_func("lsize name",RET_OK,"5"));
}

TEST(Lsize,Non_Normal)
{
    
    ASSERT_TRUE(test_func("lsize leader",RET_NO_KEY,""));
    ASSERT_TRUE(test_func("lsizee name gong",RET_ERR_CMD,""));
    ASSERT_TRUE(test_func("lsize name t",RET_INVAILD_ARG,""));
}

TEST(Delete,Normal)
{
    
    ASSERT_TRUE(test_func("ldelete name",RET_OK,""));
    ASSERT_TRUE(test_func("lget name",RET_NO_KEY,""));
}

TEST(Delete,Non_Normal)
{
    
    ASSERT_TRUE(test_func("ldelete leader",RET_NO_KEY,""));
    ASSERT_TRUE(test_func("ldeletee name",RET_ERR_CMD,""));
    ASSERT_TRUE(test_func("ldelete name gong",RET_INVAILD_ARG,""));
}

