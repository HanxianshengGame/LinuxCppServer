// Authors: Handling
// Created by 15870 on 2020/7/15.
// This is 

#ifndef LINUXCPPSERVER_LINUXCPPSERVER_BOOSTASIO_STUDY_H_
#define LINUXCPPSERVER_LINUXCPPSERVER_BOOSTASIO_STUDY_H_
#include <iostream>
#include <deque>
#include <set>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp> //稳定计时库

// asio 基本学习，时间C/S程序
namespace asiobasicstudy {

//1. 使用asio同步编程与异步编程简单例子
//使用IOServices对deadline_timer/steady_timer同步操作测试（阻塞式）
//用steady_timer 直接向 IO_services注册事件并立即执行，（非阻塞式）
void IOServicesSyncTest();

//使用IOServices对deadline_timer/steady_timer异步步操作测试
/* 使用deadline_timer向IO中注册事件
 * 当io_serivec run时，会同时对其进行处理,处理完毕调用回调函数
 *
 * 注意点：一定要在run前，保证回调函数参数方面的存活。
 */



void CallBackEvent1(const boost::system::error_code &);
void CallBackEvent2(const boost::system::error_code &);
void RepeatCallBackEvent(const boost::system::error_code &, boost::asio::steady_timer &t, int &count);
void IOServicesAsyncTest1();
void IOServicesAsyncTest2();

//封装timer异步操作例子
//线程同步 srand
class Printer {
 public:
  Printer(boost::asio::io_service &io) :
      strand_(io), timer1_(io, std::chrono::seconds(1)), timer2_(io, std::chrono::seconds(1)), count_(0) {
    timer1_.async_wait(strand_.wrap([&](const boost::system::error_code &error_code) {
      if (error_code != boost::asio::error::operation_aborted)this->Print1();
    }));
    timer2_.async_wait(strand_.wrap([&](const boost::system::error_code &error_code) {
      if (error_code != boost::asio::error::operation_aborted)this->Print2();
    }));
  }
  ~Printer() {}
  void Print1();
  void Print2();

 private:
  boost::asio::io_service::strand strand_;
  boost::asio::steady_timer timer1_;
  boost::asio::steady_timer timer2_;
  int count_;
};

//多个线程同时跑异步操作注册的事件
void MultiThreadAsyncOperator();

//一个同步TCP daytime 客户端
//argc: make时传入的信息数量，argv是表示信息，第一个文件名是 文件名，其余自己添加
int SyncTcpDaytimeClient();
//一个同步TCP daytime 服务器
std::string MakeDaytimeString();
int SyncTcpDaytimeServer();

class TCPConnection : std::enable_shared_from_this<TCPConnection> {
  using tcp = boost::asio::ip::tcp;
 public:
  using pointer = std::shared_ptr<TCPConnection>;
  static pointer Create(boost::asio::io_service &io);

  tcp::socket &socket() { return socket_; }
  void start();

 private:
  explicit TCPConnection(boost::asio::io_service &io) : socket_(io) {}
  void handle_write(const boost::system::error_code &error, std::size_t bytes_transferred);
  tcp::socket socket_;
  std::string message_;
};
class TCPServer {
  using tcp = boost::asio::ip::tcp;

 public:

  TCPServer(boost::asio::io_service &io) : io_service_(io), acceptor_(io_service_, tcp::endpoint(tcp::v4(), 13)) {
    start_accpect();
  }

 private:

  void start_accpect();
  void handle_accpect(TCPConnection::pointer pointer, const boost::system::error_code &e);
  boost::asio::io_service &io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
};

//一个异步TCP daytime 服务器
int AsyncTcpDaytimeServer();

}


//可回调信息的 C/S 程序： C:block sync   S：async
namespace echoclientserver {

// 用于描述服务器与客户端之间的会话并为服务器提供额外的功能
class Session : public std::enable_shared_from_this<Session> {
  using tcp = boost::asio::ip::tcp;
 public:
  Session(tcp::socket socket) : socket_(std::move(socket)) {} //注意，在构造函数如果去生成自身，是违法行为的，尤其是生成其动态内存
  void Start() { do_read(); }

 private:
  enum { max_length = 1024 };

  void do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self](const boost::system::error_code &error, std::size_t len) {
                              if (!error) {
                                (std::cout << "receive a message :").write(data_, len) << std::endl;
                                do_write(len);
                              }
                            });
  }
  void do_write(std::size_t len) {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, len),
                             [this, self](const boost::system::error_code &error, std::size_t len) {
                               std::cout << "write success" << std::endl;
                               if (!error)
                                 do_read();
                               else
                                 std::cerr << error.message() << std::endl;
                             });

  }

  tcp::socket socket_;
  char data_[max_length];
};

 //可回调消息的服务器
class EchoServer {
  using tcp = boost::asio::ip::tcp;
 public:
  EchoServer(boost::asio::io_service &io_service, int port): acceptor_(io_service, tcp::endpoint(tcp::v4(), port)){ }
  void Start() { do_accpect(); }

