import { ref, onMounted, onUnmounted } from "vue";

export function useWebSocket(url) {
  const socket = ref(null);
  const isConnected = ref(false);
  const messages = ref([]);

  const send = (data) => {
    if (socket.value && isConnected.value) {
      socket.value.send(JSON.stringify(data));
    }
  };

  const sendBinary = (arr) => {
    if (socket.value && isConnected.value) {
      socket.value.send(new Uint8Array(arr));
    }
  };

  const connect = () => {
    socket.value = new WebSocket(url);

    socket.value.addEventListener("open", () => {
      isConnected.value = true;
      console.log("✅ WebSocket connected");
    });

    socket.value.addEventListener("message", (event) => {
      const data = JSON.parse(event.data);
      messages.value.push(data);
    });

    socket.value.addEventListener("close", () => {
      isConnected.value = false;
      console.warn("⚠️ WebSocket closed — reconnecting in 1s...");
      setTimeout(connect, 1000);
    });

    socket.value.addEventListener("error", (err) => {
      console.error("WebSocket error:", err);
      socket.value.close();
    });
  };

  onMounted(connect);

  onUnmounted(() => {
    socket.value?.close();
  });

  return { isConnected, messages, send };
}
