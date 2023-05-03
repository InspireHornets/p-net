# Code from ChatGPT
import socket
import struct
from enum import Enum

PORT = 2000

class CommandType(Enum):
    NO_COMMAND = 0x00
    GET_X = 0x10
    GET_Y = 0x11
    GET_Z = 0x12
    SET_X = 0x20
    SET_Y = 0x21
    SET_Z = 0x22
    INVALID_COMMAND = 0xff

def _set(direction: CommandType, setpoint: int):
    command_type = struct.pack('>B', direction.value)
    command_data = struct.pack('>I', setpoint)

    return command_type + command_data


def set_x(setpoint: int):
    return _set(CommandType.SET_X, setpoint)

def set_y(setpoint: int):
    return _set(CommandType.SET_Y, setpoint)

server_address = ('127.0.0.1', PORT)
with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as client_socket:
    command = set_x(1337)
    client_socket.sendto(command, server_address)
    print(f"Sent: {command}")
    client_socket.sendto(set_y(2356), server_address)

    # Get y
    command_type = CommandType.GET_X
    command = struct.pack('<B', command_type.value)
    client_socket.sendto(command, server_address)
    response, server = client_socket.recvfrom(PORT)
    print(f"Response: {response}")

