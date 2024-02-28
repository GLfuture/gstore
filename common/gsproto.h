#ifndef  GSTORE_PROTOCOL_H
#define	 GSTORE_PROTOCOL_H

#include <galay/protocol/basic_protocol.h>
#include <arpa/inet.h>
#include <string.h>

#define VERSION1_0     10

#pragma pack(1)

struct GS_Proto_Head
{
    uint16_t m_version;
    uint16_t m_ret_code;
    uint16_t m_length;
};

#pragma pack()

class GS_Proto_Request : public galay::Tcp_Request_Base,public galay::Tcp_Response_Base
{
public:
    using ptr = std::shared_ptr<GS_Proto_Request>;
    //for server
    virtual int proto_type() override;

    virtual int proto_fixed_len() override;

    virtual int decode(const std::string& buffer, int& state) override;

    virtual int proto_extra_len() override;

    virtual void set_extra_msg(std::string&& msg) override;

    //for client
    virtual std::string encode() override;
    

    GS_Proto_Head& get_head();

    std::string& get_body();

    virtual ~GS_Proto_Request();
protected:
    GS_Proto_Head m_head;
    std::string m_body;
};

class GS_Proto_Response: public GS_Proto_Request
{
public:
    using ptr = std::shared_ptr<GS_Proto_Response>;

    virtual int decode(const std::string& buffer,int &state) override;
};



#endif // ! GSTORE_PROTOCOL_H
