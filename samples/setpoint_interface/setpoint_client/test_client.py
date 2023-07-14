import socket
import struct
from unittest.mock import patch

from assertpy import assert_that
from client import CartesianState, CommandType, SetpointClient, TrajectoryPoint, get_command, set_command


def test_set_command():
    actual_command = set_command(CommandType.GET_X_POWER, 42)
    expected_command = struct.pack("<B", CommandType.GET_X_POWER.value) + struct.pack("<I", 42)

    assert_that(actual_command).is_equal_to(expected_command)


def test_get_command():
    actual_command = get_command(CommandType.GET_X_TEMPERATURE)
    expected_command = struct.pack(">B", CommandType.GET_X_TEMPERATURE.value)

    assert_that(actual_command).is_equal_to(expected_command)


@patch("socket.socket")
def test_set_x_trajectory_point(mock_socket):
    with SetpointClient("127.0.0.1", 12345) as client:
        trajectory = TrajectoryPoint(position=123, speed=35, acceleration=98)
        client.set_x_trajectory(trajectory)

        mock_socket.assert_called_once_with(socket.AF_INET, socket.SOCK_DGRAM)
        mock_socket.return_value.sendto.assert_called_once()

        expected_command = struct.pack("<Biii", CommandType.SET_X_TRAJECTORY_POINT.value, 123, 35, 98)
        mock_socket.return_value.sendto.assert_called_with(expected_command, ("127.0.0.1", 12345))


@patch("socket.socket")
def test_get_x_power(mock_socket):
    # Mock the send and receive methods of the socket
    mock_send = mock_socket.return_value.sendto
    mock_receive = mock_socket.return_value.recvfrom

    # Set the expected response from the PLC
    expected_response = struct.pack("<Bi", CommandType.GET_X_POWER.value, 123)
    mock_receive.return_value = (expected_response, None)

    # Create the client and call the method
    with SetpointClient("localhost", 1234) as client:
        x_power = client.get_x_power()

        # Check that the socket methods were called with the expected commands
        mock_send.assert_called_once_with(struct.pack(">B", CommandType.GET_X_POWER.value), ("localhost", 1234))
        mock_receive.assert_called_once_with(1024)

        assert_that(x_power).is_equal_to(123)


@patch("socket.socket")
def test_get_x_temperature(mock_socket):
    # Mock the send and receive methods of the socket
    mock_send = mock_socket.return_value.sendto
    mock_receive = mock_socket.return_value.recvfrom

    # Set the expected response from the PLC
    expected_response = struct.pack("<Bi", CommandType.GET_X_TEMPERATURE.value, 123)
    mock_receive.return_value = (expected_response, None)

    # Create the client and call the method
    with SetpointClient("localhost", 1234) as client:
        x_temp = client.get_x_temperature()

        # Check that the socket methods were called with the expected commands
        mock_send.assert_called_once_with(struct.pack(">B", CommandType.GET_X_TEMPERATURE.value), ("localhost", 1234))
        mock_receive.assert_called_once_with(1024)

        assert_that(x_temp).is_equal_to(123)


@patch("socket.socket")
def test_get_x_trajectory(mock_socket):
    # Mock the send and receive methods of the socket
    mock_send = mock_socket.return_value.sendto
    mock_receive = mock_socket.return_value.recvfrom

    # Set the expected response from the PLC
    expected_response = struct.pack("<Biii", CommandType.GET_X_TRAJECTORY_POINT.value, 123, 456, 789)
    mock_receive.return_value = (expected_response, None)

    # Create the client and call the method
    with SetpointClient("localhost", 1234) as client:
        x_trajectory = client.get_x_trajectory()

        # Check that the socket methods were called with the expected commands
        mock_send.assert_called_once_with(
            struct.pack(">B", CommandType.GET_X_TRAJECTORY_POINT.value), ("localhost", 1234)
        )
        mock_receive.assert_called_once_with(1024)

        assert_that(x_trajectory.dict()).is_equal_to({"position": 123, "speed": 456, "acceleration": 789})


@patch("socket.socket")
def test_set_y_trajectory_point(mock_socket):
    with SetpointClient("127.0.0.1", 12345) as client:
        trajectory = TrajectoryPoint(position=123, speed=35, acceleration=98)
        client.set_y_trajectory(trajectory)

        mock_socket.assert_called_once_with(socket.AF_INET, socket.SOCK_DGRAM)
        mock_socket.return_value.sendto.assert_called_once()

        expected_command = struct.pack("<Biii", CommandType.SET_Y_TRAJECTORY_POINT.value, 123, 35, 98)
        mock_socket.return_value.sendto.assert_called_with(expected_command, ("127.0.0.1", 12345))


