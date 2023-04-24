# Code from ChatGPT
import socket

PORT = 2000

server_address = ('127.0.0.1', PORT)
with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as client_socket:
    # Set x to 42
    x = 42
    x_packet = f"set_x {x}"
    client_socket.sendto(x_packet.encode('utf-8'), server_address)
    print(f"Sent: {x_packet}")

    # # Get y
    # y_packet = "get_y"
    # client_socket.sendto(y_packet.encode('utf-8'), server_address)
    # response, server = client_socket.recvfrom(PORT)
    # y = response.decode('utf-8')
    # print(f"Response: {y}")

