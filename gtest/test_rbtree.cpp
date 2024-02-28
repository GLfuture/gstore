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

TEST(Rbappend,Normal)
{
    
    ASSERT_TRUE(test_func("rbappend bonus manager gong clerk lu",RET_OK,""));
}

TEST(Rbappend,Non_Normal)
{
    
    ASSERT_TRUE(test_func("rbappend bonus st",RET_INVAILD_ARG,""));
    ASSERT_TRUE(test_func("rbappendd bonus name st",RET_ERR_CMD,""));
}

TEST(Rbget, Normal)
{
    
    ASSERT_TRUE(test_func("rbget bonus manager st",RET_OK,"manager: gong\nst: "));
}

TEST(Rbget, Non_Normal)
{
    
    ASSERT_TRUE(test_func("rbget bonus",RET_INVAILD_ARG,""));
    ASSERT_TRUE(test_func("rbbget bonus manager",RET_ERR_CMD,""));
    ASSERT_TRUE(test_func("rbget name gong",RET_NO_KEY,""));
}

TEST(Rbexist,Normal)
{
    
    ASSERT_TRUE(test_func("rbexist bonus",RET_OK,"True"));
    ASSERT_TRUE(test_func("rbexist bonus manager",RET_OK,"True"));
    ASSERT_TRUE(test_func("rbexist name",RET_OK,"False"));
    ASSERT_TRUE(test_func("rbexist bonus cto",RET_OK,"False"));
}

TEST(Rbexist,Non_Normal)
{
    
    ASSERT_TRUE(test_func("rbexist bonus manager gong",RET_INVAILD_ARG,""));
    ASSERT_TRUE(test_func("rbexistt bonus",RET_ERR_CMD,""));
}

TEST(Rbsize,Normal)
{
    
    ASSERT_TRUE(test_func("rbsize bonus",RET_OK,"2"));
}

TEST(Rbsize,Non_Normal)
{
    
    ASSERT_TRUE(test_func("rbsize txt",RET_NO_KEY,""));
    ASSERT_TRUE(test_func("rbsizew bonus",RET_ERR_CMD,""));
    ASSERT_TRUE(test_func("rbsize bonus x",RET_INVAILD_ARG,""));
}

TEST(Rbkeys,Normal)
{
    ASSERT_TRUE(test_func("rbkeys bonus",RET_OK,"clerk\nmanager"));
}

TEST(Rbkeys,Non_Normal)
{
    ASSERT_TRUE(test_func("rbkeys",RET_INVAILD_ARG,""));
    ASSERT_TRUE(test_func("rbkeyss bonus",RET_ERR_CMD,""));
    ASSERT_TRUE(test_func("rbkeys name",RET_NO_KEY,""));
}

TEST(Rberase,Normal)
{
    ASSERT_TRUE(test_func("rberase bonus manager clerk",RET_OK,"2"));
    ASSERT_TRUE(test_func("rbexist bonus",RET_OK,"False"));
}

TEST(Rberase,Non_Normal)
{
    ASSERT_TRUE(test_func("rberase name manager clerk",RET_NO_KEY,""));
    ASSERT_TRUE(test_func("rberasee name manager",RET_ERR_CMD,""));
    ASSERT_TRUE(test_func("rberase name",RET_INVAILD_ARG,""));
}


TEST(Rbdelete,Normal)
{
    ASSERT_TRUE(test_func("rbappend date monday 17:00",RET_OK,""));
    ASSERT_TRUE(test_func("rbget date monday",RET_OK,"monday: 17:00"));
    ASSERT_TRUE(test_func("rbdelete date",RET_OK,""));
}

TEST(Rbdelete,Non_Normal)
{
    ASSERT_TRUE(test_func("rbdelete date",RET_NO_KEY,""));
    ASSERT_TRUE(test_func("rbdeletee name",RET_ERR_CMD,""));
    ASSERT_TRUE(test_func("rbdelete name manager",RET_INVAILD_ARG,""));
}
