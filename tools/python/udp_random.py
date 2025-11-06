import socket
import struct
import time
import random
import math

# --- Configuration ---
REMOTE_IP = "192.168.0.164"   # IP of the LinuxCNC or microcontroller
REMOTE_PORT = 58427             # where we send data
LOCAL_PORT  = 58428             # where we listen for echo replies

# --- Update rate configuration ---
UPDATE_RATE_HZ = 50             # Updates per second (1-100 Hz recommended)
                                # 1 Hz = once per second (slow)
                                # 10 Hz = 10 times per second (typical LinuxCNC)
                                # 50 Hz = 50 times per second (stress test)
                                # 100 Hz = 100 times per second (extreme)

# --- Setup sockets ---
send_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
recv_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# --- Struct format ---
packet_format = "<fffBBffII"         # Format: little endian used for MachineState_t
hal_format    = "<3f3f3f3BBH"
# packet_size = struct.calcsize(packet_format)
packet_size = struct.calcsize(hal_format)

print(f"Packet size: {packet_size} bytes")

# --- Axis ranges ---
X_MIN, X_MAX = 0.0, 1000.0
Y_MIN, Y_MAX = 0.0, 600.0
Z_MIN, Z_MAX = 0.0, 80.0
FEED_MIN, FEED_MAX = 0.0, 200.0
SPINDLE_MIN, SPINDLE_MAX = 0.0, 10000.0

# --- Choose update mode ---
UPDATE_MODE = "increment"  # Options: "increment", "random", "sine"

# --- Initial values ---
G53_X = 100.0
G53_Y = 50.0
G53_Z = 10.0
FEED_OVERRIDE = 100.0
SPINDLE_SPEED = 1000.0

# --- Increment step sizes (per update) ---
# Note: These are per update, so actual speed depends on UPDATE_RATE_HZ
# For example: X_STEP=0.5 at 10 Hz = 5 mm/s actual movement
X_STEP = 0.05    # mm per update
Y_STEP = 0.03    # mm per update
Z_STEP = 0.1     # mm per update
FEED_STEP = 1.0
SPINDLE_STEP = 50.0

# --- Direction flags for increment mode ---
x_direction = 1
y_direction = 1
z_direction = 1
feed_direction = 1
spindle_direction = 1

# --- Static values ---
HOME_X, HOME_Y, HOME_Z = 1, 1, 1    # All axes homed
NUM_AXIS = 3
STATUS_FLAG = 1

# --- Prepare homing bitfield ---
homing_list = [HOME_Z, HOME_Y, HOME_X]
HOMED = 0
for i, bit in enumerate(homing_list):
    HOMED |= bit << ((NUM_AXIS - 1) - i)


# --- HAL Structure ---
HAL_POS = [1000, 2000, 3000]
HAL_VEL = [1.5, 2.5, 3.5]
HAL_SCALE = [1.0, 0.5, 0.25]
HAL_ENABLE = [1, 1, 0]
HAL_CONTROL = 7
HAL_IO = 0xFFFF


# Bind receive socket to listen for incoming packets
recv_sock.bind(("192.168.0.73", LOCAL_PORT))
recv_sock.settimeout(0.01)  # Short timeout for non-blocking receive
recv_sock.setblocking(False)  # Non-blocking mode for high-rate updates

UPDATE_INTERVAL = 1.0 / UPDATE_RATE_HZ  # Calculate interval in seconds

print(f"UDP test client started (mode: {UPDATE_MODE})")
print(f"Sending to {REMOTE_IP}:{REMOTE_PORT}, listening on port {LOCAL_PORT}")
print(f"Update rate: {UPDATE_RATE_HZ} Hz (every {UPDATE_INTERVAL*1000:.1f} ms)")
print(f"X range: {X_MIN}-{X_MAX}, Y range: {Y_MIN}-{Y_MAX}, Z range: {Z_MIN}-{Z_MAX}")
print("-" * 60)

def update_increment(value, step, direction, min_val, max_val):
    """Update value with bounce-back at limits"""
    new_value = value + (step * direction)
    new_direction = direction
    
    if new_value >= max_val:
        new_value = max_val
        new_direction = -1
    elif new_value <= min_val:
        new_value = min_val
        new_direction = 1
    
    return new_value, new_direction

