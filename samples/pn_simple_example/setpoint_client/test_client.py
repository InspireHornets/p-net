import socket
import struct
from unittest.mock import patch

from assertpy import assert_that
from client import CommandType, SetpointClient, TrajectoryPoint, get_command, set_command


def test_set_command():
    actual_command = set_command(CommandType.GET_X_POWER, 42)
    expected_command = struct.pack("<B", CommandType.GET_X_POWER.value) + struct.pack("<I", 42)

    assert_that(actual_command).is_equal_to(expected_command)


def test_get_command():
    actual_command = get_command(CommandType.GET_X_TEMPERATURE)
    expected_command = struct.pack(">B", CommandType.GET_X_TEMPERATURE.value)

    assert_that(actual_command).is_equal_to(expected_command)


@patch("socket.socket")
def test_set_x_position(mock_socket):
    with SetpointClient("127.0.0.1", 12345) as client:
        client.set_x_position(0)

        mock_socket.assert_called_once_with(socket.AF_INET, socket.SOCK_DGRAM)
        mock_socket.return_value.sendto.assert_called_once()

        expected_command = struct.pack(">BI", CommandType.SET_X_POSITION_UM.value, 0)
        mock_socket.return_value.sendto.assert_called_with(expected_command, ("127.0.0.1", 12345))

        client.set_x_position(4294967295)
        expected_command = struct.pack(">BI", CommandType.SET_X_POSITION_UM.value, 4294967295)
        mock_socket.return_value.sendto.assert_called_with(expected_command, ("127.0.0.1", 12345))


@patch("socket.socket")
def test_set_x_trajectory_point(mock_socket):
    with SetpointClient("127.0.0.1", 12345) as client:
        trajectory = TrajectoryPoint(position=123, speed=35, acceleration=98)
        client.set_x_trajectory(trajectory)

        mock_socket.assert_called_once_with(socket.AF_INET, socket.SOCK_DGRAM)
        mock_socket.return_value.sendto.assert_called_once()

        expected_command = struct.pack("<BIII", CommandType.SET_X_TRAJECTORY_POINT.value, 123, 35, 98)
        mock_socket.return_value.sendto.assert_called_with(expected_command, ("127.0.0.1", 12345))


@patch("socket.socket")
def test_get_x_position(mock_socket):
    # Mock the send and receive methods of the socket
    mock_send = mock_socket.return_value.sendto
    mock_receive = mock_socket.return_value.recvfrom

    # Set the expected response from the PLC
    expected_response = struct.pack("<BI", CommandType.GET_X_POSITION_UM.value, 123)
    mock_receive.return_value = (expected_response, None)

    # Create the client and call the method
    with SetpointClient("localhost", 1234) as client:
        x_position = client.get_x_position()

        # Check that the socket methods were called with the expected commands
        mock_send.assert_called_once_with(struct.pack(">B", CommandType.GET_X_POSITION_UM.value), ("localhost", 1234))
        mock_receive.assert_called_once_with(1024)

        assert_that(x_position).is_equal_to(123)


@patch("socket.socket")
def test_get_x_speed(mock_socket):
    # Mock the send and receive methods of the socket
    mock_send = mock_socket.return_value.sendto
    mock_receive = mock_socket.return_value.recvfrom

    # Set the expected response from the PLC
    expected_response = struct.pack("<BI", CommandType.GET_X_SPEED_UM_S.value, 123)
    mock_receive.return_value = (expected_response, None)

    # Create the client and call the method
    with SetpointClient("localhost", 1234) as client:
        x_speed = client.get_x_speed()

        # Check that the socket methods were called with the expected commands
        mock_send.assert_called_once_with(struct.pack(">B", CommandType.GET_X_SPEED_UM_S.value), ("localhost", 1234))
        mock_receive.assert_called_once_with(1024)

        assert_that(x_speed).is_equal_to(123)


@patch("socket.socket")
def test_get_x_acceleration(mock_socket):
    # Mock the send and receive methods of the socket
    mock_send = mock_socket.return_value.sendto
    mock_receive = mock_socket.return_value.recvfrom

    # Set the expected response from the PLC
    expected_response = struct.pack("<BI", CommandType.GET_X_ACCELERATION_UM_S2.value, 123)
    mock_receive.return_value = (expected_response, None)

    # Create the client and call the method
    with SetpointClient("localhost", 1234) as client:
        x_acceleration = client.get_x_acceleration()

        # Check that the socket methods were called with the expected commands
        mock_send.assert_called_once_with(
            struct.pack(">B", CommandType.GET_X_ACCELERATION_UM_S2.value), ("localhost", 1234)
        )
        mock_receive.assert_called_once_with(1024)

        assert_that(x_acceleration).is_equal_to(123)


@patch("socket.socket")
def test_get_command_with_wrong_return_command(mock_socket):
    # Mock the send and receive methods of the socket
    mock_send = mock_socket.return_value.sendto
    mock_receive = mock_socket.return_value.recvfrom

    # Set the expected response from the PLC
    actual_response = struct.pack("<BI", CommandType.GET_X_POSITION_UM.value, 123)
    mock_receive.return_value = (actual_response, None)

    # Create the client and call the method
    with SetpointClient("localhost", 1234) as client:
        assert_that(client.get_x_acceleration).raises(AssertionError).when_called_with().is_equal_to(
            "PLC returned CommandType.GET_X_POSITION_UM, but expected CommandType.GET_X_ACCELERATION_UM_S2."
        )

        # Check that the socket methods were called with the expected commands
        mock_send.assert_called_once_with(
            struct.pack(">B", CommandType.GET_X_ACCELERATION_UM_S2.value), ("localhost", 1234)
        )
        mock_receive.assert_called_once_with(1024)


def test_with_magic_methods():
    with SetpointClient("127.0.0.1", 12345) as client:
        client.set_x_position(123)

    assert_that(client.set_x_position).raises(OSError).when_called_with(123)
