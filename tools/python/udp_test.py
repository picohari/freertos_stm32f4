import socket
import struct
import time

# --- Configuration ---
REMOTE_IP = "192.168.0.164"   # IP of the LinuxCNC or microcontroller
REMOTE_PORT = 58427             # where we send data
LOCAL_PORT  = 58428             # where we listen for echo replies

# --- Setup sockets ---
send_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
recv_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# --- Struct format ---
packet_format = "<fffBBffII"         # Format: little endian, float, float, float, unsigned char
jog_format = "<fHhBBBB"
#packet_size = struct.calcsize(packet_format)
packet_size = struct.calcsize(jog_format)

print(f"Packet size: {packet_size} bytes")

# --- Some example data (check this in LinuxCNC HAL) ---

'''
typedef struct {
    
    float   pos_x;              // Axis positions (G53 machine absolute)
    float   pos_y;
    float   pos_z;

    uint8_t homed;              // Bitfield for homing status
    uint8_t num_axes;           // Number of configured axes

    float   feed_override;
    float   spindle_speed;

    uint32_t status_flags;      // bitfield for states (running, homed, etc.)
    uint32_t last_update_ms;    // timestamp

    //float   encoder_value;      // Test actual encoder value

} MachineState_t;
'''

G53_X, G53_Y, G53_Z = 10.123, 20.456, 30.789    # [float]  Position of each axis
HOME_X, HOME_Y, HOME_Z = 0, 0, 1                # [bits]   1 = Homed, 0 = Unhomed
HOMED = 0                                       # [uint8]  Holds all homing statutes for each axis
NUM_AXIS = 3
FEED_OVERRIDE = 12.345                          # [float]  Feedrate in mm/s
SPINDLE_SPEED = 8765                            # [uint16] Spindle RPM

STATUS_FLAG = 1
TIMESTAMP = 0

AXIS_SELECT = 0
ENCODER = 0

active_mask = 0
control = 0
io = 0    
encoder_count = 0
encoder_value = 0
axis_select = 0
range_mode = 0

# --- Data structure details ---
'''
Absolute machine position
G53_X = float .3f
G53_Y = float .3f
G53_Z = float .3f

Homing status of each axis
          ..CBAZYX
          ||||||||
HOMED = 0x00000000

'''

# --- Prepare the data structures (precalculations) ---
homing_list = [HOME_Z, HOME_Y, HOME_X]
HOMED = 0
for i, bit in enumerate(homing_list):
    HOMED |= bit << ((NUM_AXIS - 1) - i)

# --- Pack all into hal_data packet ---
hal_data = struct.pack(packet_format,
                       G53_X,               # 4
                       G53_Y,               # 4
                       G53_Z,               # 4
                       HOMED,               # 1
                       NUM_AXIS,            # 1
                       FEED_OVERRIDE,       # 4
                       SPINDLE_SPEED,       # 4
                       STATUS_FLAG,         # 4
                       TIMESTAMP,           # 4
                      )                     # 34 bytes total


# Bind receive socket to listen for incoming packets
recv_sock.bind(("192.168.0.73", LOCAL_PORT))
recv_sock.settimeout(1.0)  # 1 second timeout for receive

print(f"UDP test client started.")
print(f"Sending to {REMOTE_IP}:{REMOTE_PORT}, listening on port {LOCAL_PORT}")

try:
    counter = 0
    while True:
        # --- Send ping message (for testing) ---
        #ping = f"ping {counter}".encode("utf-8")
        
        # Create socket and send the data ()
        #send_sock.sendto(hal_data, (REMOTE_IP, REMOTE_PORT))
        
        #print(f"Sent: {hal_data.decode()}")

        # --- Try to receive echo ---
        try:
            data, addr = recv_sock.recvfrom(1024)
            print(f"Received from {addr}: {data.decode(errors='ignore')}")

            if len(data) != packet_size:
                print(f"⚠️ Invalid packet size ({len(data)} bytes) from {addr}")
                continue

            #G53_X, G53_Y, G53_Z, 
            #HOMED, NUM_AXIS,
            #FEED_OVERRIDE, SPINDLE_SPEED,
            #STATUS_FLAG, TIMESTAMP,
            #AXIS_SELECT, ENCODER 

            encoder_value, encoder_count, io, active_mask, control, axis_select, range_mode = struct.unpack(jog_format, data)

            # Ausgabe strukturieren
            print(f"\n📦 Packet from {addr}:")
            #print(f"  Position: X={G53_X:.3f}, Y={G53_Y:.3f}, Z={G53_Z:.3f}")
            #print(f"  Homed: {HOMED:08b}   NumAxes: {NUM_AXIS}")
            #print(f"  Feed Override: {FEED_OVERRIDE:.2f}")
            #print(f"  Spindle Speed: {SPINDLE_SPEED:.2f}")
            #print(f"  Status Flags: 0x{STATUS_FLAG:08X}")
            #print(f"  Last Update: {TIMESTAMP} ms")


            print(f"  active_mask: {active_mask}")
            print(f"  control: {control}")
            print(f"  io: {io}")
            print(f"  encoder_count: {encoder_count}")
            print(f"  encoder_value: {encoder_value:.1f}")
            print(f"  axis_select: {axis_select}")
            print(f"  range_mode: {range_mode}")

        except socket.timeout:
            print("(no response)")

        #counter += 1
        #time.sleep(1.0)  # send every second

except KeyboardInterrupt:
    print("\nStopped by user.")

finally:
    send_sock.close()
    recv_sock.close()
