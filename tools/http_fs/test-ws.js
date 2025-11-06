// test-websocket.js
import { WebSocketServer } from "ws";

const wss = new WebSocketServer({ port: 1234 });

console.log("✅ WebSocket test server running on ws://localhost:1234");

// Fake live sensor values
let temperature = 23.5;
let status = "OK";

// Broadcast helper
function broadcast(data) {
  const str = JSON.stringify(data);
  wss.clients.forEach(client => {
    if (client.readyState === 1) {
      client.send(str);
    }
  });
}

// Update temperature periodically
setInterval(() => {
  // simple random walk for demo
  temperature += (Math.random() - 0.5);
  broadcast({ temperature: Number(temperature.toFixed(2)), status });
}, 3000);

wss.on("connection", ws => {
  console.log("✅ Client connected");

  ws.send(JSON.stringify({
    temperature,
    status,
    message: "Welcome to test WebSocket server!"
  }));

  ws.on("message", raw => {
    let msg;
    try {
      msg = JSON.parse(raw.toString());
    } catch {
      console.warn("⚠️ Received non-JSON:", raw.toString());
      return;
    }

    console.log("📩 RX:", msg);

    // Handle GPIO demo command
    if (msg.command === "gpio") {
      const result = {
        gpio: msg.pin,
        action: msg.action,
        acknowledge: "ok"
      };

      console.log("⚙️ GPIO command:", result);
      ws.send(JSON.stringify(result));
    }

    // Client requesting status
    if (msg.command === "get_status") {
      ws.send(JSON.stringify({
        temperature,
        status
      }));
    }
  });

  ws.on("close", () => {
    console.log("❌ Client disconnected");
  });
});