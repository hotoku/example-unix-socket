const net = require("net");
const socketPath = "/tmp/my-uds-socket"; // Unixドメインソケットのパス

// サーバーの作成
const server = net.createServer((socket) => {
  console.log("クライアントが接続しました。");

  socket.on("data", (data) => {
    const receivedMessage = data.toString();
    console.log(
      `サーバーが受信したメッセージ: "${receivedMessage}" (長さ: ${receivedMessage.length}バイト)`
    );

    // 受信したメッセージが "helloworld" であることを確認
    if (receivedMessage === "helloworld") {
      console.log('✔ "helloworld" が単一のメッセージとして受信されました！');
    } else {
      console.log('⚠ "helloworld" 以外のメッセージが受信されました。');
    }
  });

  socket.on("end", () => {
    console.log("クライアントが切断しました。");
  });

  socket.on("error", (err) => {
    console.error("サーバーソケットエラー:", err);
  });
});

// ソケットファイルのクリーンアップ（サーバー起動前）
const fs = require("fs");
if (fs.existsSync(socketPath)) {
  fs.unlinkSync(socketPath);
}

// サーバーをリッスン開始
server.listen(socketPath, () => {
  console.log(
    `サーバーがUnixドメインソケット ${socketPath} でリッスンを開始しました。`
  );
  console.log("クライアントを起動して接続してください...");
});

server.on("error", (err) => {
  console.error("サーバーエラー:", err);
});

// プロセス終了時のクリーンアップ
process.on("SIGINT", () => {
  console.log("\nサーバーをシャットダウンします...");
  server.close(() => {
    if (fs.existsSync(socketPath)) {
      fs.unlinkSync(socketPath);
      console.log("ソケットファイルを削除しました。");
    }
    process.exit(0);
  });
});
