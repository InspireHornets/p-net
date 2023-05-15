#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "commands.h"
#include "app_data.h"
#include "app_log.h"
#include "sampleapp_common.h"

// TODO review this implementaiton with Thomas
void respond (
   const uint8_t * response,
   int response_size,
   struct sockaddr_in client_addr,
   int socket_desc)
{
   socklen_t client_struct_length = sizeof (client_addr);

   uint8_t server_message[APP_UDP_MESSAGE_LENGTH];
   memset (server_message, 0, sizeof (server_message));
   memcpy (server_message, response, response_size);

   sendto (
      socket_desc,
      server_message,
      5,
      MSG_DONTWAIT,
      (struct sockaddr *)&client_addr,
      client_struct_length);
}

// TODO review this implementaiton with Thomas
// Function to parse input string and convert to Command struct
void handle_command (
   const uint8_t * input,
   struct sockaddr_in client_addr,
   int socket_desc)
{
   union Unint32 plc_output;
   union Unint32 plc_input;
   uint8_t buffer[5];

   APP_LOG_DEBUG ("UDP server: received command %x\n", input[0]);

   switch (input[0])
   {
   case NO_COMMAND:
      break;
   case GET_X_POSITION_UM:
      plc_output = get_x_position();
      APP_LOG_DEBUG ("Current x position: %u\n", plc_output.unint32);

      buffer[0] = GET_X_POSITION_UM;
      memcpy (buffer + 1, &plc_output.bytes, 4);

      respond (buffer, sizeof (buffer), client_addr, socket_desc);
      break;
   case GET_X_SPEED_UM_S:
      plc_output = get_x_speed();
      APP_LOG_DEBUG ("Current x speed: %u\n", plc_output.unint32);

      buffer[0] = GET_X_SPEED_UM_S;
      memcpy (buffer + 1, &plc_output.bytes, 4);

      respond (buffer, sizeof (buffer), client_addr, socket_desc);
      break;
   case GET_X_ACCELERATION_UM_S2:
      plc_output = get_x_acceleration();
      APP_LOG_DEBUG ("Current x acceleration: %u\n", plc_output.unint32);

      buffer[0] = GET_X_ACCELERATION_UM_S2;
      memcpy (buffer + 1, &plc_output.bytes, 4);

      respond (buffer, sizeof (buffer), client_addr, socket_desc);
      break;
   case SET_X_POSITION_UM:
      memcpy (plc_input.bytes, input + 1, 4);
      APP_LOG_DEBUG ("New x position %u\n", plc_input.unint32);
      set_x (plc_input.unint32);
      break;
   default:
      APP_LOG_ERROR ("Invalid command: %i", input[0]);
      break;
   }
}
