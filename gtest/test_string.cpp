#include <gtest/gtest.h>
#include <galay/factory/factory.h>
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
  std::string body = g_executor->execute(scheduler,resp.get_head(), GS_Parser::parse(req_cmd));
  resp.get_head().m_length = body.length();
  resp.get_body() = std::move(body);
  return expect.encode().compare(resp.encode()) == 0;
}

TEST(SetAndGet, Normal)
{
  ASSERT_TRUE(test_func("set name gong", RET_OK, ""));
  ASSERT_TRUE(test_func("get name", RET_OK, "gong"));
  ASSERT_TRUE(test_func("set name king", RET_OK, ""));
  ASSERT_TRUE(test_func("get name", RET_OK, "king"));
}

TEST(SetAndGet, Non_Normal)
{
  ASSERT_TRUE(test_func("sett a func", RET_ERR_CMD, ""));
  ASSERT_TRUE(test_func("set test gc st", RET_INVAILD_ARG, ""));
  ASSERT_TRUE(test_func("get teacher", RET_NO_KEY, ""));
  ASSERT_TRUE(test_func("geet name", RET_ERR_CMD, ""));
  ASSERT_TRUE(test_func("get name king", RET_INVAILD_ARG, ""));
}

TEST(Exist, Normal)
{
  ASSERT_TRUE(test_func("exist name", RET_OK, "True"));
  ASSERT_TRUE(test_func("exist teacher", RET_OK, "False"));
}

TEST(Exist, Non_Normal)
{
  ASSERT_TRUE(test_func("exist name int", RET_INVAILD_ARG, ""));
  ASSERT_TRUE(test_func("existt name", RET_ERR_CMD, ""));
}

TEST(LEN, Normal)
{
  ASSERT_TRUE(test_func("len name", RET_OK, "4"));
}

TEST(LEN, Non_Normal)
{
  ASSERT_TRUE(test_func("len name a", RET_INVAILD_ARG, ""));
  ASSERT_TRUE(test_func("len teacher", RET_NO_KEY, ""));
  ASSERT_TRUE(test_func("lenn name", RET_ERR_CMD, ""));
}

TEST(Delete, Normal)
{
  ASSERT_TRUE(test_func("delete name", RET_OK, ""));
  ASSERT_TRUE(test_func("delete teacher", RET_NO_KEY, ""));
}

TEST(Delete, Non_Normal)
{
  ASSERT_TRUE(test_func("delete teacher king", RET_INVAILD_ARG, ""));
  ASSERT_TRUE(test_func("delet name", RET_ERR_CMD, ""));
}