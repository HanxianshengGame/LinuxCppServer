// Authors: Handling
// Created by 15870 on 2020/7/15.
// This is 

#include "BoostAsio_Study.h"

#include <ctime>

#include <boost/array.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace asiobasicstudy {
class resolver;
void IOServicesSyncTest() {
  boost::asio::io_service io;
  // boost::asio::deadline_timer t(io, boost::posix_time::seconds(2)); //使用deadline
  boost::asio::steady_timer t(io);
  t.expires_from_now(std::chrono::seconds(3));

  t.wait();
  std::cout << "IO operator end!" << std::endl;
}
void IOServicesAsyncTest1() {
  boost::asio::io_service io;
  //向IO中注册事件
  //boost::asio::deadline_timer t1(io, boost::posix_time::seconds(2));
  boost::asio::steady_timer t1(io);
  t1.expires_from_now(std::chrono::seconds(3));
  t1.async_wait(CallBackEvent1);

  // boost::asio::deadline_timer t2(io, boost::posix_time::seconds(1));
  boost::asio::steady_timer t2(io);
  t2.expires_from_now(std::chrono::seconds(3));
  t2.async_wait(CallBackEvent2);

  std::cout << "call begin" << std::endl;
  io.run();
}
void CallBackEvent1(const boost::system::error_code &) {
  std::cout << "call end!" << std::endl;
}
void CallBackEvent2(const boost::system::error_code &) {
  std::cout << "second call but first end!" << std::endl;
}
void IOServicesAsyncTest2() {
  boost::asio::io_service io;

  int count = 0;
  boost::asio::steady_timer t(io); //设置注册事件的I/O

  t.expires_from_now(std::chrono::seconds(1));
  t.async_wait([&t, &count](const boost::system::error_code &error_code) {
    RepeatCallBackEvent(error_code,
                        t,
                        count);
  });
  t.cancel();
  io.run();
  std::cout << "call end!";

}
void RepeatCallBackEvent(const boost::system::error_code &error, boost::asio::steady_timer &t, int &count) {
  if (error == boost::asio::error::operation_aborted)  //预防其 timer失效
    return;
  if (count < 5) {
    std::cout << count << std::endl;
    ++count;
    t.expires_from_now(std::chrono::seconds(1));
    t.async_wait([&t, &count](const boost::system::error_code &error_code) {
      RepeatCallBackEvent(error_code,
                          t,
                          count);
    });
  }
}
void MultiThreadAsyncOperator() {
  boost::asio::io_service io;
  std::cout << "run begin!" << std::endl;
  std::thread t([&] { io.run(); });
  Printer printer(io);  //在t线程结束时 io.state状态码已经变化,变成了 strm::eofbit(已经在尾部)
  io.reset(); //重新设置。。但这貌似跟跑一个线程没什么区别，哈哈
  io.run();
  t.join();
}