@patch("socket.socket")
def test_get_y_power(mock_socket):
    mock_send = mock_socket.return_value.sendto
    mock_receive = mock_socket.return_value.recvfrom

    expected_response = struct.pack("<Bi", CommandType.GET_Y_POWER.value, 123)
    mock_receive.return_value = (expected_response, None)

    with SetpointClient("localhost", 1234) as client:
        y_power = client.get_y_power()

        mock_send.assert_called_once_with(struct.pack(">B", CommandType.GET_Y_POWER.value), ("localhost", 1234))
        mock_receive.assert_called_once_with(1024)

        assert_that(y_power).is_equal_to(123)


@patch("socket.socket")
def test_get_y_temperature(mock_socket):
    mock_send = mock_socket.return_value.sendto
    mock_receive = mock_socket.return_value.recvfrom

    expected_response = struct.pack("<Bi", CommandType.GET_Y_TEMPERATURE.value, 123)
    mock_receive.return_value = (expected_response, None)

    with SetpointClient("localhost", 1234) as client:
        y_temp = client.get_y_temperature()

        mock_send.assert_called_once_with(struct.pack(">B", CommandType.GET_Y_TEMPERATURE.value), ("localhost", 1234))
        mock_receive.assert_called_once_with(1024)

        assert_that(y_temp).is_equal_to(123)


@patch("socket.socket")
def test_get_y_trajectory(mock_socket):
    mock_send = mock_socket.return_value.sendto
    mock_receive = mock_socket.return_value.recvfrom

    expected_response = struct.pack("<Biii", CommandType.GET_Y_TRAJECTORY_POINT.value, 123, 456, 789)
    mock_receive.return_value = (expected_response, None)

    with SetpointClient("localhost", 1234) as client:
        y_trajectory = client.get_y_trajectory()

        mock_send.assert_called_once_with(
            struct.pack(">B", CommandType.GET_Y_TRAJECTORY_POINT.value), ("localhost", 1234)
        )
        mock_receive.assert_called_once_with(1024)

        assert_that(y_trajectory.dict()).is_equal_to({"position": 123, "speed": 456, "acceleration": 789})


@patch("socket.socket")
def test_set_xyz_trajectory(mock_socket):
    with SetpointClient("127.0.0.1", 12345) as client:
        trajectory_point = CartesianState(
            x=TrajectoryPoint(position=100, speed=10, acceleration=5),
            y=TrajectoryPoint(position=200, speed=20, acceleration=15),
            z=TrajectoryPoint(position=300, speed=30, acceleration=25),
        )
        client.set_xyz_trajectory(trajectory_point)

        mock_socket.assert_called_once_with(socket.AF_INET, socket.SOCK_DGRAM)
        mock_socket.return_value.sendto.assert_called_once()

        expected_command = struct.pack(
            "<Biiiiiiiii", CommandType.SET_XYZ_TRAJECTORY_POINT.value, 100, 10, 5, 200, 20, 15, 300, 30, 25
        )
        mock_socket.return_value.sendto.assert_called_with(expected_command, ("127.0.0.1", 12345))


@patch("socket.socket")
def test_get_command_with_wrong_return_command(mock_socket):
    # Mock the send and receive methods of the socket
    mock_send = mock_socket.return_value.sendto
    mock_receive = mock_socket.return_value.recvfrom

    # Set the expected response from the PLC
    actual_response = struct.pack("<Biii", CommandType.GET_Y_TRAJECTORY_POINT.value, 123, 456, 78)
    mock_receive.return_value = (actual_response, None)

    # Create the client and call the method
    with SetpointClient("localhost", 1234) as client:
        assert_that(client.get_x_trajectory).raises(AssertionError).when_called_with().is_equal_to(
            "PLC returned CommandType.GET_Y_TRAJECTORY_POINT, but expected CommandType.GET_X_TRAJECTORY_POINT."
        )

        # Check that the socket methods were called with the expected commands
        mock_send.assert_called_once_with(
            struct.pack(">B", CommandType.GET_X_TRAJECTORY_POINT.value), ("localhost", 1234)
        )
        mock_receive.assert_called_once_with(1024)


def test_with_magic_methods():
    with SetpointClient("127.0.0.1", 12345) as client:
        point = TrajectoryPoint(position=12, speed=23, acceleration=98)
        client.set_x_trajectory(point)

    assert_that(client.set_x_trajectory).raises(OSError).when_called_with(point)
