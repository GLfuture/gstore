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

TEST(Happend,Normal)
{
    ASSERT_TRUE(test_func("happend date monday tuesday wednesday thursday friday saturday sunday",RET_OK,""));
}

TEST(Happend,Non_Normal)
{
    ASSERT_TRUE(test_func("happend date",RET_INVAILD_ARG,""));
    ASSERT_TRUE(test_func("happendd date yesterday",RET_ERR_CMD,""));
}

TEST(Herase,Normal)
{
    ASSERT_TRUE(test_func("herase date monday tuesday",RET_OK,"2"));
}

TEST(Herase,Non_Normal)
{
    ASSERT_TRUE(test_func("herase date",RET_INVAILD_ARG,""));
    ASSERT_TRUE(test_func("herase name yesterday",RET_NO_KEY,""));
    ASSERT_TRUE(test_func("herasee date sunday",RET_ERR_CMD,""));
}

TEST(Hexist,Normal)
{
    ASSERT_TRUE(test_func("hexist date",RET_OK,"True"));
    ASSERT_TRUE(test_func("hexist date sunday",RET_OK,"True"));
    ASSERT_TRUE(test_func("hexist date monday",RET_OK,"False"));
    ASSERT_TRUE(test_func("hexist name",RET_OK,"False"));
}

TEST(Hexist,Non_Normal)
{
    ASSERT_TRUE(test_func("hexist date name t",RET_INVAILD_ARG,""));
    ASSERT_TRUE(test_func("hexistt date",RET_ERR_CMD,""));
}

TEST(Hsize,Normal)
{
    ASSERT_TRUE(test_func("hsize date",RET_OK,"5"));
}

TEST(Hsize,Non_Normal)
{
    ASSERT_TRUE(test_func("hsize name",RET_NO_KEY,""));
    ASSERT_TRUE(test_func("hsizee date",RET_ERR_CMD,""));
    ASSERT_TRUE(test_func("hsize date monday",RET_INVAILD_ARG,""));
}

TEST(Hdelete,Normal)
{
    ASSERT_TRUE(test_func("hdelete date",RET_OK,""));
    ASSERT_TRUE(test_func("hexist date",RET_OK,"False"));
}

TEST(Hdelete,Non_Normal)
{
    ASSERT_TRUE(test_func("hdelete data",RET_NO_KEY,""));
    ASSERT_TRUE(test_func("hdeletee date",RET_ERR_CMD,""));
    ASSERT_TRUE(test_func("hdelete date st",RET_INVAILD_ARG,""));

}