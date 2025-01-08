#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "utils.hpp"

// 接続の目印となるファイル
#define SOCKNAME "/tmp/udsock"

namespace es = example_socket;

int main(void) {
  std::cout << "connecting" << std::endl;
  int fd = es::socket_connect(SOCKNAME);

  std::vector<char> msg;
  es::recv(fd, msg);
  std::cout << "received: " << std::string(msg.begin(), msg.end()) << std::endl;

  es::recv(fd, msg);
  std::cout << "received: " << std::string(msg.begin(), msg.end()) << std::endl;

  close(fd);

  return 0;
}
