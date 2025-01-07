import net from "net";
import { exit } from "process";

const socketPath = "/tmp/example-uds.sock";

let count = 0;

async function sleep(ms: number): Promise<void> {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}

/**
 * サーバーへの接続を試みるプロミスを返す。
 * サーバーへの接続が失敗した場合にはrejectする。
 */
function connectToServer(): Promise<net.Socket> {
  count++;
  let succeeded = false;
  return new Promise((resolve, reject) => {
    /**
     * net.createConnection()で、サーバーに接続するconnectionを作る。
     * この関数は、net.Socketインスタンスを返す。
     * 第二引数のコールバックは、net.Socketインスタンスのconnectイベントが発生したときに呼び出される。
     */
    const client = net.createConnection(socketPath, () => {
      console.log("Connected");
      succeeded = true;
      resolve(client);
    });
    client.on("error", (err) => {
      console.log(`connection error: ${err}`);
      succeeded = false;
    });
    client.on("data", (data) => {
      const data2 = data.toString();
      console.log(data2);
    });
    client.on("close", () => {
      if (succeeded) {
        /**
         * サーバーとの接続が成功したあとのcloseイベントでは、クライアントも終了する
         */
        console.log("bye");
        exit(0);
      } else {
        /**
         * サーバーとの接続が失敗したあとのcloseイベントでは、rejectする。
         */
        console.log("failed to connect", count);
        reject();
      }
    });
  });
}

async function waitUntilServerIsReady(): Promise<net.Socket> {
  let con = null;
  while (true) {
    try {
      /**
       * connectToServerがreject or resolveするまで待つ。
       */
      con = await connectToServer();
      /**
       * connectToServerがresolveした場合には、ループを抜ける。
       */
      break;
    } catch (err) {
      /**
       * connectToServerがrejectした場合には、1秒待つ。
       */
      console.log("sleep 1 second");
      await sleep(1000);
    }
  }
  return con; // ※ conの型は、ちゃんとnet.Socketと推論されている。すごい
}

const con = await waitUntilServerIsReady();

let message = 0;
while (true) {
  message++;
  console.log("send", message);
  con.write(`message ${message}`);
  await sleep(2000);
}
