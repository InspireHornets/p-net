import socket
import struct
from enum import Enum
from typing import Tuple

from hat.ddd import ValueObject


class CommandType(Enum):
    NO_COMMAND = 0x00

    GET_X_POWER = 0x19
    GET_Y_POWER = 0x1A
    GET_Z_POWER = 0x1B
    GET_X_TEMPERATURE = 0x1C
    GET_Y_TEMPERATURE = 0x1D
    GET_Z_TEMPERATURE = 0x1E
    GET_X_TRAJECTORY_POINT = 0x20
    GET_Y_TRAJECTORY_POINT = 0x21
    GET_Z_TRAJECTORY_POINT = 0x22
    GET_XYZ_TRAJECTORY_POINT = 0x30

    SET_X_STATE = 0x49
    SET_Y_STATE = 0x4A
    SET_Z_STATE = 0x4B
    SET_X_TRAJECTORY_POINT = 0x50
    SET_Y_TRAJECTORY_POINT = 0x51
    SET_Z_TRAJECTORY_POINT = 0x52
    SET_XYZ_TRAJECTORY_POINT = 0x60

    INVALID_COMMAND = 0xFF


def get_command_return_values(command_type: CommandType) -> int:
    """Return the number of ints returned by get commands. X, Y, Z trajectory points are 3, XYZ is 9, other are 1."""
    if command_type == CommandType.GET_XYZ_TRAJECTORY_POINT:
        return 9
    elif command_type == CommandType.GET_X_TRAJECTORY_POINT:
        return 3
    elif command_type == CommandType.GET_Y_TRAJECTORY_POINT:
        return 3
    elif command_type == CommandType.GET_Z_TRAJECTORY_POINT:
        return 3
    else:
        return 1


um = int
um_s = int
um_s2 = int


class TrajectoryPoint(ValueObject):
    position: um
    speed: um_s
    acceleration: um_s2


class CartesianState(ValueObject):
    x: TrajectoryPoint
    y: TrajectoryPoint
    z: TrajectoryPoint


def set_command(command: CommandType, *setpoints: int) -> bytes:
    command_type = struct.pack("<B", command.value)
    command_data = struct.pack("<" + "i" * len(setpoints), *setpoints)

    return command_type + command_data


# TODO when calling plc.get_x_temperature() followed by plc.get_x_power(), it gives an assertion error after the
# second command. You need to call plc.get_x_power() a few times before the pn device returns CommandType GET_X_POWER.
class SetpointClient:
    def __init__(self, host: str, port: int):
        self.host = host
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def get_command(self, command_type: CommandType) -> Tuple[int]:
        command = struct.pack(">B", command_type.value)
        self._send(command)

        return_value_size = get_command_return_values(command_type)
        format = "<B" + ("i" * return_value_size)
        answer = struct.unpack(format, self._receive())
        assert answer[0] == command_type.value, f"PLC returned {CommandType(answer[0])}, but expected {command_type}."

        return answer[1:]

    def get_x_power(self) -> int:
        return self.get_command(CommandType.GET_X_POWER)[0]

    def get_x_temperature(self) -> int:
        return self.get_command(CommandType.GET_X_TEMPERATURE)[0]

    def get_x_trajectory(self) -> TrajectoryPoint:
        trajectory = self.get_command(CommandType.GET_X_TRAJECTORY_POINT)

        return TrajectoryPoint(position=trajectory[0], speed=trajectory[1], acceleration=trajectory[2])

    def set_x_trajectory(self, trajectory_point: TrajectoryPoint) -> None:
        command = set_command(
            CommandType.SET_X_TRAJECTORY_POINT,
            trajectory_point.position,
            trajectory_point.speed,
            trajectory_point.acceleration,
        )
        self._send(command)

    def get_y_power(self) -> int:
        return self.get_command(CommandType.GET_Y_POWER)[0]

    def get_y_temperature(self) -> int:
        return self.get_command(CommandType.GET_Y_TEMPERATURE)[0]

    def get_y_trajectory(self) -> TrajectoryPoint:
        trajectory = self.get_command(CommandType.GET_Y_TRAJECTORY_POINT)

        return TrajectoryPoint(position=trajectory[0], speed=trajectory[1], acceleration=trajectory[2])

    def set_y_trajectory(self, trajectory_point: TrajectoryPoint) -> None:
        command = set_command(
            CommandType.SET_Y_TRAJECTORY_POINT,
            trajectory_point.position,
            trajectory_point.speed,
            trajectory_point.acceleration,
        )
        self._send(command)

    def get_xyz_trajectory(self) -> CartesianState:
        xyz_trajectory = self.get_command(CommandType.GET_XYZ_TRAJECTORY_POINT)

        x_point = TrajectoryPoint(position=xyz_trajectory[0], speed=xyz_trajectory[1], acceleration=xyz_trajectory[2])
        y_point = TrajectoryPoint(position=xyz_trajectory[3], speed=xyz_trajectory[4], acceleration=xyz_trajectory[5])
        z_point = TrajectoryPoint(position=xyz_trajectory[6], speed=xyz_trajectory[7], acceleration=xyz_trajectory[8])

        return CartesianState(x=x_point, y=y_point, z=z_point)

    def set_xyz_trajectory(self, robot_state: CartesianState) -> None:
        command = set_command(
            CommandType.SET_XYZ_TRAJECTORY_POINT,
            robot_state.x.position,
            robot_state.x.speed,
            robot_state.x.acceleration,
            robot_state.y.position,
            robot_state.y.speed,
            robot_state.y.acceleration,
            robot_state.z.position,
            robot_state.z.speed,
            robot_state.z.acceleration,
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
