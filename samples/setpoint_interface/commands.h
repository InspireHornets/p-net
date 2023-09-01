#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint-gcc.h>

// Enum definition for commands
enum CommandType
{
   NO_COMMAND = 0x00,
   GET_XYZC_TRAJECTORY_POINT = 0x10,
   SET_XYZC_TRAJECTORY_POINT = 0x20,
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
