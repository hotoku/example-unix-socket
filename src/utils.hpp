#ifndef UTILS_HPP
#define UTILS_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

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
} // namespace example_socket

#endif // UTILS_HPP