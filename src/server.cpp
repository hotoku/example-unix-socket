#include "utils.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sstream>
#include <iostream>
#include <algorithm>

// 接続の目印となるファイル
#define SOCKNAME "/tmp/udsock"

int main(void)
{
  unlink(SOCKNAME);

  const int fd_base = example_socket::socket_bind_listen(SOCKNAME);
  const int fd_client = accept(fd_base, NULL, NULL);

  // 最初にデータサイズを受診する
  size_t msg_size;
  int read = recv(fd_client, &msg_size, sizeof(msg_size), 0);
  if (read < sizeof(msg_size))
  {
    throw std::runtime_error("recv size");
  }

  // データ本体を受信する
  std::vector<char> data;
  example_socket::recv_all(fd_client, data, msg_size);
  std::string msg(data.begin(), data.end());

  std::cout << "Received message: " << msg << std::endl;

  // レスポンスを返す
  std::stringstream ss;
  ss << "This is server. " << msg.size() << " bytes received.";
  msg = ss.str();
  msg_size = msg.size();

  int sent = send(fd_client, &msg_size, sizeof(msg_size), 0);
  if (sent < sizeof(msg_size))
  {
    throw std::runtime_error("send size");
  }
  sent = send(fd_client, msg.c_str(), msg.size(), 0);
  if (sent < msg.size())
  {
    throw std::runtime_error("send data");
  }

  // ソケットの廃止
  close(fd_client);
  close(fd_base);

  // 目印の削除(義務ではないが礼儀)
  unlink(SOCKNAME);

  return 0;
}
