#include "utils.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sstream>
#include <iostream>

// 接続の目印となるファイル
#define SOCKNAME "/tmp/udsock"

int main(void)
{
  unlink(SOCKNAME);

  int fd_base = example_socket::socket_bind_listen(SOCKNAME);
  int s = accept(fd_base, NULL, NULL);

  // データの受信
  const int buf_size = 64;
  char buf[buf_size + 1];
  std::stringstream ss;
  int read;
  while ((read = recv(s, buf, buf_size, 0)) > 0)
  {
    buf[read] = '\0';
    ss << buf;
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
