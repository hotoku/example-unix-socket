#ifndef UTILS_HPP
#define UTILS_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <vector>

namespace example_socket
{
    /**
     * サーバー用
     * socket_file_pathのアドレスで待ち受けるunixドメインソケットを作る。
     */
    int socket_bind_listen(const char *socket_file_path);

    /**
     * クライアント用
     * socket_file_pathのアドレスに接続するunixドメインソケットを作る。
     */
    int socket_connect(const char *socket_file_path);

    void recv_all(int fd_client, std::vector<char> &ret, size_t msg_size);
} // namespace example_socket

#endif // UTILS_HPP