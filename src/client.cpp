#include "utils.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include <sstream>
#include <iostream>

// 接続の目印となるファイル
#define SOCKNAME "/tmp/udsock"

int main(void)
{
    std::cout << "connecting" << std::endl;
    int fd = example_socket::socket_connect(SOCKNAME);

    // 送信するデータ
    std::stringstream ss;
    ss << "This is client. Very long message follows:";
    for (int i = 0; i < 100; i++)
    {
        ss << "Hello, World! " << i << std::endl;
    }
    const auto msg = ss.str();
    const std::size_t msg_size = msg.size();

    /**
     * 最初にデータのサイズを送信
     * note: 最初にデータサイズが来ること、データサイズのデータはsize_t型であることを
     * サーバーと共有しておく必要がある。
     */
    auto sent = send(fd, &msg_size, sizeof(msg_size), 0);
    if (sent < sizeof(msg_size))
    {
        throw std::runtime_error("send size");
    }

    // データ本体の送信
    sent = send(fd, msg.c_str(), msg.size(), 0);
    if (sent < msg.size())
    {
        throw std::runtime_error("send data");
    }

    close(fd);

    return 0;
}
