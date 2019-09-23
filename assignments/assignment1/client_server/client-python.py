###############################################################################
# client-python.py
# Name:
# NetId:
###############################################################################

import sys
import socket

SEND_BUFFER_SIZE = 2048


def client(server_ip, server_port):
    """TODO: Open socket and send message from sys.stdin"""
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((server_ip, server_port))

    send_buff = ""
    for line in sys.stdin:
        send_buff += line
    sock.sendall(line)
    data = sock.recv(SEND_BUFFER_SIZE)
    sock.close()
    print("Received: %s", data)


def main():
    """Parse command-line arguments and call client function """
    if len(sys.argv) != 3:
        sys.exit(
            "Usage: python client-python.py [Server IP] [Server Port] < [message]")
    server_ip = sys.argv[1]
    server_port = int(sys.argv[2])
    client(server_ip, server_port)


if __name__ == "__main__":
    main()
