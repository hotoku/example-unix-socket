#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

namespace example_socket
{
    void init_addr(sockaddr_un *addr, const char *socket_file_path)
    {
        bzero(addr, sizeof(*addr));
        addr->sun_family = AF_LOCAL;
        strcpy(addr->sun_path, socket_file_path);
    }

    int socket_bind_listen(const char *socket_file_path)
    {
        /**
         * AF_LOCAL: UNIXドメインソケット
         * SOCK_STREAM: ストリームソケット、の意味。
         */
        int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
        if (fd < 0)
        {
            throw std::runtime_error("socket");
        }

        sockaddr_un addr;
        init_addr(&addr, socket_file_path);

        /**
         * bind: ソケットにアドレスを割り当てる
         */
        int ret = bind(fd, (sockaddr *)&addr, sizeof(addr));
        if (ret < 0)
        {
            throw std::runtime_error("bind");
        }

        /**
         * listen: ソケットを接続待ち状態にする
         */
        ret = listen(fd, 1);
        if (ret < 0)
        {
            throw std::runtime_error("listen");
        }

        return fd;
    }

    int socket_connect(const char *socket_file_path)
    {
        int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
        if (fd < 0)
        {
            throw std::runtime_error("socket");
        }

        sockaddr_un addr;
        init_addr(&addr, socket_file_path);

        /**
         * connect: ソケットをサーバーに接続する
         */
        const auto ret = connect(fd, (sockaddr *)&addr, sizeof(addr));
        if (ret < 0)
        {
            throw std::runtime_error("connect");
        }

        return fd;
    }

} // namespace example_socket
