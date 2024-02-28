#include "gsproto.h"
#include <iostream>
#include "galay/kernel/error.h"

int GS_Proto_Request::proto_type()
{
    return galay::GY_HEAD_FIXED_PROTOCOL_TYPE;
}

int GS_Proto_Request::proto_fixed_len()
{
    return sizeof(GS_Proto_Head);
}

int GS_Proto_Request::decode(const std::string& buffer, int& state)
{
    state = galay::error::GY_SUCCESS;
    const char *temp = buffer.c_str();
    memcpy(&this->m_head.m_version, temp , 2);
    memcpy(&this->m_head.m_ret_code , temp+2 ,2);
    memcpy(&this->m_head.m_length, temp+4, 2);
    this->m_head.m_version = ntohs(m_head.m_version);
    this->m_head.m_ret_code = ntohs(m_head.m_ret_code);
    this->m_head.m_length = ntohs(m_head.m_length);
    return sizeof(GS_Proto_Head);
}

int GS_Proto_Request::proto_extra_len()
{
    return m_head.m_length;
}

void GS_Proto_Request::set_extra_msg(std::string&& msg)
{
    this->m_body = msg;
}

std::string GS_Proto_Request::encode()
{
    char buffer[sizeof(GS_Proto_Head)] = { 0 };
    GS_Proto_Head head;
    head.m_version = htons(m_head.m_version);
    head.m_ret_code = htons(m_head.m_ret_code);
    head.m_length = htons(m_head.m_length);
    memcpy(buffer, &head.m_version, 2);
    memcpy(buffer+2,&head.m_ret_code,2);
    memcpy(buffer+4,&head.m_length,2);
    std::string res(buffer, sizeof(GS_Proto_Head));
    return res + this->m_body;
}

GS_Proto_Head& GS_Proto_Request::get_head()
{
	return this->m_head;
}

std::string& GS_Proto_Request::get_body()
{
	return this->m_body;
}

int GS_Proto_Response::decode(const std::string& buffer,int &state)
{
    state = galay::error::GY_SUCCESS;
    const char *temp = buffer.c_str();
    memcpy(&this->m_head.m_version, temp , 2);
    memcpy(&this->m_head.m_ret_code , temp+2 , 2);
    memcpy(&this->m_head.m_length, temp + 4, 2);
    this->m_head.m_version = ntohs(m_head.m_version);
    this->m_head.m_ret_code = ntohs(m_head.m_ret_code);
    this->m_head.m_length = ntohs(m_head.m_length);
    this->m_body = buffer.substr(sizeof(GS_Proto_Head),this->m_head.m_length);
    return sizeof(GS_Proto_Head) + this->m_head.m_length;
}

GS_Proto_Request::~GS_Proto_Request()
{

}