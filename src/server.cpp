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

  int fd_base = example_socket::socket_bind_listen(SOCKNAME);
  int s = accept(fd_base, NULL, NULL);

  // 最初にデータサイズを受診する
  size_t msg_size;
  int read = recv(s, &msg_size, sizeof(msg_size), 0);
  if (read < sizeof(msg_size))
  {
    throw std::runtime_error("recv size");
  }

  // データ本体を受信する
  const size_t buf_size = 64;
  char buf[buf_size + 1];
  std::stringstream ss;
  size_t expect = std::min(msg_size, buf_size);
  while ((read = recv(s, buf, expect, 0)) > 0)
  {
    if (read < expect)
    {
      throw std::runtime_error("recv data");
    }
    buf[read] = '\0';
    ss << buf;
    msg_size -= read;
    expect = std::min(msg_size, buf_size);
    if (expect == 0)
    {
      break;
    }
  }

  // データの表示
  std::cout << "Server received : " << ss.str() << std::endl;

  // ソケットの廃止
  close(s);
  close(fd_base);

  // 目印の削除(義務ではないが礼儀)
  unlink(SOCKNAME);

  return 0;
}
