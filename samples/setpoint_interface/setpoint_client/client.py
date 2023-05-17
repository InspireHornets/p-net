import socket
import struct
from enum import Enum
from typing import Tuple

from hat.ddd import ValueObject


class CommandType(Enum):
    NO_COMMAND = 0x00

    GET_X_POSITION_UM = 0x10
    GET_Y_POSITION_UM = 0x11
    GET_Z_POSITION_UM = 0x12
    GET_X_SPEED_UM_S = 0x13
    GET_Y_SPEED_UM_S = 0x14
    GET_Z_SPEED_UM_S = 0x15
    GET_X_ACCELERATION_UM_S2 = 0x16
    GET_Y_ACCELERATION_UM_S2 = 0x17
    GET_Z_ACCELERATION_UM_S2 = 0x18
    GET_X_POWER = 0x19
    GET_Y_POWER = 0x1A
    GET_Z_POWER = 0x1B
    GET_X_TEMPERATURE = 0x1C
    GET_Y_TEMPERATURE = 0x1D
    GET_Z_TEMPERATURE = 0x1E
    GET_X_TRAJECTORY_POINT = 0x20
    GET_Y_TRAJECTORY_POINT = 0x21
    GET_Z_TRAJECTORY_POINT = 0x22

    SET_X_POSITION_UM = 0x40
    SET_Y_POSITION_UM = 0x41
    SET_Z_POSITION_UM = 0x42
    SET_X_SPEED_UM_MIN = 0x43
    SET_Y_SPEED_UM_MIN = 0x44
    SET_Z_SPEED_UM_MIN = 0x45
    SET_X_ACCELERATION_UM_MIN2 = 0x46
    SET_Y_ACCELERATION_UM_MIN2 = 0x47
    SET_Z_ACCELERATION_UM_MIN2 = 0x48
    SET_X_STATE = 0x49
    SET_Y_STATE = 0x4A
    SET_Z_STATE = 0x4B
    SET_X_TRAJECTORY_POINT = 0x50
    SET_Y_TRAJECTORY_POINT = 0x51
    SET_Z_TRAJECTORY_POINT = 0x52

    INVALID_COMMAND = 0xFF


um = int
um_s = int
um_s2 = int


class TrajectoryPoint(ValueObject):
    position: um
    speed: um_s
    acceleration: um_s2


def set_command(command: CommandType, *setpoints: int) -> bytes:
    command_type = struct.pack("<B", command.value)
    command_data = struct.pack("<" + "i" * len(setpoints), *setpoints)

    return command_type + command_data


def get_command(command: CommandType) -> bytes:
    return struct.pack(">B", command.value)


class SetpointClient:
    def __init__(self, host: str, port: int):
        self.host = host
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def get_command(self, command_type: CommandType) -> Tuple[int]:
        command = struct.pack(">B", command_type.value)
        self._send(command)

        return_value_size = 3 if command_type.value >= CommandType.GET_X_TRAJECTORY_POINT.value else 1
        format = "<B" + ("i" * return_value_size)
        answer = struct.unpack(format, self._receive())
        assert answer[0] == command_type.value, f"PLC returned {CommandType(answer[0])}, but expected {command_type}."

        return answer[1:]

    def get_x_position(self) -> int:
        return self.get_command(CommandType.GET_X_POSITION_UM)[0]

    def get_x_speed(self) -> int:
        return self.get_command(CommandType.GET_X_SPEED_UM_S)[0]

    def get_x_acceleration(self) -> int:
        return self.get_command(CommandType.GET_X_ACCELERATION_UM_S2)[0]

    def get_x_power(self) -> int:
        return self.get_command(CommandType.GET_X_POWER)[0]

    def get_x_temperature(self) -> int:
        return self.get_command(CommandType.GET_X_TEMPERATURE)[0]

    def get_x_trajectory(self) -> TrajectoryPoint:
        trajectory = self.get_command(CommandType.GET_X_TRAJECTORY_POINT)

        return TrajectoryPoint(position=trajectory[0], speed=trajectory[1], acceleration=trajectory[2])

    def set_x_position(self, setpoint: int) -> None:
        command = set_command(CommandType.SET_X_POSITION_UM, setpoint)
        self._send(command)

    def set_x_state(self, state: int) -> None:
        command = set_command(CommandType.SET_X_STATE, state)
        self._send(command)

    def set_x_trajectory(self, trajectory_point: TrajectoryPoint) -> None:
        command = set_command(
            CommandType.SET_X_TRAJECTORY_POINT,
            trajectory_point.position,
            trajectory_point.speed,
            trajectory_point.acceleration,
        )
        self._send(command)

    def _send(self, command: bytes) -> None:
        self.socket.sendto(command, (self.host, self.port))

    def _receive(self) -> bytes:
        response, _ = self.socket.recvfrom(1024)
        return response

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.disconnect()

    def disconnect(self) -> None:
        self.socket.close()