 private:
  void do_accpect() {
    acceptor_.async_accept([this](const boost::system::error_code &error, tcp::socket socket) {
      if(!error) {
         std::make_shared<Session>(std::move(socket))->Start();
      }
      std::cout << "one client accpect! " << std::endl;
      do_accpect();
    });
  }
  tcp::acceptor  acceptor_;
};
int EchoClient(); // Echo客户端

void StartServer();


}

// 聊天室 C/S 程序
namespace chatclientserver {
class ChatMessage {
 public:
  enum { HEADER_LENGTH = 4 };
  enum { MAX_BODY_LENGTH = 512};
  ChatMessage(): body_length_(0) { }

  const char* data() const { return data_; }
  char* data()  { return data_; }
  std::size_t length() const { return HEADER_LENGTH + body_length_; }
  const char *body() const { return data_ + HEADER_LENGTH;  }

  char *body() { return data_ + HEADER_LENGTH; }

  std::size_t body_length() const { return body_length_; }

  bool DecodeHeader();
  void EncodeHeader();
  void set_body_length(std::size_t new_length) {
    body_length_ = new_length > MAX_BODY_LENGTH ? MAX_BODY_LENGTH : new_length;
  }
 private:
  char data_[HEADER_LENGTH + MAX_BODY_LENGTH]{};
  std::size_t body_length_;
};

using boost::asio::ip::tcp;
using ChatMessageQue = std::deque<ChatMessage>;

class ChatParticipant {
 public:
  virtual ~ChatParticipant() = default;
  virtual void Deliver(const ChatMessage &msg) = 0;
};

using ChatParticipantPtr = std::shared_ptr<ChatParticipant>;



//聊天室
/* 1. 添加参与者
 * 2. 参与者断开连接后移除参与者
 * 3. 向室内成员派发消息
 */
class ChatRoom {
 public:
  void Join(ChatParticipantPtr participant);
  void Leave(ChatParticipantPtr participant);
  void Deliver(const ChatMessage &msg);
 private:

  std::set<ChatParticipantPtr> participants_;
  enum { MAX_RECENT_MSGS = 100 };
  ChatMessageQue recent_msgs_;
};

//聊天会话：管理客户端在聊天室内的消息通信
/* 1. 将当前会话的客户端加入到聊天室内
 * 2. 开始接收客户端的消息
 * 3. 将客户端的消息分发到聊天室的其他客户端
 * 4. 将聊天室的其他客户端消息分发给当前会话的客户端
 */
class ChatSession: public ChatParticipant , public std::enable_shared_from_this<ChatSession> {
 private:
  void do_write();
  void do_read_body();
  void do_read_header();
  int id_;
  tcp::socket socket_;
  ChatRoom &room_;
  ChatMessage read_msg_;
  ChatMessageQue write_msgs_;
 public:
  ChatSession(int id, tcp::socket socket, ChatRoom &room) : id_(id), socket_(std::move(socket)), room_(room) {}
  void Start() {
    room_.Join(shared_from_this());
    std::cout<< "You have joined Han Zhenjiang's chat room!" << std::endl;
    do_read_header();
  }

  void Deliver(const ChatMessage &msg)  override {
    bool write_in_process = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if(!write_in_process)
      do_write();
  }
};


//聊天服务器：接收客户端的连接并创建 ChatSession 管理客户端的行为
class ChatServer {
 private:
  void do_accept() {
    acceptor_.async_accept([this](const boost::system::error_code &ec, tcp::socket socket) {
      if(!ec) {
        std::cout << "a client accpet! \n";
        std::make_shared<ChatSession>(++client_id_, std::move(socket), room_)->Start();
      }
      do_accept();
    });
  }
  int client_id_ = 0;
  tcp::acceptor acceptor_;
  ChatRoom room_;

 public:
  ChatServer(boost::asio::io_service &io_service, const tcp::endpoint &endpoint): acceptor_(io_service, endpoint){}
  void Start() { do_accept(); }
};

//开始服务
int StartServer();

//聊天客户端
class ChatClient {

 private:
  void do_connect(const tcp::resolver::results_type &end_points);
  void do_read_header();
  void do_read_body();
  void do_write();
  boost::asio::io_service &io_service_;
  tcp::socket socket_;
  ChatMessage read_msg_;
  ChatMessageQue write_msgs_;

 public:

  ChatClient(boost::asio::io_service &io_service, const tcp::resolver::results_type &end_points): io_service_(io_service), socket_(io_service) { }
  void Write(const ChatMessage &msg);
  void Close() { boost::asio::post(io_service_,[this](){ socket_.close(); }); }

};

int StartClient();
}

