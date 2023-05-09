#include <gtest/gtest.h>
#include "utils_for_testing.h"

extern "C" {
#include "pn_simple_example/commands.h"
}

// Test fixture for commands
// TODO how to mock UDP and p-net traffic when parse_command handles the
// command directly?
class CommandTest : public PnetUnitTest
{
 protected:
   // Helper function to parse command and check type and num values
   static void testCommand (
      uint8_t * input,
      CommandType expectedType,
      uint32_t expectedNum)
   {
      struct Command command = parse_command (input);
      ASSERT_EQ (command.type, expectedType);
      ASSERT_EQ (command.num, expectedNum);
   }
};

// Tests for commands
TEST_F (CommandTest, TestGetCommand)
{
   uint8_t input[3] = {GET_X_POSITION_UM, 23, 255};
   testCommand (input, GET_X_POSITION_UM, 0);
}

TEST_F (CommandTest, TestSetCommand)
{
   uint8_t input[5] = {SET_X_POSITION_UM, 0, 0, 6, 255};
   testCommand (input, SET_X_POSITION_UM, 1791);
}
