#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <sstream>

#include "utils.hpp"

// 接続の目印となるファイル
#define SOCKNAME "/tmp/udsock"

namespace es = example_socket;

namespace {
std::string make_message() { /* 適当な長さのメッセージを作る */
  std::stringstream ss;
  for (int i = 0; i < 32; i++) {
    if (i > 0) ss << "|";
    ss << i;
  }
  const std::string& m = ss.str();
  return m;
}
}  // namespace

int main(void) {
  unlink(SOCKNAME);

  const auto fd_base = es::socket_bind_listen(SOCKNAME);
  const auto fd_client = accept(fd_base->get(), NULL, NULL);

  const auto m = make_message();
  std::vector<char> msg(m.begin(), m.end());

  es::send(fd_client, msg);
  es::send(fd_client, msg);

  unlink(SOCKNAME);

  return 0;
}
