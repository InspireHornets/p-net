import socket
import struct
from unittest import mock
from unittest.mock import patch

from assertpy import assert_that
from client import CommandType, SetpointClient


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


def test_get_x():
    with mock.patch("socket.socket") as mock_socket:
        mock_socket.return_value.recv.return_value = (12, struct.pack("<B", 0x00))
        client = SetpointClient("127.0.0.1", 12345)
        actual = client.get_x_position()

    client.socket.connect.assert_called_with(struct.pack("<B", CommandType.GET_X_POSITION_UM.value))
    client.socket.connect.assert_called_with(("127.0.0.1", 12345))
    assert_that(actual).is_equal_to(123)


def test_with_magic_methods():
    with SetpointClient("127.0.0.1", 12345) as client:
        client.set_x_position(123)

    assert_that(client.set_x_position).raises(OSError).when_called_with(123)
