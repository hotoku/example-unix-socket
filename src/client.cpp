#include "utils.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

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
    std::cout << "sent: " << sent << " bytes" << std::endl;
    if (sent < msg.size())
    {
        throw std::runtime_error("send data");
    }

    // レスポンスの受信
    std::size_t res_size;
    int read = recv(fd, &res_size, sizeof(res_size), 0);
    if (read < sizeof(res_size))
    {
        throw std::runtime_error("recv size");
    }
    std::vector<char> data;
    example_socket::recv_all(fd, data, res_size);
    const std::string res(data.begin(), data.end());
    std::cout << "Received response: " << res << std::endl;

    close(fd);

    return 0;
}
