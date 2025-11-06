#!/usr/bin/env python3
import socket
import base64
import hashlib
import sys
import re

def websocket_key():
    return base64.b64encode(b"test_key_123456").decode()

def expected_accept(key):
    GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
    return base64.b64encode(
        hashlib.sha1((key + GUID).encode()).digest()
    ).decode()

def connect_ws(host, port=80, path="/stream"):
    key = websocket_key()
    req = (
        f"GET {path} HTTP/1.1\r\n"
        f"Host: {host}:{port}\r\n"
        f"Upgrade: websocket\r\n"
        f"Connection: Upgrade\r\n"
        f"Sec-WebSocket-Key: {key}\r\n"
        f"Sec-WebSocket-Version: 13\r\n\r\n"
    )

    print(f"[*] Connecting to {host}:{port}")
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(5)
    sock.connect((host, port))
    sock.sendall(req.encode())

    # Read a full header
    response = b""
    sock.settimeout(5)
    try:
        while not response.endswith(b"\r\n\r\n"):
            chunk = sock.recv(1024)
            if not chunk:
                break
            response += chunk
    except socket.timeout:
        print("\n⚠️ Timeout: partial data received:")
        print(response)
        print("[hex dump]", response.hex())
        sock.close()
        return

    print("[*] Raw response bytes (hex):", response[:60].hex(), "...")
    try:
        decoded = response.decode(errors="replace")
    except Exception:
        decoded = str(response)
    print("[*] Server response:\n")
    print(decoded)

    # Match headers robustly
    m = re.search(r"^HTTP/1\.\d\s+101", decoded, re.MULTILINE)
    accept = re.search(r"Sec-WebSocket-Accept:\s*(\S+)", decoded, re.IGNORECASE)
    if m and accept:
        expected = expected_accept(key)
        if accept.group(1).strip() == expected:
            print("\n✅ WebSocket handshake successful!")
        else:
            print(f"\n⚠️ Accept key mismatch!\nExpected: {expected}\nGot: {accept.group(1)}")
    else:
        print("\n❌ Handshake failed or malformed response!")

    sock.close()

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python websocket_test.py <host> <port>")
        sys.exit(1)
    connect_ws(sys.argv[1], int(sys.argv[2]))