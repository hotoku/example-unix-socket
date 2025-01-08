#include "utils.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <stdexcept>

static auto send_orig = send;
static auto recv_orig = recv;

namespace example_socket {
FileDescriptor::FileDescriptor(int fd_) : fd(fd_) {}
FileDescriptor::~FileDescriptor() {
  if (fd >= 0) {
    std::cout << "closing: " << this->fd << std::endl;
    close(fd);
  }
}
int FileDescriptor::get() const { return fd; }
FileDescriptorPtr FileDescriptor::create(int fd) {
  return std::make_unique<FileDescriptor>(fd);
}

void init_addr(sockaddr_un *addr, const char *socket_file_path) {
  bzero(addr, sizeof(*addr));
  addr->sun_family = AF_LOCAL;
  strcpy(addr->sun_path, socket_file_path);
}

FileDescriptorPtr socket_bind_listen(const char *socket_file_path) {
  /**
   * AF_LOCAL: UNIXドメインソケット
   * SOCK_STREAM: ストリームソケット、の意味。
   */

  auto fd = FileDescriptor::create(socket(AF_LOCAL, SOCK_STREAM, 0));
  if (fd->get() < 0) {
    throw std::runtime_error("socket");
  }

  sockaddr_un addr;
  init_addr(&addr, socket_file_path);

  /**
   * bind: ソケットにアドレスを割り当てる
   */
  int ret = bind(fd->get(), (sockaddr *)&addr, sizeof(addr));
  if (ret < 0) {
    throw std::runtime_error("bind");
  }

  /**
   * listen: ソケットを接続待ち状態にする
   */
  ret = listen(fd->get(), 1);
  if (ret < 0) {
    throw std::runtime_error("listen");
  }

  return fd;
}

FileDescriptorPtr socket_connect(const char *socket_file_path) {
  auto fd = FileDescriptor::create(socket(AF_LOCAL, SOCK_STREAM, 0));
  if (fd->get() < 0) {
    throw std::runtime_error("failed to open socket");
  }
  sockaddr_un addr;
  init_addr(&addr, socket_file_path);

  /**
   * connect: ソケットをサーバーに接続する
   */
  const auto ret = connect(fd->get(), (sockaddr *)&addr, sizeof(addr));
  if (ret < 0) {
    throw std::runtime_error("failed connecting to server");
  }

  return fd;
}

void send(int fd, const std::vector<char> &data) {
  const uint32_t size = data.size();
  const ssize_t ret = send_orig(fd, &size, sizeof(size), 0);
  if (ret < 0) {
    throw std::runtime_error("send");
  }

  ssize_t sent = 0;
  while (sent < size) {
    const ssize_t ret = send_orig(fd, data.data() + sent, size - sent, 0);
    if (ret < 0) {
      throw std::runtime_error("send");
    }
    sent += ret;
  }
}

void recv(int fd, std::vector<char> &data) {
  uint32_t size;
  const ssize_t ret = recv_orig(fd, &size, sizeof(size), 0);
  if (ret < 0) {
    throw std::runtime_error("recv");
  }

  data.resize(size);
  ssize_t received = 0;
  while (received < size) {
    const ssize_t ret =
        recv_orig(fd, data.data() + received, size - received, 0);
    if (ret < 0) {
      throw std::runtime_error("recv");
    }
    received += ret;
  }
}
}  // namespace example_socket
