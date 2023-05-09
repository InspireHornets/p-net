import struct

from assertpy import assert_that

import client
from client import CommandType

def test_set_x():
    expected_result = struct.pack(">BI", CommandType.SET_X_POSITION_UM.value, 0x00)
    assert_that(client.set_x(0)).is_equal_to(expected_result)

    expected_result = struct.pack(">BI", CommandType.SET_X_POSITION_UM.value, 0xFFFFFFFF)
    assert_that(client.set_x(4294967295)).is_equal_to(expected_result)

    expected_result = struct.pack(">BI", CommandType.SET_X_POSITION_UM.value, 0x80000000)
    assert_that(client.set_x(2147483648)).is_equal_to(expected_result)
