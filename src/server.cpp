#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sstream>
#include <iostream>

// 接続の目印となるファイル
#define SOCKNAME "/tmp/udsock"

int main(void)
{
  // LOCAL で STREAM なソケットの作成
  int sb = socket(AF_LOCAL, SOCK_STREAM, 0);

  // 接続の目印の指定
  sockaddr_un addr;

  // addrが占めるメモリを0初期化
  bzero(&addr, sizeof(addr));

  // AF_LOCAL は AF_UNIX と同じ。AF = Address Family
  addr.sun_family = AF_LOCAL;

  // アドレスを構造体にコピー。UNIXドメインソケットでは、ファイルパスがアドレスになる
  strcpy(addr.sun_path, SOCKNAME);

  // 古い目印の削除(もし残っていると厄介)
  unlink(SOCKNAME);

  // 目印の公開
  bind(sb, (sockaddr *)&addr, sizeof(addr));

  // 接続要求の受信開始(同時に1回線まで接続可とする)
  listen(sb, 1);

  // 接続要求の受信
  int s = accept(sb, NULL, NULL);

  // データの受信
  const int buf_size = 64;
  char buf[buf_size + 1];
  std::stringstream ss;
  int read;
  while ((read = recv(s, buf, buf_size, 0)) > 0)
  {
    buf[read] = '\0';
    ss << buf;
  }

  // データの表示
  std::cout << "Server received : " << ss.str() << std::endl;

  // ソケットの廃止
  close(s);
  close(sb);

  // 目印の削除(義務ではないが礼儀)
  unlink(SOCKNAME);

  return 0;
}
