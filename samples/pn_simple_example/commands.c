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
      union Unint32 x_pos = get_x_position();
      // TODO use bffer[0] = GET_X_POS and then memccpy() to copy the position
      // with memcopy(buffer + 1, position, 4)
      //      uint8_t response[5] =
      //         {GET_X_POSITION_UM, x_pos[0], x_pos[1], x_pos[2], x_pos[3]};
      //      respond (response);
      APP_LOG_DEBUG ("Get x position result: %u\n", cmd.num);
      APP_LOG_DEBUG ("As an array: %u\n", x_pos.unint32);
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
