#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "commands.h"
#include "app_data.h"
#include "app_log.h"

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

// Function to parse input string and convert to Command struct
void handle_command (
   const uint8_t * input,
   struct sockaddr_in client_addr,
   int socket_desc)
{
   union Sint32 plc_output;
   union Sint32 plc_input;
   app_setpoint_data_t setpoint;
   app_actual_data_t actual;
   uint8_t buffer[13];
   size_t INT32_SIZE = sizeof (int32_t);
   size_t TRAJECTORY_POINT1 = 3;
   //   size_t TRAJECTORY_POINT3 = TRAJECTORY_POINT1 * 3;

   APP_LOG_DEBUG ("UDP server: received command %x\n", input[0]);

   switch (input[0])
   {
   case NO_COMMAND:
      break;
   case GET_X_TRAJECTORY_POINT:
      actual = get_x_trajectory();
      APP_LOG_DEBUG (
         "Current x position: %i, x speed: %i, x acceleration: %i\n",
         actual.position_um,
         actual.speed_mm_min,
         actual.acceleration_mm_min2);

      buffer[0] = GET_X_TRAJECTORY_POINT;
      memcpy (buffer + 1, &actual, 12);

      respond (buffer, 13, client_addr, socket_desc);
      break;
   case GET_X_POWER:
      plc_output = get_x_power();

      APP_LOG_DEBUG ("Current x power: %u\n", plc_output.sint32);

      buffer[0] = GET_X_POWER;
      memcpy (buffer + 1, &plc_output.bytes, INT32_SIZE);

      respond (buffer, 5, client_addr, socket_desc);
      break;
   case GET_X_TEMPERATURE:
      plc_output = get_x_temperature();

      APP_LOG_DEBUG ("Current x temperature: %i\n", plc_output.sint32);

      buffer[0] = GET_X_TEMPERATURE;
      memcpy (buffer + 1, &plc_output.bytes, INT32_SIZE);

      respond (buffer, 5, client_addr, socket_desc);
      break;
   case SET_X_STATE:
      memcpy (plc_input.bytes, input + 1, INT32_SIZE);
      APP_LOG_DEBUG ("New x state %i\n", plc_input.sint32);
      set_x_state (plc_input.sint32);
      break;
   case SET_X_TRAJECTORY_POINT:
      memcpy (&setpoint, input + 1, INT32_SIZE * TRAJECTORY_POINT1);
      APP_LOG_DEBUG (
         "New x position %i, x speed %i, x acceleration: %i\n",
         setpoint.position_um,
         setpoint.speed_mm_min,
         setpoint.acceleration_mm_min2);
      set_x_trajectory_point (setpoint);
      break;
   case GET_Y_TRAJECTORY_POINT:
      actual = get_y_trajectory();
      APP_LOG_DEBUG (
         "Current y position: %i, y speed: %i, y acceleration: %i\n",
         actual.position_um,
         actual.speed_mm_min,
         actual.acceleration_mm_min2);

      buffer[0] = GET_Y_TRAJECTORY_POINT;
      memcpy (buffer + 1, &actual, INT32_SIZE * TRAJECTORY_POINT1);

      respond (buffer, 13, client_addr, socket_desc);
      break;
   case SET_Y_TRAJECTORY_POINT:
      memcpy (&setpoint, input + 1, INT32_SIZE * TRAJECTORY_POINT1);
      APP_LOG_DEBUG (
         "New y position %i, x speed %i, x acceleration: %i\n",
         setpoint.position_um,
         setpoint.speed_mm_min,
         setpoint.acceleration_mm_min2);
      set_y_trajectory_point (setpoint);
      break;
   default:
      APP_LOG_ERROR ("Invalid command: %i", input[0]);
      break;
   }
}
