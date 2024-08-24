// #include <stdio.h>
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
    // LOCAL で STREAM なソケットの作成
    int s = socket(AF_LOCAL, SOCK_STREAM, 0);

    // 接続の目印の指定
    sockaddr_un addr;

    // addrが占めるメモリを0初期化
    bzero(&addr, sizeof(addr));

    // AF_LOCAL は AF_UNIX と同じ。AF = Address Family
    addr.sun_family = AF_LOCAL;

    // アドレスを構造体にコピー。UNIXドメインソケットでは、ファイルパスがアドレスになる
    strcpy(addr.sun_path, SOCKNAME);

    // ソケットの回線への接続
    if (connect(s, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    // 送信するデータ
    std::stringstream ss;
    ss << "This is client. Very long message follows:";
    for (int i = 0; i < 100; i++)
    {
        ss << "Hello, World! " << i << std::endl;
    }

    // データの送信
    const auto num = ss.str().size();
    const auto sent = send(s, ss.str().c_str(), ss.str().size(), 0);
    std::cout << "Sent " << sent << " bytes of " << num << " bytes." << std::endl;

    // 回線の切断
    close(s);

    return 0;
}