int SyncTcpDaytimeClient() {
  try {
    boost::asio::io_service io;
    using boost::asio::ip::tcp;
    tcp::resolver resolver(io);  //解析
    //将主机号解析为ip地址，daytime解析为其上的应用端口，返回其搜集到的所有集合，返回类型其实是vector,数据类型是<ip, endpoint>
    boost::system::error_code error_code;
    tcp::resolver::results_type end_points = resolver.resolve("192.168.1.175", "daytime");

    tcp::socket socket(io);
    boost::asio::connect(socket, end_points);
    for (;;) {

      std::array<char, 128> buf;
      size_t len = socket.read_some(boost::asio::buffer(buf), error_code);

      if (error_code == boost::asio::error::eof)
        break;
      else if (error_code)
        throw boost::system::error_code(error_code);

      std::cout.write(buf.data(), len);
    }

  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
int SyncTcpDaytimeServer() {
  try {
    using boost::asio::ip::tcp;
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));
    std::cout << "Server running!!!" << std::endl;
    for (;;) {
      tcp::socket socket1(io_service);
      acceptor.accept(socket1);

      std::string message = MakeDaytimeString();
      boost::system::error_code ignore_error;
      socket1.write_some(boost::asio::buffer(message), ignore_error);
      std::cout << "message already arrived" << std::endl;
    }
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
std::string MakeDaytimeString() {
  std::time_t now = std::time(0);
  return ctime(&now);
}

int AsyncTcpDaytimeServer() {
  try {
    boost::asio::io_service io;
    TCPServer tcp_server(io);
    io.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

void Printer::Print1() {
  if (count_ >= 5)
    return;
  std::cout << "timer1:" << ++count_ << std::endl;
  timer1_.expires_at(timer1_.expiry() + std::chrono::seconds(1));
  timer1_.async_wait(strand_.wrap([&](const boost::system::error_code &error_code) {
    if (error_code != boost::asio::error::operation_aborted) this->Print1();
  }));
}
void Printer::Print2() {

  if (count_ >= 5)
    return;
  std::cout << "timer2:" << ++count_ << std::endl;
  timer2_.expires_at(timer2_.expiry() + std::chrono::seconds(1));
  timer2_.async_wait(strand_.wrap([&](const boost::system::error_code &error_code) {
    if (error_code != boost::asio::error::operation_aborted)this->Print2();
  }));
}

void TCPServer::start_accpect() {
  std::cout << acceptor_.local_endpoint() << "  run!" << std::endl;
  auto tcp_connection = TCPConnection::Create(io_service_);
  acceptor_.async_accept(tcp_connection->socket(), [this, tcp_connection](const boost::system::error_code &error_code) {
    handle_accpect(tcp_connection, error_code);
  });
}
void TCPServer::handle_accpect(TCPConnection::pointer pointer, const boost::system::error_code &e) {
  if (!e)
    pointer->start();
  std::cout << e.message() << std::endl;
  start_accpect();
}
TCPConnection::pointer TCPConnection::Create(boost::asio::io_service &io) {
  return pointer(new TCPConnection(io));
}
void TCPConnection::start() {
  message_ = MakeDaytimeString();
  socket_.async_write_some(boost::asio::buffer(message_),
                           [&](const boost::system::error_code &error, std::size_t bytes_transferred) {
                             handle_write(error, bytes_transferred);
                           });
}
void TCPConnection::handle_write(const boost::system::error_code &error, std::size_t bytes_transferred) {
  if (!error)
    std::cout << "write success!" << std::endl;
  else
    std::cerr << "write error!" << std::endl;
}

}

namespace echoclientserver {

void StartServer() {
  try {
    boost::asio::io_service io_service;
    EchoServer server(io_service, 2525); //将服务器程序绑定到该端口上
    server.Start();
    std::cout << "server is running!!!" << std::endl;
    io_service.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
int EchoClient() {
  try {
    enum { max_length = 1024 };
    using tcp = boost::asio::ip::tcp;
    boost::asio::io_service io_service;
    tcp::socket socket(io_service);
    tcp::resolver resolver(io_service);
    boost::asio::connect(socket, resolver.resolve({"192.168.1.175", "2525"}));
    std::cout << "please enter: ";
    char request[max_length];
    std::cin.getline(request, max_length);
    size_t request_len = strlen(request);
    boost::asio::write(socket, boost::asio::buffer(request, request_len));

    char reply[max_length];
    auto sz = boost::asio::read(socket, boost::asio::buffer(reply, request_len));
    (std::cout << "receive a message : ").write(reply,sz);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
}

namespace chatclientserver {


bool ChatMessage::DecodeHeader() {
  char header[HEADER_LENGTH + 1] = "";
  std::strncat(header, data_, HEADER_LENGTH);
  body_length_ = atoi(header);
  if (body_length_ > MAX_BODY_LENGTH) {
    body_length_ = 0;
    return false;
  }
  return true;
}
void ChatMessage::EncodeHeader() {
  char header[HEADER_LENGTH + 1] = "";
  std::sprintf(header, "%4d", static_cast<int>(body_length_));
  std::strncpy(data_, header, HEADER_LENGTH);
}
void ChatRoom::Join(ChatParticipantPtr participant) {
  participants_.insert(participant);
  for (const auto &msg  : recent_msgs_)
    participant->Deliver(msg);

}
void ChatRoom::Leave(ChatParticipantPtr participant) {
  participants_.erase(participant);
}
void ChatRoom::Deliver(const ChatMessage &msg) {
  recent_msgs_.push_back(msg);
  for (const auto &participant_ptr: participants_) {
    participant_ptr->Deliver(msg);
  }
}
void ChatSession::do_write() {
  auto self(shared_from_this());
  boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
                           [this, self](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                             if (!ec) {
                               write_msgs_.pop_front();
                               if (!write_msgs_.empty())
                                 do_write();
                             } else
                               room_.Leave(shared_from_this());
                           });
}
void ChatSession::do_read_body() {
  auto self(shared_from_this());
  boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                          [this, self](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                            if (!ec) {
                              std::cout.write(read_msg_.body(), read_msg_.body_length());
                              room_.Deliver(read_msg_);
                              do_read_header();
                            } else
                              room_.Leave(self);
                          });
}

void ChatSession::do_read_header() {
  auto self(shared_from_this());
  boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(), ChatMessage::HEADER_LENGTH),
                          [this, self](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                            if (!ec && read_msg_.DecodeHeader()) {
                              do_read_body();
                            } else
                              room_.Leave(self);
                          });
}

int StartServer() {
  try {
    boost::asio::io_service io_service;
    ChatServer server(io_service, tcp::endpoint(tcp::v4(), 2525));
    server.Start();
    std::cout << "server is running!\n";
    io_service.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  return 0;
}
int StartClient() {
  try {
    boost::asio::io_service io_service;
    tcp::resolver::query q("192.168.0.103", "2525");
    tcp::resolver resolver(io_service);
    auto end_points = resolver.resolve(q);
    ChatClient client(io_service, end_points);
    std::thread t([&io_service]() {
      io_service.run();
    });

    char line[ChatMessage::MAX_BODY_LENGTH + 1];
    while(std::cin.getline(line, ChatMessage::MAX_BODY_LENGTH + 1)) {
      ChatMessage chat_message;
      chat_message.set_body_length(strlen(line));
      std::memcpy(chat_message.body(), line, chat_message.body_length());
      chat_message.EncodeHeader();
      client.Write(chat_message);
    }
    client.Close();
    if(t.joinable())
      t.join();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

void ChatClient::do_connect(const boost::asio::ip::basic_resolver<tcp,boost::asio::executor>::results_type &end_points) {
  boost::asio::async_connect(socket_, end_points,
      [this]( const boost::system::error_code& ec,
          const tcp::endpoint&){
    if(!ec)
      do_read_header();
  });
}
void ChatClient::do_read_header() {
  boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(), ChatMessage::HEADER_LENGTH),
      [this](const boost::system::error_code &ec, std::size_t){
    if(!ec && read_msg_.DecodeHeader())
      do_read_body();
    else
      socket_.close();
  });
}
void ChatClient::do_read_body() {
  boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
      [this](const boost::system::error_code &ec, std::size_t){
    if(!ec) {
      (std::cout).write(read_msg_.body(), read_msg_.body_length()) << "\n";
      do_read_header();
    }
    else
      socket_.close();
  });
}
void ChatClient::do_write() {
  boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
      [this](const boost::system::error_code &ec, std::size_t){
    if(!ec) {
      write_msgs_.pop_front();
      if(!write_msgs_.empty())
        do_write();
    } else
      socket_.close();
  });
}
void ChatClient::Write(const ChatMessage &msg) {
  boost::asio::post(io_service_,[this, &msg](){
    bool write_in_process = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if(!write_in_process)
      do_write();
  });
}
}

namespace chatclientserver_version1 {
bool cstructheader::ParseMessagge(const std::string &input, int *type, std::string *outbuffer) {
  if(!type || !outbuffer)
    return false;
  auto pos = input.find_first_of(' ');
  if(pos == std::string::npos || pos == 0)
    return false;
  //分割命令
  const auto command = input.substr(0, pos);
  if(command == "BindName") {
    std::string name = input.substr(pos + 1);
    if(name.size() > 32) //名字字节限制
      return false;
    *type = MT_BIND_NAME;
    BindName bind_info{};
    bind_info.name_len_ = name.size();
    std::memcpy(bind_info.name_, name.data(), name.size());
    auto buffer = reinterpret_cast<const char *>(&bind_info);
    outbuffer->assign(buffer, buffer + sizeof(bind_info));
    return true;
  } else if(command == "Chat") {
    std::string chat_info = input.substr(pos + 1);
    if(chat_info.size() > 256) //消息字节限制
      return false;
    *type = MT_CHAT_INFO;
    ChatInformation bind_info{};
    bind_info.info_len_ = chat_info.size();
    std::memcpy(bind_info.information_, chat_info.data(), chat_info.size());
    auto buffer = reinterpret_cast<const char *>(&bind_info);
    outbuffer->assign(buffer, buffer + sizeof(bind_info));
    return true;
  }


  return false;
}

bool ChatMessage::DecodeHeader() {
  std::memcpy(&m_header_, data(), HEADER_LENGTH);
  if(m_header_.body_size_ > MAX_BODY_LENGTH) {
    std::cout << "body size " << m_header_.body_size_ << " "
              << m_header_.type_ << "\n";
    return false;
  }
  return true;
}


void ChatRoom::Join(ChatParticipantPtr participant) {
  participants_.insert(participant);
  for (const auto &msg  : recent_msgs_)
    participant->Deliver(msg);

}
void ChatRoom::Leave(ChatParticipantPtr participant) {
  participants_.erase(participant);
}
void ChatRoom::Deliver(const ChatMessage &msg) {
  recent_msgs_.push_back(msg);
  for (const auto &participant_ptr: participants_) {
    participant_ptr->Deliver(msg);
  }
}
void ChatSession::do_write() {
  auto self(shared_from_this());
  boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
                           [this, self](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                             if (!ec) {
                               write_msgs_.pop_front();
                               if (!write_msgs_.empty())
                                 do_write();
                             } else
                               room_.Leave(shared_from_this());
                           });
}
void ChatSession::handling_message() {
  if(read_msg_.type() == cstructheader::MT_BIND_NAME) {  //处理名字信息
    const cstructheader::BindName *bind =
        reinterpret_cast<const cstructheader::BindName *>(read_msg_.body());
    client_name_.assign(bind->name_, std::strlen(bind->name_));

  } else if(read_msg_.type() == cstructheader::MT_CHAT_INFO) {  //处理消息信息
    const cstructheader::ChatInformation *chat_info_ptr =
        reinterpret_cast<const cstructheader::ChatInformation *>(read_msg_.body());
    client_msg_.assign(chat_info_ptr->information_, std::strlen(chat_info_ptr->information_));
    auto room_info = build_room_info();
    ChatMessage chat_msg;
    chat_msg.set_message(cstructheader::MT_ROOM_INFO, &room_info, sizeof(room_info));
    room_.Deliver(chat_msg);
  }
}
cstructheader::RoomInformation ChatSession::build_room_info() const {
  cstructheader::RoomInformation room_info;
  room_info.bind_name_.name_len_ = client_name_.size();
  std::memcpy(room_info.bind_name_.name_, client_name_.data(), client_name_.size());
  room_info.chat_info_.info_len_ = client_msg_.size();
  std::memcpy(room_info.chat_info_.information_, client_msg_.data(), client_msg_.size());
  // 也许有问题
  return std::move(room_info); //TODO


}

void ChatSession::do_read_body() {
  auto self(shared_from_this());
  boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                          [this, self](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                            if (!ec) {
                              //  room_.Deliver(read_msg_);
                              handling_message();
                              do_read_header();

                            } else
                              room_.Leave(self);
                          });
}

void ChatSession::do_read_header() {
  auto self(shared_from_this());
  boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(), ChatMessage::HEADER_LENGTH),
                          [this, self](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                            if (!ec && read_msg_.DecodeHeader()) {
                              do_read_body();
                            } else
                              room_.Leave(self);
                          });
}