// 使用 cstruct 实现的消息协议的  聊天室 C/S 程序
namespace chatclientserver_version1 {

namespace  cstructheader {
/*
 *
 * 注意：考虑好字节对齐，免去其空间上多余分配
 */

struct Header {
  int body_size_;
  int type_;
};
enum MessageType {
  MT_BIND_NAME = 1,
  MT_CHAT_INFO = 2,
  MT_ROOM_INFO = 3
};
struct BindName {  //type：  1
  char name_[32];
  int name_len_;
};

struct ChatInformation {  // type: 0
  char information_[256];
  int  info_len_;
};

//server send
struct RoomInformation {
  BindName bind_name_;
  ChatInformation chat_info_;
};

//cmd MessageBody: 对客户端输入的消息进行正确校验与格式化
bool ParseMessagge(const std::string &input, int *type, std::string *outbuffer);

}

// c-s s-c  message { header, body }
class ChatMessage {

 public:
  enum { HEADER_LENGTH = sizeof(cstructheader::Header) };
  enum { MAX_BODY_LENGTH = 512};
  ChatMessage()  = default;

  const char* data() const { return data_; }
  char* data()  { return data_; }
  std::size_t length() const { return HEADER_LENGTH + m_header_.body_size_; }

  const char *body() const { return data_ + HEADER_LENGTH;  }
  char *body() { return data_ + HEADER_LENGTH; }

  int type() const { return m_header_.type_; }

  std::size_t body_length() const { return m_header_.body_size_; }

  bool DecodeHeader();


  void set_message(int message_type, const void *buffer, size_t buffer_size) {
    assert(buffer_size <= MAX_BODY_LENGTH);
    m_header_.body_size_ = buffer_size;
    m_header_.type_ = message_type;
    std::memcpy(body(), buffer, buffer_size);
    std::memcpy(data(), &m_header_, sizeof(m_header_));
  }
 private:

  char data_[HEADER_LENGTH + MAX_BODY_LENGTH]{};
  cstructheader::Header m_header_{};
};

using boost::asio::ip::tcp;
using ChatMessageQue = std::deque<ChatMessage>;

class ChatParticipant {
 public:
  virtual ~ChatParticipant() = default;
  virtual void Deliver(const ChatMessage &msg) = 0;
};

using ChatParticipantPtr = std::shared_ptr<ChatParticipant>;



//聊天室
/* 1. 添加参与者
 * 2. 参与者断开连接后移除参与者
 * 3. 向室内成员派发消息
 */
class ChatRoom {
 public:
  void Join(ChatParticipantPtr participant);
  void Leave(ChatParticipantPtr participant);
  void Deliver(const ChatMessage &msg);
 private:

  std::set<ChatParticipantPtr> participants_;
  enum { MAX_RECENT_MSGS = 100 };
  ChatMessageQue recent_msgs_;
};

//聊天会话：管理客户端在聊天室内的消息通信
/* 1. 将当前会话的客户端加入到聊天室内
 * 2. 开始接收客户端的消息
 * 3. 将客户端的消息分发到聊天室的其他客户端
 * 4. 将聊天室的其他客户端消息分发给当前会话的客户端
 */
class ChatSession: public ChatParticipant , public std::enable_shared_from_this<ChatSession> {
 private:
  void do_write();
  void do_read_body();
  void do_read_header();
  void handling_message();
  cstructheader::RoomInformation build_room_info() const ;

  std::string client_name_;

  std::string client_msg_;

  tcp::socket socket_;
  ChatRoom &room_;
  ChatMessage read_msg_;
  ChatMessageQue write_msgs_;
 public:
  ChatSession(tcp::socket socket, ChatRoom &room) : socket_(std::move(socket)), room_(room) {}
  void Start() {
    room_.Join(shared_from_this());
    std::cout<< "You have joined Han Zhenjiang's chat_info_ room!" << std::endl;
    do_read_header();
  }

  void Deliver(const ChatMessage &msg)  override {
    bool write_in_process = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if(!write_in_process)
      do_write();
  }
};


//聊天服务器：接收客户端的连接并创建 ChatSession 管理客户端的行为
class ChatServer {
 private:
  void do_accept() {
    acceptor_.async_accept([this](const boost::system::error_code &ec, tcp::socket socket) {
      if(!ec) {
        std::cout << "a client accpet! \n";
        std::make_shared<ChatSession>(std::move(socket), room_)->Start();
      }
      do_accept();
    });
  }
  tcp::acceptor acceptor_;
  ChatRoom room_;

 public:
  ChatServer(boost::asio::io_service &io_service, const tcp::endpoint &endpoint): acceptor_(io_service, endpoint){}
  void Start() { do_accept(); }
};

//开始服务
int StartServer();

//聊天客户端
class ChatClient {

 private:
  void do_connect(const tcp::resolver::results_type &end_points);
  void do_read_header();
  void do_read_body();
  void do_write();
  boost::asio::io_service &io_service_;
  tcp::socket socket_;
  ChatMessage read_msg_;
  ChatMessageQue write_msgs_;

 public:

  ChatClient(boost::asio::io_service &io_service, const tcp::resolver::results_type &end_points): io_service_(io_service), socket_(io_service) {
    do_connect(end_points);
  }
  void Write(const ChatMessage &msg);
  void Close() { boost::asio::post(io_service_,[this](){ socket_.close(); }); }

};

int StartClient();

}

#endif //LINUXCPPSERVER_LINUXCPPSERVER_BOOSTASIO_STUDY_H_
