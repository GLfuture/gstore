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
    std::string body = g_executor->execute(scheduler, resp.get_head(), GS_Parser::parse(req_cmd));
    resp.get_head().m_length = body.length();
    resp.get_body() = std::move(body);
    return expect.encode().compare(resp.encode()) == 0;
}

TEST(Get, Normal)
{
    
    ASSERT_TRUE(test_func("vappend name gong lu king darren", RET_OK, ""));
    ASSERT_TRUE(test_func("vget name", RET_OK, "gong\nlu\nking\ndarren"));
}

TEST(Get, Non_Normal)
{
    
    ASSERT_TRUE(test_func("vget teacher", RET_NO_KEY, ""));
    ASSERT_TRUE(test_func("vgeet name", RET_ERR_CMD, ""));
    ASSERT_TRUE(test_func("vget name king", RET_INVAILD_ARG, ""));
}

TEST(Exist, Normal)
{
    
    ASSERT_TRUE(test_func("vexist name", RET_OK, "True"));
    ASSERT_TRUE(test_func("vexist name int", RET_OK, "False"));
    ASSERT_TRUE(test_func("vexist name gong", RET_OK, "True"));
    ASSERT_TRUE(test_func("vexist teacher", RET_OK, "False"));
}

TEST(Exist, Non_Normal)
{
    
    ASSERT_TRUE(test_func("vexist name int ad", RET_INVAILD_ARG, ""));
    ASSERT_TRUE(test_func("vexistt name", RET_ERR_CMD, ""));
}

TEST(Append, Normal)
{
    
    ASSERT_TRUE(test_func("vappend name jiji tt gong", RET_OK, ""));
    ASSERT_TRUE(test_func("vget name", RET_OK, "gong\nlu\nking\ndarren\njiji\ntt\ngong"));
}

TEST(Append, Non_Normal)
{
    
    ASSERT_TRUE(test_func("vappend name", RET_INVAILD_ARG, ""));
    ASSERT_TRUE(test_func("vappendd name t", RET_ERR_CMD, ""));
}

TEST(Size, Normal)
{
    
    ASSERT_TRUE(test_func("vsize name", RET_OK, "7"));
}

TEST(Size, Non_Normal)
{
    
    ASSERT_TRUE(test_func("vsize name a", RET_INVAILD_ARG, ""));
    ASSERT_TRUE(test_func("vsize teacher", RET_NO_KEY, ""));
    ASSERT_TRUE(test_func("vsizee name", RET_ERR_CMD, ""));
}

TEST(Indx, Normal)
{
    
    ASSERT_TRUE(test_func("vindx name gong",RET_OK,"0\n6"));
    ASSERT_TRUE(test_func("vindx name fff",RET_OK,"-1"));
}

TEST(Indx, Non_Normal)
{
    
    ASSERT_TRUE(test_func("vindx name",RET_INVAILD_ARG,""));
    ASSERT_TRUE(test_func("vindx teacher gong",RET_NO_KEY,""));
    ASSERT_TRUE(test_func("vindxx name",RET_ERR_CMD,""));
}

TEST(Erase, Normal)
{
    
    ASSERT_TRUE(test_func("verase name 6",RET_OK,"1"));
    ASSERT_TRUE(test_func("vappend demo co",RET_OK,""));
    ASSERT_TRUE(test_func("verase demo 0",RET_OK,"1"));
    ASSERT_TRUE(test_func("vexist demo",RET_OK,"False"));
}

TEST(Erase, Non_Normal)
{
    
    ASSERT_TRUE(test_func("verase name 11",RET_OK,g_ret_str[RET_INDX_OVERFLOW_STR]));
    ASSERT_TRUE(test_func("verase teacher 10",RET_NO_KEY,""));
    ASSERT_TRUE(test_func("verase name gong",RET_INVAILD_ARG,""));
    ASSERT_TRUE(test_func("verasee name 0",RET_ERR_CMD,""));
}


TEST(Delete, Normal)
{
    
    ASSERT_TRUE(test_func("vdelete name", RET_OK, ""));
    ASSERT_TRUE(test_func("vdelete teacher", RET_NO_KEY, ""));
}

TEST(Delete, Non_Normal)
{
    
    ASSERT_TRUE(test_func("vdelete teacher king", RET_INVAILD_ARG, ""));
    ASSERT_TRUE(test_func("vdelet name", RET_ERR_CMD, ""));
}