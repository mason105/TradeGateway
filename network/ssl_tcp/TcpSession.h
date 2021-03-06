#ifndef _TCP_SESSION_
#define _TCP_SESSION_




#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/pool/object_pool.hpp>

#include "Network/ssl_tcp/MsgHeader.h"
//#include "network/CustomMessage.h"
#include "ThreadSafeQueue/job_queue.h"




#include "network/ssl_tcp/TcpSession.h"
#include "business/IBusiness.h"


/*
session的释放可以参考
http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/ssl/server.cpp

http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/http/server2/connection.hpp
*/

class IMessage;

class TcpSession : public boost::enable_shared_from_this<TcpSession>
{
public:
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::asio::io_service ios_type;

	typedef ios_type::strand strand_type;
	typedef job_queue<IMessage*> queue_type;

	//内存池，暂时不用
	//typedef boost::object_pool<CustomMessage*> object_pool_type;

private:
	socket_type socket_;
	strand_type strand_;
	queue_type& queue_;

	//static object_pool_type msg_pool_;


public:
	TcpSession( ios_type& ios, queue_type& q, int msgType);
	~TcpSession();

	socket_type& socket();
	ios_type& io_service();

	virtual void start();
	virtual void close();
	


	virtual IMessage* create_request();
	virtual void read();
	virtual void handle_read_head(const boost::system::error_code& error, size_t bytes_transferred, IMessage* req);
	virtual void handle_read_msg(const boost::system::error_code& error, size_t bytes_transferred, IMessage* req);


	virtual void write(IMessage* resp);
	virtual void handle_write_head(const boost::system::error_code& error, size_t bytes_transferred, IMessage* resp);
	virtual void handle_write_msg(const boost::system::error_code& error, size_t bytes_transferred, IMessage* resp);

public:

	// 是否登录状态
	int loginStatus;

	

	// 建立线程，发送心跳消息，调用业务连接的心跳自定义实现方法，保持活动

	// 柜台连接
	IBusiness * counterT2;
	IBusiness * counterSzkingdom;
	IBusiness * counterApex;
	IBusiness * counterAGC;
	IBusiness * counterXinYi;

	// 消息类型
	int m_msgType;

	
	// 关闭柜台连接
	void CloseCounterConnect();
	// 得到柜台连接
	IBusiness * GetCounterConnect(int counterType);
	
};


typedef boost::shared_ptr<TcpSession> TcpSessionPtr;

#endif // _TCP_SESSION_