try:
    counter = 0
    start_time = time.time()
    next_update_time = start_time
    
    while True:
        current_time = time.time()
        
        # Only update if enough time has passed
        if current_time >= next_update_time:
            # --- Update values based on mode ---
            if UPDATE_MODE == "increment":
                G53_X, x_direction = update_increment(G53_X, X_STEP, x_direction, X_MIN, X_MAX)
                G53_Y, y_direction = update_increment(G53_Y, Y_STEP, y_direction, Y_MIN, Y_MAX)
                G53_Z, z_direction = update_increment(G53_Z, Z_STEP, z_direction, Z_MIN, Z_MAX)
                FEED_OVERRIDE, feed_direction = update_increment(FEED_OVERRIDE, FEED_STEP, feed_direction, FEED_MIN, FEED_MAX)
                SPINDLE_SPEED, spindle_direction = update_increment(SPINDLE_SPEED, SPINDLE_STEP, spindle_direction, SPINDLE_MIN, SPINDLE_MAX)
                
            elif UPDATE_MODE == "random":
                G53_X = random.uniform(X_MIN, X_MAX)
                G53_Y = random.uniform(Y_MIN, Y_MAX)
                G53_Z = random.uniform(Z_MIN, Z_MAX)
                FEED_OVERRIDE = random.uniform(FEED_MIN, FEED_MAX)
                SPINDLE_SPEED = random.uniform(SPINDLE_MIN, SPINDLE_MAX)
                
            elif UPDATE_MODE == "sine":
                # Smooth sine wave motion
                t = current_time - start_time
                G53_X = X_MIN + (X_MAX - X_MIN) * (0.5 + 0.5 * math.sin(t * 0.2))
                G53_Y = Y_MIN + (Y_MAX - Y_MIN) * (0.5 + 0.5 * math.sin(t * 0.3))
                G53_Z = Z_MIN + (Z_MAX - Z_MIN) * (0.5 + 0.5 * math.sin(t * 0.5))
                FEED_OVERRIDE = FEED_MIN + (FEED_MAX - FEED_MIN) * (0.5 + 0.5 * math.sin(t * 0.15))
                SPINDLE_SPEED = SPINDLE_MIN + (SPINDLE_MAX - SPINDLE_MIN) * (0.5 + 0.5 * math.sin(t * 0.1))
            
            # --- Get timestamp in milliseconds ---
            TIMESTAMP = int((current_time - start_time) * 1000) & 0xFFFFFFFF
            

            # --- Pack HAL data ---
            HAL_POS = [G53_X, G53_Y, G53_Z]
            HAL_VEL = [1.5, 2.5, 3.5]
            HAL_SCALE = [1.0, 0.5, 0.25]
            HAL_ENABLE = [1, 1, 0]
            HAL_CONTROL = 7
            HAL_IO = 0x00FF

            # --- Pack all into hal_data packet ---
            hal_data = struct.pack(hal_format,
                                   *HAL_POS,
                                   *HAL_VEL,
                                   *HAL_SCALE,
                                   *HAL_ENABLE,
                                   HAL_CONTROL,
                                   HAL_IO
                                   )
            
            '''
            hal_data = struct.pack(packet_format,
                                   G53_X,               # 4 bytes
                                   G53_Y,               # 4 bytes
                                   G53_Z,               # 4 bytes
                                   HOMED,               # 1 byte
                                   NUM_AXIS,            # 1 byte
                                   FEED_OVERRIDE,       # 4 bytes
                                   SPINDLE_SPEED,       # 4 bytes
                                   STATUS_FLAG,         # 4 bytes
                                   TIMESTAMP            # 4 bytes
                                  )                     # 30 bytes total
            
            '''
            
            # --- Send data ---
            send_sock.sendto(hal_data, (REMOTE_IP, REMOTE_PORT))
            

            # Print current values periodically
            print_interval = max(1, UPDATE_RATE_HZ)  # Print at least once per second
            if counter % print_interval == 0:
                print(f"[{counter:5d}] X:{G53_X:7.3f} Y:{G53_Y:7.3f} Z:{G53_Z:6.3f} "
                      f"Feed:{FEED_OVERRIDE:6.1f} Spindle:{SPINDLE_SPEED:7.1f}")
            
            counter += 1
            next_update_time += UPDATE_INTERVAL
        
        # --- Try to receive echo (non-blocking) ---
        try:
            data, addr = recv_sock.recvfrom(1024)
            # Uncomment to see responses:
            # print(f"Received from {addr}: {len(data)} bytes")
        except (socket.timeout, BlockingIOError):
            pass  # No response is normal
        
        # Small sleep to prevent 100% CPU usage
        time.sleep(0.001)  # 1ms sleep

except KeyboardInterrupt:
    print("\n" + "=" * 60)
    print("Stopped by user.")
    print(f"Total packets sent: {counter}")

finally:
    send_sock.close()
    recv_sock.close()