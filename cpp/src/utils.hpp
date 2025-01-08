#ifndef UTILS_HPP
#define UTILS_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <memory>
#include <vector>

namespace example_socket {
class FileDescriptor;
typedef std::unique_ptr<FileDescriptor> FileDescriptorPtr;

class FileDescriptor {
 public:
  FileDescriptor(int fd_);
  ~FileDescriptor();
  int get() const;
  static FileDescriptorPtr create(int fd);

 private:
  int fd;
};

/**
 * サーバー用
 * socket_file_pathのアドレスで待ち受けるunixドメインソケットを作る。
 */
FileDescriptorPtr socket_bind_listen(const char *socket_file_path);

/**
 * クライアント用
 * socket_file_pathのアドレスに接続するunixドメインソケットを作る。
 */
FileDescriptorPtr socket_connect(const char *socket_file_path);

/**
 * Unixドメインソケットでは、データの区切りがないため、どこまでが1つのデータかを明示する必要がある。
 * 一番単純には、先頭の4バイトにデータの長さを入れる。
 */

void send(int fd, const std::vector<char> &data);
void recv(int fd, std::vector<char> &data);

}  // namespace example_socket

#endif  // UTILS_HPP
