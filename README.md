# example-dev-container

docker コンテナに VS Code で接続し、c++のプログラムをデバッグする例。

## コンテナ

ubuntu 24.04 をベースに、基本的な開発用ツールをインストール。`make`を実行すれば、イメージのビルドが走ったあとに、コンテナが起動する。
コンテナを起動したあとに、VS Code で `Dev Containers: Attach to Running Container`を実行すれば、コンテナに接続される。

## cmake

プログラムのビルドに cmake を使用。関連するファイルだけ抜粋すると以下のような感じ。

```
.
├── CMakeLists.txt
└── src
    ├── CMakeLists.txt
    ├── hello.cpp
    ├── hello.hpp
    └── main.cpp
```

## tasks.json, launch.json

tasks.json に、プログラムをビルドする手順が書かれている。

launch.json に、デバッガを起動するための設定が書かれている。

[参考にしたサイト](https://motchy869.com/wordpress/2021/05/09/cmake%E3%81%A7%E3%83%93%E3%83%AB%E3%83%89%E3%81%97%E3%81%A6%E3%81%84%E3%82%8B%E7%92%B0%E5%A2%83%E4%B8%8B%E3%81%A7vs-code%E3%81%A7%E3%83%87%E3%83%90%E3%83%83%E3%82%B0%E3%81%99%E3%82%8B/)

`main.cpp`を開いた状態で、`F5`を押せばデバッグが始まる。
