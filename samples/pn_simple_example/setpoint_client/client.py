import socket
import struct
from enum import Enum


class CommandType(Enum):
    NO_COMMAND = 0x00
    GET_X_POSITION_UM = 0x10
    GET_Y_POSITION_UM = 0x11
    GET_Z_POSITION_UM = 0x12
    GET_X_SPEED_MM_MIN = 0x13
    GET_Y_SPEED_MM_MIN = 0x14
    GET_Z_SPEED_MM_MIN = 0x15
    GET_X_ACCELERATION_MM_MIN2 = 0x16
    GET_Y_ACCELERATION_MM_MIN2 = 0x17
    GET_Z_ACCELERATION_MM_MIN2 = 0x18
    GET_X_POWER = 0x19
    GET_Y_POWER = 0x1A
    GET_Z_POWER = 0x1B
    GET_X_TEMPERATURE = 0x1C
    GET_Y_TEMPERATURE = 0x1D
    GET_Z_TEMPERATURE = 0x1E
    SET_X_POSITION_UM = 0x20
    SET_Y_POSITION_UM = 0x21
    SET_Z_POSITION_UM = 0x22
    SET_X_SPEED_MM_MIN = 0x23
    SET_Y_SPEED_MM_MIN = 0x24
    SET_Z_SPEED_MM_MIN = 0x25
    SET_X_ACCELERATION_MM_MIN2 = 0x26
    SET_Y_ACCELERATION_MM_MIN2 = 0x27
    SET_Z_ACCELERATION_MM_MIN2 = 0x28
    INVALID_COMMAND = 0xFF


def _set(direction: CommandType, setpoint: int):
    command_type = struct.pack("<B", direction.value)
    command_data = struct.pack("<I", setpoint)

    return command_type + command_data


def _get(command: CommandType):
    return struct.pack(">B", command.value)


def set_x(setpoint: int):
    return _set(CommandType.SET_X_POSITION_UM, setpoint)


class SetpointClient:
    def __init__(self, host: str, port: int):
        self.host = host
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def get_x_position(self) -> int:
        command_type = CommandType.GET_X_POSITION_UM
        command = struct.pack(">B", command_type.value)
        self._send(command)

        answer = struct.unpack("<BI", self._receive())
        assert (
            answer[0] == CommandType.GET_X_POSITION_UM.value
        ), f"PLC returned {answer[0]}, expected {CommandType.GET_X_POSITION_UM.value,}"

        return answer[1]

    def get_x_speed(self) -> int:
        command_type = CommandType.GET_X_SPEED_MM_MIN
        command = struct.pack(">B", command_type.value)
        self._send(command)

        answer = struct.unpack("<BI", self._receive())
        assert (
            answer[0] == CommandType.GET_X_SPEED_MM_MIN.value
        ), f"PLC returned {answer[0]}, expected {CommandType.GET_X_SPEED_MM_MIN.value,}"

        return answer[1]

    def get_x_acceleration(self) -> int:
        command_type = CommandType.GET_X_ACCELERATION_MM_MIN2
        command = struct.pack(">B", command_type.value)
        self._send(command)

        answer = struct.unpack("<BI", self._receive())
        assert (
            answer[0] == CommandType.GET_X_ACCELERATION_MM_MIN2.value
        ), f"PLC returned {answer[0]}, expected {CommandType.GET_X_ACCELERATION_MM_MIN2.value,}"

        return answer[1]

    def _send(self, command: bytes) -> None:
        self.socket.sendto(command, (self.host, self.port))

    def _receive(self):
        response, _ = self.socket.recvfrom(1024)
        return response

    def set_x_position(self, setpoint: int):
        self._send(set_x(setpoint))

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.disconnect()

    def disconnect(self):
        self.socket.close()
