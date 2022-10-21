import socket
import threading
import time
import pathlib
import os

host = ""  # localhost
port = 12008
PACKET_SIZE = 1024

log = os.path.join(pathlib.Path(__file__).parent.resolve(), "track.nmea")
with open(log, "rb") as f:
    lines = f.readlines()


def work_thread(client):
    i = 0
    while True:  # continuously read and handle data
        i = (i + 1) % len(lines)
        data = lines[i]
        client.sendall(data)
        time.sleep(0.1)


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.bind((host, port))
    sock.listen(5)  # we should never have more than one client
    print(f"Listening on port {port}")

    client, addr = sock.accept()  # your script will block here waiting for a connection
    t = threading.Thread(target=work_thread, args=(client,))
    t.start()
