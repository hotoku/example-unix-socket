const net = require("net");
const socketPath = "/tmp/my-uds-socket"; // サーバーと同じパス

const client = net.createConnection({ path: socketPath }, () => {
  console.log("サーバーに接続しました。");

  // 1. "hello" を送信
  client.write("hello");
  console.log('クライアントが "hello" を送信しました。');

  // 2. 続けて "world" を送信
  client.write("world");
  console.log('クライアントが "world" を送信しました。');

  // 少し待ってから切断（サーバーが受信する時間を与えるため）
  setTimeout(() => {
    client.end();
    console.log("クライアントが切断しました。");
  }, 100);
});

client.on("data", (data) => {
  console.log(
    `クライアントがサーバーから受信したメッセージ: ${data.toString()}`
  );
});

client.on("error", (err) => {
  console.error("クライアントソケットエラー:", err);
});

client.on("close", () => {
  console.log("クライアント接続が閉じられました。");
});
