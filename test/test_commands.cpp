#include <gtest/gtest.h>
#include "utils_for_testing.h"

extern "C" {
#include "pn_simple_example/commands.h"
}

// Test fixture for commands
// TODO how to mock UDP and p-net traffic when handle_command handles the
// command directly?
class CommandTest : public PnetUnitTest
{
 protected:
   // Helper function to parse command and check type and num values
   //   static void testCommand (
   //      uint8_t * input,
   //      CommandType expectedType,
   //      uint32_t expectedNum)
   //   {
   //      struct Command command = handle_command (input);
   //      ASSERT_EQ (command.type, expectedType);
   //      ASSERT_EQ (command.num, expectedNum);
   //   }
};
