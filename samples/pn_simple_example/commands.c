#include "commands.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to parse input string and convert to Command struct
struct Command parse_command (const uint8_t * input)
{
   struct Command cmd;

   switch (input[0])
   {
   case NO_COMMAND:
      cmd.type = NO_COMMAND;
      break;
   case GET_X:
      cmd.type = GET_X;
      cmd.num = 0; // TODO get_command
      break;
   case SET_X:
      cmd.type = SET_X;
      cmd.num = combine_bytes_to_uint32 (&input[1]);
      break;
   default:
      cmd.type = INVALID_COMMAND;
      break;
   }

   return cmd;
}
//
// const char * commandTypeToString (enum CommandType cmd)
//{
//   switch (cmd)
//   {
//   case GET:
//      return "get";
//   case SET:
//      return "set";
//   case INVALID_COMMAND:
//      return "invalid command";
//   default:
//      return "unknown";
//   }
//}
