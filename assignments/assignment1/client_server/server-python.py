###############################################################################
# server-python.py
# Name:
# NetId:
###############################################################################

import sys
import socket
import sys

RECV_BUFFER_SIZE = 2048
QUEUE_LENGTH = 10


def server(server_port):
    """TODO: Listen on socket and print received message to sys.stdout"""
    sock = socket.socket(socket.AF_INET,
                         socket.SOCK_STREAM)
    sock.bind(("", server_port))
    sock.listen(QUEUE_LENGTH)
    print("Server is listening...")
    conn, addr = sock.accept()
    print("Connected by ", addr)
    while True:
        data = conn.recv(RECV_BUFFER_SIZE)
        if not data:
            break
        conn.sendall(data)
    conn.close()
    sys.stdout.write(data)


def main():
    """Parse command-line argument and call server function """
    if len(sys.argv) != 2:
        sys.exit("Usage: python server-python.py [Server Port]")
    server_port = int(sys.argv[1])
    server(server_port)


if __name__ == "__main__":
    main()
