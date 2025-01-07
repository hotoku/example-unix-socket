import net from "net";
import fs from "fs";

const socketPath = "/tmp/example-uds.sock";
if (fs.existsSync(socketPath)) {
  console.log("remove socket file");
  fs.unlinkSync(socketPath);
}

let connections: (net.Socket | null)[] = [];

/**
 * サーバーは、net.createServer() で作成する。
 * クライアントからの接続があるたびに、コールバック関数が呼び出される。
 * コールバック関数の引数には、接続情報を表す net.Socket インスタンスconが渡される。
 * con.onメソッドで、このインスタンスに関して発生したイベントに対するリスナーを登録できる。
 */
const server = net.createServer((con) => {
  const index = connections.length;
  con.on("close", () => {
    console.log("close", index);
    connections[index] = null;
  });
  con.on("data", (data) => {
    console.log(data.toString(), "from", index);
  });
  con.on("error", (err) => {
    console.log("error", index);
    console.error(err);
  });
  connections.push(con);
});

/**
 * server.onで、サーバーに関するイベントに対するリスナーを登録できる。
 */
server.on("error", (err) => {
  console.error(err);
});
server.on("close", () => {
  console.log("close");
});

/**
 * サーバーは、listen() メソッドで、指定したソケットファイルパスで接続を待ち受ける。
 *
 */
server.listen(socketPath, () => {
  console.log("start listening");
});

async function sleep(ms: number) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}

let count = 0;
while (true) {
  await sleep(1000);
  const cons = connections.filter((con) => con !== null) as net.Socket[];
  console.log("send", count, "to", cons.length, "clients");
  for (const con of cons) {
    con.write(`message ${count}`);
  }
  count++;
}
