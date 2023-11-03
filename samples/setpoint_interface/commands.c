#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
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
   app_actual4_data_t actual4;
   app_setpoint4_data_t setpoint4 = {.x = {0}, .y = {0}, .z = {0}, .c = {0}};
   uint8_t buffer[APP_UDP_MESSAGE_LENGTH];
   int COMMAND_SIZE = 1;
   struct timeval tv;

   gettimeofday (&tv, NULL);
   APP_LOG_DEBUG (
      "UDP server: received command %x at seconds %i, milliseconds %i \n",
      input[0],
      tv.tv_sec,
      tv.tv_usec / 1000);

   switch (input[0])
   {
   case NO_COMMAND:
      break;
   case GET_XYZC_TRAJECTORY_POINT:
      actual4 = get_xyzc_trajectory();
      APP_LOG_DEBUG (
         "UDP: Current x1 position: %i, x1 speed: %i, x1 acceleration: %i\n",
         actual4.x.x1_position_um,
         actual4.x.x1_speed_um_s,
         actual4.x.x1_acceleration_um_s2);
      APP_LOG_DEBUG (
         "UDP: Current y position: %i, y speed: %i, y acceleration: %i\n",
         actual4.y.position_um,
         actual4.y.speed_um_s,
         actual4.y.acceleration_um_s2);
      APP_LOG_DEBUG (
         "UDP: Current z position: %i, z speed: %i, z acceleration: %i\n",
         actual4.z.position_um,
         actual4.z.speed_um_s,
         actual4.z.acceleration_um_s2);
      APP_LOG_DEBUG (
         "UDP: Current c position: %i, c speed: %i, c acceleration: %i\n",
         actual4.c.position_um,
         actual4.c.speed_um_s,
         actual4.c.acceleration_um_s2);

      buffer[0] = GET_XYZC_TRAJECTORY_POINT;
      memcpy (
         buffer + 1,
         &actual4,
         3 * APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE +
            APP_GSDML_OUTPUT_DATA_SETPOINT_X_SIZE);

      respond (
         buffer,
         COMMAND_SIZE + 3 * APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE +
            APP_GSDML_OUTPUT_DATA_SETPOINT_X_SIZE,
         client_addr,
         socket_desc);
      break;
   case SET_XYZC_TRAJECTORY_POINT:
      memcpy (&setpoint4, input + 1, 4 * APP_GSDML_INPUT_DATA_SETPOINT_SIZE);
      APP_LOG_DEBUG (
         "UDP: New x position %i, x speed %i, x acceleration: %i\n",
         setpoint4.x.position_um,
         setpoint4.x.speed_um_s,
         setpoint4.x.acceleration_um_s2);
      APP_LOG_DEBUG (
         "UDP: New y position %i, y speed %i, y acceleration: %i\n",
         setpoint4.y.position_um,
         setpoint4.y.speed_um_s,
         setpoint4.y.acceleration_um_s2);
      APP_LOG_DEBUG (
         "UDP: New z position %i, z speed %i, z acceleration: %i\n",
         setpoint4.z.position_um,
         setpoint4.z.speed_um_s,
         setpoint4.z.acceleration_um_s2);
      APP_LOG_DEBUG (
         "UDP: New c position %i, z speed %i, z acceleration: %i\n",
         setpoint4.c.position_um,
         setpoint4.c.speed_um_s,
         setpoint4.c.acceleration_um_s2);
      set_xyzc_trajectory_point (setpoint4);
      break;
   default:
      APP_LOG_ERROR ("Invalid command: %i", input[0]);
      break;
   }
}
