#include <gtest/gtest.h>
#include "utils_for_testing.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "pn_simple_example/commands.h"
#ifdef __cplusplus
}
#endif


// Test fixture for commands
class CommandTest : public PnetUnitTest
{
 protected:
   // Helper function to parse command and check type and num values
   static void testCommand (
      const char * input,
      CommandType expectedType,
      int32_t expectedNum)
   {
      struct Command command = parseCommand (input);
      ASSERT_EQ (command.type, expectedType);
      ASSERT_EQ (command.num, expectedNum);
   }
};

// Tests for commands
TEST_F (CommandTest, TestGetCommand)
{
   testCommand ("get_x 423", GET_X, 423);
}

TEST_F (CommandTest, TestSetCommand)
{
   testCommand ("set_x -123", SET_X, -123);
}

TEST_F (CommandTest, TestInvalidCommand)
{
   struct Command command = parseCommand ("invalid_input");
   ASSERT_EQ (command.type, INVALID_COMMAND);
}
