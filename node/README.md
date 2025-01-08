# node で Unix Domain socket で通信する例

## 動作内容

サーバー・クライアントそれぞれのプロセスが、Unix Domain Socket を用いて IPC を行う例。socket のパスは、`/tmp/example-uds.sock`を用いる。

サーバー側の動作

- socket ファイルを指定して、接続を待ち受ける
- サーバーは、コネクションを保存しておく配列を持っており、新しいクライアントから接続がある度に、コネクションを保持する
- サーバーは、一秒ごとに、接続済みのクライアントに対してメッセージを送る
- サーバーは、クライアントからデータを受信すると、コンソールに表示する

クライアント側の動作

- socket ファイルのパスに接続を試みる
  - 失敗した場合は 1 秒待って再接続（成功するまで無限に試す）
- クライアントは、サーバーからデータを受信すると、コンソールに表示する
- クライアントは、一秒ごとに、サーバーに対してメッセージを送る

## 起動方法

サーバー

```shell
npm run start-server
```

クライアント

```shell
npm run start-client
```

サーバーは 1 つだけ起動する。クライアントは、何個起動しても良い。

## クライアントの実装について

クライアント側で「サーバーへの接続に失敗した場合は 1 秒待ってから再試行」という動作を実装しようとして苦労したので、概要をメモしておく。

当初は、以下のような実装を考えていた。

```typescript
async function connectToServer(): Promise<net.Socket> {
  return net.createConnection(/*略*/);
}

async function waitUntilServerIsReady(): Promise<net.Socket> {
  let con = null;
  while (true) {
    try {
      con = connectToServer();
      break;
    } catch (err) {
      await sleep(1000);
    }
  }
  return con;
}
```

`net.createConnection`接続に失敗したときは例外を投げてくれることを期待して上のような実装を試したが、動作しないことが分かった。
これがなぜ上手くいかないかをざっくりと理解したところ、以下のような事情のようである。

`net.createConnection`自身は、接続に失敗してもそれ自身からは例外を出さない。
`net.createConnection`は、とにかく`net.Socket`のインスタンスを返す。
`net.Socket`インスタンスは、接続に失敗した時点で、`error`イベントを受け取る。
接続に失敗した場合に何かしたい場合には、この`error`イベントのリスナーを登録しなければならない。

これを踏まえて、[client.ts](src/client.ts)の`connectToServer`関数は、

- 接続に成功して`net.Socket`を`resolve`する、または
- 失敗して`reject`する

ようなプロミスを返している。詳しく見ると、

- 接続に成功 -> `connect`イベントが発生 -> `createConnection`の第二引数のコールバックが発動 -> `net.Socket`が`resolve`される
- 接続に失敗 -> `error`イベントが発生、その後`close`イベントが発生 -> `client.on("close", ...)`で登録したリスナーが発動 -> `reject`される

というような動作をするようになっている。むずかしい

## Docker 越しに試す

イメージをビルド

```shell
docker build -t ex-unix-sock:latest .
```

サーバーを起動

```shell
docker run -v $(pwd)/shared:/tmp -it --rm --name server ex-unix-sock:latest start-server
```

クライアントを起動

```shell
docker run -v $(pwd)/shared:/tmp -it --rm --name client ex-unix-sock:latest start-client
```
