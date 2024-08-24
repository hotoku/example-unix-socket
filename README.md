# example-socket

unix ドメインソケットでプロセス間で通信する例

[参考 1](http://cms.phys.s.u-tokyo.ac.jp/~naoki/CIPINTRO/NETWORK/localtcp.html)
[参考 2](http://cms.phys.s.u-tokyo.ac.jp/~naoki/CIPINTRO/NETWORK/struct.html)

## ざっくりとした手順

サーバー側

1. `sockaddr_un`構造体を初期化する。（昔のCプログラムは変数名が略されていて分かりづらいが、多分、`socket address unix network`だと思う）
2. `bind`する
3. `listen`する
4. `accept`する
5. `recv`する

`bind`, `listen`, `accept`の違いは良くわかってない。`accept`のところでプログラムはブロックする。実際のデータは`recv`で受け取る。

クライアント側

1. `sockaddr_un`構造体を初期化する。これはサーバーと同じ。アドレス（=ファイルパス）をサーバーと共有しておく必要がある。
2. `connect`する
3. `send`する

## その他

unixドメインソケットには、ストリーム型（TCP型）とデータグラム型（UDP型）がある。ここでの例はTCP型。

unixドメインソケットは、同一ホスト内でのプロセス間通信に使われる。ネットワーク越しに他のホストと通信する方法も、ソケット通信と呼ばれ、ほとんど同じAPIで動作する。
