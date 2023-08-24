#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint-gcc.h>

// Enum definition for commands
enum CommandType
{
   NO_COMMAND = 0x00,

   GET_X_POWER = 0x19,
   GET_Y_POWER = 0x1a,
   GET_Z_POWER = 0x1b,
   GET_X_TEMPERATURE = 0x1c,
   GET_Y_TEMPERATURE = 0x1d,
   GET_Z_TEMPERATURE = 0x1e,
   GET_X_TRAJECTORY_POINT = 0x20,
   GET_Y_TRAJECTORY_POINT = 0x21,
   GET_Z_TRAJECTORY_POINT = 0x22,
   GET_XYZ_TRAJECTORY_POINT = 0x30,

   SET_X_STATE = 0x49,
   SET_Y_STATE = 0x4a,
   SET_Z_STATE = 0x4b,
   SET_X_TRAJECTORY_POINT = 0x50,
   SET_Y_TRAJECTORY_POINT = 0x51,
   SET_Z_TRAJECTORY_POINT = 0x52,
   SET_XYZ_TRAJECTORY_POINT = 0x60,

   // TODO add one command to set and get in the same cycle, such that the p-net
   //  app can run at the same rate as the plc instead of at half the cycle time

   INVALID_COMMAND = 0xff,
};

// Struct definition
struct Command
{
   enum CommandType type; // Enum representing the command type
   uint32_t num;          // Signed 32-bit integer
};

// Function to parse input string and convert to Command struct
void handle_command (
   const uint8_t * input,
   struct sockaddr_in client_addr,
   int socket_desc);

#endif // COMMANDS_H
