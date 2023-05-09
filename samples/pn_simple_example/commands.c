#include "commands.h"
#include "utils.h"
#include "app_data.h"
#include "app_log.h"

// Function to parse input string and convert to Command struct
struct Command parse_command (const uint8_t * input)
{
   // TODO remove struct
   struct Command cmd;

   switch (input[0])
   {
   case NO_COMMAND:
      cmd.type = NO_COMMAND;
      break;
   case GET_X_POSITION_UM:
      cmd.type = GET_X_POSITION_UM;
      cmd.num = get_x(); // TODO write to UDP buffer directly
      break;
   case SET_X_POSITION_UM:
      cmd.type = SET_X_POSITION_UM;
      cmd.num = combine_bytes_to_uint32 (&input[1]);
      set_x (cmd.num);
      break;
   default:
      cmd.type = INVALID_COMMAND;
      APP_LOG_ERROR ("Invalid command: %i", input[0]);
      break;
   }

   return cmd;
}