int StartServer() {
  try {
    boost::asio::io_service io_service;
    ChatServer server(io_service, tcp::endpoint(tcp::v4(), 2525));
    server.Start();
    std::cout << "server is running!\n";
    io_service.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  return 0;
}
int StartClient() {
  try {
    boost::asio::io_service io_service;
    tcp::resolver::query q("192.168.1.175", "2525");
    tcp::resolver resolver(io_service);
    auto end_points = resolver.resolve(q);
    ChatClient client(io_service, end_points);
    std::thread t([&io_service]() {
      io_service.run();
    });

    char line[ChatMessage::MAX_BODY_LENGTH + 1];
    while(std::cin.getline(line, ChatMessage::MAX_BODY_LENGTH + 1)) {
      ChatMessage chat_message;
      int type = cstructheader::MT_CHAT_INFO;//命令类型
      std::string input(line, line + std::strlen(line));
      std::string output;
      if(cstructheader::ParseMessagge(input, &type, &output)) {
        chat_message.set_message(type, output.data(), output.size());
        client.Write(chat_message);
        std::cout << "write message for server " << output.size() << "\n";
      }
    }
    client.Close();
    if(t.joinable())
      t.join();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

void ChatClient::do_connect(const boost::asio::ip::basic_resolver<tcp,boost::asio::executor>::results_type &end_points) {
  boost::asio::async_connect(socket_, end_points,
                             [this]( const boost::system::error_code& ec,
                                     const tcp::endpoint&){
                               if(!ec)
                                 do_read_header();
                             });
}
void ChatClient::do_read_header() {
  boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(), ChatMessage::HEADER_LENGTH),
                          [this](const boost::system::error_code &ec, std::size_t){
                            if(!ec && read_msg_.DecodeHeader()) {
                              std::cout << "1";
                              do_read_body();
                            }
                            else
                              socket_.close();
                          });
}
void ChatClient::do_read_body() {
  boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                          [this](const boost::system::error_code &ec, std::size_t){
                            if(!ec) {
                              std::cout << "2" << std::endl;
                              if(read_msg_.body_length() == sizeof(cstructheader::RoomInformation) && read_msg_.type() == cstructheader::MT_ROOM_INFO) {
                                const cstructheader::RoomInformation *info =
                                    reinterpret_cast<const cstructheader::RoomInformation *>(read_msg_.body());
                                assert(std::strlen(info->bind_name_.name_) <= sizeof(info->bind_name_.name_));
                                std::cout.write(info->bind_name_.name_, info->bind_name_.name_len_);
                                assert(std::strlen(info->chat_info_.information_) <= sizeof(info->chat_info_.information_));
                                std::cout << " says ";
                                std::cout.write(info->chat_info_.information_, info->chat_info_.info_len_);
                                std::cout << "\n";
                              }
                              do_read_header();
                            }
                            else
                              socket_.close();
                          });
}
void ChatClient::do_write() {
  boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
                           [this](const boost::system::error_code &ec, std::size_t){
                             if(!ec) {
                               write_msgs_.pop_front();
                               if(!write_msgs_.empty())
                                 do_write();
                             } else
                               socket_.close();
                           });
}
void ChatClient::Write(const ChatMessage &msg) {
  boost::asio::post(io_service_,[this, &msg](){
    bool write_in_process = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if(!write_in_process)
      do_write();
  });
}

}