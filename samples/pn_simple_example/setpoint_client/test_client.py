import struct
from unittest import mock

import pytest
from assertpy import assert_that
from client import CommandType, SetpointClient

# TODO how to test this?
@pytest.skip("TODO how to test this?")
def test_set_x_position():
    with SetpointClient("127.0.0.1", 12345) as client:
        with mock.patch("client.socket") as mock_socket:
            mock_socket.return_value.recv.return_value = (12, struct.pack("<B", 0x00))
            expected_result = struct.pack(">BI", CommandType.SET_X_POSITION_UM.value, 0x00)

            expected_result = struct.pack(">BI", CommandType.SET_X_POSITION_UM.value, 0xFFFFFFFF)
            assert_that(client.set_x.position(4294967295)).is_equal_to(expected_result)

            expected_result = struct.pack(">BI", CommandType.SET_X_POSITION_UM.value, 0x80000000)
            assert_that(client.set_x(2147483648)).is_equal_to(expected_result)


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
