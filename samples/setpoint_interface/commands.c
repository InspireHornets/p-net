#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "commands.h"
#include "app_data.h"
#include "app_log.h"
#include "sampleapp_common.h"

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
   app_actual3_data_t actual3;
   app_setpoint3_data_t setpoint3 = {
      .x = {0}, // Initialize x members to 0
      .y = {0}, // Initialize y members to 0
      .z = {0}  // Initialize z members to 0
   };
   uint8_t buffer[APP_UDP_MESSAGE_LENGTH];
   int COMMAND_SIZE = 1;

   APP_LOG_DEBUG ("UDP server: received command %x\n", input[0]);

   switch (input[0])
   {
   case NO_COMMAND:
      break;
   case GET_XYZ_TRAJECTORY_POINT:
      actual3 = get_xyz_trajectory();
      APP_LOG_DEBUG (
         "Current x position: %i, x speed: %i, x acceleration: %i\n",
         actual3.x.position_um,
         actual3.x.speed_um_s,
         actual3.x.acceleration_um_s2);
      APP_LOG_DEBUG (
         "Current y position: %i, y speed: %i, y acceleration: %i\n",
         actual3.y.position_um,
         actual3.y.speed_um_s,
         actual3.y.acceleration_um_s2);
      APP_LOG_DEBUG (
         "Current z position: %i, z speed: %i, z acceleration: %i\n",
         actual3.z.position_um,
         actual3.z.speed_um_s,
         actual3.z.acceleration_um_s2);

      buffer[0] = GET_XYZ_TRAJECTORY_POINT;
      memcpy (buffer + 1, &actual3, 3 * APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE);

      respond (
         buffer,
         COMMAND_SIZE + 3 * APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE,
         client_addr,
         socket_desc);
      break;
   case SET_XYZ_TRAJECTORY_POINT:
      memcpy (&setpoint3, input + 1, 3 * APP_GSDML_INPUT_DATA_SETPOINT_SIZE);
      APP_LOG_DEBUG (
         "New x position %i, x speed %i, x acceleration: %i\n",
         setpoint3.x.position_um,
         setpoint3.x.speed_um_s,
         setpoint3.x.acceleration_um_s2);
      APP_LOG_DEBUG (
         "New y position %i, y speed %i, y acceleration: %i\n",
         setpoint3.y.position_um,
         setpoint3.y.speed_um_s,
         setpoint3.y.acceleration_um_s2);
      APP_LOG_DEBUG (
         "New z position %i, z speed %i, z acceleration: %i\n",
         setpoint3.z.position_um,
         setpoint3.z.speed_um_s,
         setpoint3.z.acceleration_um_s2);
      set_xyz_trajectory_point (setpoint3);
      break;
   default:
      APP_LOG_ERROR ("Invalid command: %i", input[0]);
      break;
   }
}
