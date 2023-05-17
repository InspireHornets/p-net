#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "commands.h"
#include "app_data.h"
#include "app_log.h"
#include "sampleapp_common.h"

// TODO review this implementation with Thomas
void respond (
   const uint8_t * response,
   int response_size,
   struct sockaddr_in client_addr,
   int socket_desc)
{
   socklen_t client_struct_length = sizeof (client_addr);

   uint8_t server_message[response_size];
   memset (server_message, 0, response_size);
   memcpy (server_message, response, response_size);

   sendto (
      socket_desc,
      server_message,
      response_size,
      MSG_DONTWAIT,
      (struct sockaddr *)&client_addr,
      client_struct_length);
}

// TODO review this implementation with Thomas
// Function to parse input string and convert to Command struct
void handle_command (
   const uint8_t * input,
   struct sockaddr_in client_addr,
   int socket_desc)
{
   union Unint32 plc_output;
   union Unint32 plc_input;
   app_setpoint_data_t setpoint;
   app_actual_data_t actual;
   uint8_t buffer[13];

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

      respond (buffer, 5, client_addr, socket_desc);
      break;
   case GET_X_SPEED_UM_S:
      plc_output = get_x_speed();
      APP_LOG_DEBUG ("Current x speed: %u\n", plc_output.unint32);

      buffer[0] = GET_X_SPEED_UM_S;
      memcpy (buffer + 1, &plc_output.bytes, 4);

      respond (buffer, 5, client_addr, socket_desc);
      break;
   case GET_X_ACCELERATION_UM_S2:
      plc_output = get_x_acceleration();
      APP_LOG_DEBUG ("Current x acceleration: %u\n", plc_output.unint32);

      buffer[0] = GET_X_ACCELERATION_UM_S2;
      memcpy (buffer + 1, &plc_output.bytes, 4);

      respond (buffer, 5, client_addr, socket_desc);
      break;
   case GET_X_TRAJECTORY_POINT:
      actual = get_x_trajectory();
      APP_LOG_DEBUG (
         "Current x position: %u, x speed: %u, x acceleration: %u\n",
         actual.position_um,
         actual.speed_mm_min,
         actual.acceleration_mm_min2);

      buffer[0] = GET_X_TRAJECTORY_POINT;
      memcpy (buffer + 1, &actual, 12);

      respond (buffer, 13, client_addr, socket_desc);
      break;
   case GET_X_POWER:
      plc_output = get_x_power();

      APP_LOG_DEBUG ("Current x power: %u\n", plc_output.unint32);

      buffer[0] = GET_X_POWER;
      memcpy (buffer + 1, &plc_output.bytes, 4);

      respond (buffer, 5, client_addr, socket_desc);
      break;
   case GET_X_TEMPERATURE:
      plc_output = get_x_temperature();

      APP_LOG_DEBUG ("Current x temperature: %u\n", plc_output.unint32);

      buffer[0] = GET_X_TEMPERATURE;
      memcpy (buffer + 1, &plc_output.bytes, 4);

      respond (buffer, 5, client_addr, socket_desc);
      break;
   case SET_X_POSITION_UM:
      memcpy (plc_input.bytes, input + 1, 4);
      APP_LOG_DEBUG ("New x position %u\n", plc_input.unint32);
      set_x_position (plc_input.unint32);
      break;
   case SET_X_STATE:
      memcpy (plc_input.bytes, input + 1, 4);
      APP_LOG_DEBUG ("New x state %u\n", plc_input.unint32);
      set_x_state (plc_input.unint32);
      break;
   case SET_X_TRAJECTORY_POINT:
      memcpy (&setpoint, input + 1, 4 * 3);
      APP_LOG_DEBUG (
         "New x position %u, x speed %u, x acceleration: %u\n",
         setpoint.position_um,
         setpoint.speed_mm_min,
         setpoint.acceleration_mm_min2);
      set_trajectory_point (setpoint);
      break;
   default:
      APP_LOG_ERROR ("Invalid command: %i", input[0]);
      break;
   }
}
