#include "commands.h"
#include "utils.h"
#include "app_data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to parse input string and convert to Command struct
// TODO void hanle_command(...)
struct Command parse_command (const uint8_t * input)
{
   struct Command cmd;

   switch (input[0])
   {
   case NO_COMMAND:
      cmd.type = NO_COMMAND;
      break;
   case GET_X:
      // TODO Send command type: GET_X
      // TODO Send value: get_x()
      cmd.type = GET_X;  // TODO remove enum
      cmd.num = get_x(); // TODO write to UDP buffer
      break;
   case SET_X:
      cmd.type = SET_X;
      cmd.num = combine_bytes_to_uint32 (&input[1]);
      set_x (cmd.num);
      break;
   case SET_Y:
      cmd.type = SET_Y;
      cmd.num = combine_bytes_to_uint32 (&input[1]);
      set_y (cmd.num);
      break;
   default:
      cmd.type = INVALID_COMMAND; // TODO use app log to log error
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
