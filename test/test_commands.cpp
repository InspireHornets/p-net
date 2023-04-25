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

   // Helper function to check string representation of coordinate
   static void testCoordinateToString (
      enum Coordinate coord,
      const char * expectedStr)
   {
      const char * str = coordinateToString (coord);
      ASSERT_STREQ (str, expectedStr);
   }

   // Helper function to check string representation of command
   static void testCommandToString (CommandType type, const char * expectedStr)
   {
      const char * str = commandTypeToString (type);
      ASSERT_STREQ (str, expectedStr);
   }
};

// Tests for commands
TEST_F (CommandTest, TestGetCommand)
{
   testCommand ("get X 423", GET, 423);
}

TEST_F (CommandTest, TestSetCommand)
{
   testCommand ("set X -123", SET, -123);
}

TEST_F (CommandTest, TestInvalidCommand)
{
   struct Command command = parseCommand ("invalid_input");
   ASSERT_EQ (command.type, INVALID_COMMAND);
}

// Tests for coordinateToString()
TEST_F (CommandTest, TestCoordinateToStringX)
{
   testCoordinateToString (X, "X");
}

TEST_F (CommandTest, TestCoordinateToStringY)
{
   testCoordinateToString (Y, "Y");
}

TEST_F (CommandTest, TestCoordinateToStringZ)
{
   testCoordinateToString (Z, "Z");
}

TEST_F (CommandTest, TestCoordinateToStringInvalid)
{
   testCoordinateToString (INVALID_COORDINATE, "INVALID_COORDINATE");
}

// Tests for commandToString()
TEST_F (CommandTest, TestCommandToStringGet)
{
   testCommandToString (GET, "get");
}

TEST_F (CommandTest, TestCommandToStringSet)
{
   testCommandToString (SET, "set");
}
