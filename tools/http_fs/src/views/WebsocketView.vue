<template>
  <div class="websocket">
    <section>
      <div>
        <h1>Websockets</h1>
        <div id="status_box" class="alert alert-info">{{ isConnected ? "Connected" : "Disconnected" }}</div>
      </div>
      <div>
        <h3>LED 4</h3>
        <div class="onoffswitch">
					<input type="checkbox" name="onoffswitch" class="onoffswitch-checkbox" id="led-switch" @click="setled(4)">
					<label class="onoffswitch-label" for="led-switch">
						<span class="onoffswitch-inner"></span>
						<span class="onoffswitch-switch"></span>
					</label>
				</div>
        <button @click="sendJSON({ text: 'Hello server!' })"  :disabled="!isConnected">Send Message</button>
        <button @click="gpio(4, 'off')"                     :disabled="!isConnected">Turn Pin 4 ON</button>
        <button @click="gpio(1, 'on')"                    :disabled="!isConnected">Toggle GPIO</button>
      </div>

      <h3>Control Panel</h3>

      <!-- Control Buttons -->
      <div>
        <button @click="connect" :disabled="isConnected">Connect</button>
        <button @click="disconnect" :disabled="!isConnected">Disconnect</button>
      </div>

      <!-- Sent Messages -->
      <div>
        <h4>Sent Messages</h4>
        <ul>
          <li v-for="(msg,i) in sentMessages" :key="'s'+i">
            {{ msg }}
          </li>
        </ul>
      </div>

      <!-- Received Messages -->
      <div>
        <h4>Received Messages</h4>
        <ul>
          <li v-for="(msg,i) in receivedMessages" :key="'r'+i">
            {{ msg }}
          </li>
        </ul>
      </div>

    </section>
    <section>
      <div>
        <h1>Server status</h1>
        <table class="table table-striped">
          <tbody>
            <tr>
              <td><b>Uptime:</b></td>
              <td id="uptime">{{uptime}}</td>
            </tr>
            <tr>
              <td><b>Free heap:</b></td>
              <td id="heap">{{heapsize}}</td>
            </tr>
            <tr>
              <td><b>LED state:</b></td>
              <td id="led">{{status}}</td>
            </tr>
            <tr>
              <td><b>Temperature:</b></td>
              <td id="temperature">{{temperature}}</td>
            </tr>
          </tbody>
        </table>
        <button @click="requestUpdate">Request Update</button>
      </div>
    </section>
    <section>
      <div>
        <h1>How it works</h1>
        <p>This demo uses 2 WebSockets: Status parameters are streamed by the server in JSON format every 2 seconds.
          A <code>websocket_task</code> is created each time a specific URI is requested.</p>
        <p>ADC values are being continuously polled by the client (i.e. your browser).
          Each time a WebSocket frame is received on the server side, <code>websocket_cb</code> function is being called.</p>
      </div>
    </section>
  </div>


</template>


<script setup>
  import { ref, onUnmounted } from "vue";

const WS_URL = "ws://192.168.0.164:80/stream";

let socket = null;

// Reactive connection + data
const isConnected = ref(false);
const temperature = ref("...°C");
const status = ref("...");
const heapsize = ref("...");
const uptime = ref("...");

// Message logs
const sentMessages = ref([]);
const receivedMessages = ref([]);

// Connect function
function connect() {
  if (socket && isConnected.value) return;

  socket = new WebSocket(WS_URL);

  socket.addEventListener("open", () => {
    isConnected.value = true;
    console.log("Connected to WebSocket");
  });

  socket.addEventListener("message", event => {
    try {
      const data = JSON.parse(event.data);
      receivedMessages.value.push(JSON.stringify(data));

      // Parse known values
      if (data.temperature !== undefined) temperature.value   = data.temperature + "°C";
      if (data.status !== undefined)      status.value        = data.status;
      if (data.heap !== undefined)        heapsize.value      = data.heap;
      if (data.uptime !== undefined)      uptime.value        = data.uptime;

    } catch (e) {
      console.warn("Non-JSON message:", event.data);
      receivedMessages.value.push(event.data);
    }
  });

  socket.addEventListener("close", () => {
    isConnected.value = false;
    console.log("WebSocket closed");
  });

  socket.addEventListener("error", err => {
    console.error("WebSocket error:", err);
  });
}

// Disconnect function
function disconnect() {
  if (socket) {
    socket.close();
    socket = null;
  }
  isConnected.value = false;
}

function sendData(obj) {
  if (!socket || socket.readyState !== 1) return;
    socket.send(obj);              // binary
}

// Send wrapper (track in list)
function sendJSON(obj) {
  if (!socket || socket.readyState !== 1) return;
  const message = JSON.stringify(obj);
  socket.send(message);             // JSON
  sentMessages.value.push(message);
}

// Example: GPIO toggle command
function gpio(pin, mode) {

  sendJSON({ cmd: "gpio", action: mode, pin: pin });
}

function setled(led) {

  if (document.getElementById("led-switch").checked) {
    sendJSON({ cmd: "led", action: 'on'});
  } else {
    sendJSON({ cmd: "led", action: 'off'});
  }
}

// Cleanup socket on page unload
onUnmounted(() => {
  disconnect();
});


</script>

<style>

.alert {
  margin-bottom: 25px;
}

.alert {
    text-align: left;
    border-width: 1px;
    border-style: solid;
    background-color: #E1F5FE;
    color: #03A9F4;
    border-color: #03A9F4;
    padding: 16px 14px;
    padding: 16px 14px;
    padding: 16px 14px;
}

.alert, .btn {
    border-radius: 5px;
}

.onoffswitch {
	position: relative; width: 90px;
	-webkit-user-select:none; -moz-user-select:none; -ms-user-select: none;
}
.onoffswitch-checkbox {
	display: none;
}
.onoffswitch-label {
	display: block; overflow: hidden; cursor: pointer;
	border: 2px solid #03A9F4; border-radius: 20px;
}
.onoffswitch-inner {
	display: block; width: 200%; margin-left: -100%;
	transition: margin 0.3s ease-in 0s;
}
.onoffswitch-inner:before, .onoffswitch-inner:after {
	display: block; float: left; width: 50%; height: 30px; padding: 0; line-height: 30px;
	font-size: 14px; color: white; font-family: Trebuchet, Arial, sans-serif; font-weight: bold;
	box-sizing: border-box;
}
.onoffswitch-inner:before {
	content: "ON";
	text-align: left;
	padding-left: 14px;
	background-color: #E1F5FE; color: #03A9F4;
}
.onoffswitch-inner:after {
	content: "OFF";
	padding-right: 14px;
	background-color: #FFFFFF; color: #999999;
	text-align: right;
}
.onoffswitch-switch {
	display: block; width: 18px; margin: 6px;
	background: #FFFFFF;
	position: absolute; top: 0; bottom: 0;
	right: 56px;
	border: 2px solid #03A9F4; border-radius: 20px;
	transition: all 0.3s ease-in 0s; 
}
.onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-inner {
	margin-left: 0;
}
.onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-switch {
	right: 0px; 
}


.table {
  display: table;
  width: 100%;
  border-width: 0;
  border-collapse: collapse;
  font-weight: 300;
  color: #526475;
  margin-top: 0;
  margin-bottom: 20px;
}

.table tr td {
  border-bottom: 1px solid #d1e1e8;
  padding-top: 10px;
  padding-bottom: 10px;
  padding-left: 10px;
}


@media (min-width: 1024px) {
  
}
</style>
