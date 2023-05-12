#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint-gcc.h>

// Enum definition for commands
enum CommandType
{
   NO_COMMAND = 0x00,
   GET_X_POSITION_UM = 0x10,
   GET_Y_POSITION_UM = 0x11,
   GET_Z_POSITION_UM = 0x12,
   GET_X_SPEED_UM_S = 0x13,
   GET_Y_SPEED_UM_S = 0x14,
   GET_Z_SPEED_UM_S = 0x15,
   GET_X_ACCELERATION_uM_s2 = 0x16,
   GET_Y_ACCELERATION_uM_s2 = 0x17,
   GET_Z_ACCELERATION_uM_s2 = 0x18,
   GET_X_POWER = 0x19,
   GET_Y_POWER = 0x1a,
   GET_Z_POWER = 0x1b,
   GET_X_TEMPERATURE = 0x1c,
   GET_Y_TEMPERATURE = 0x1d,
   GET_Z_TEMPERATURE = 0x1e,
   SET_X_POSITION_UM = 0x20,
   SET_Y_POSITION_UM = 0x21,
   SET_Z_POSITION_UM = 0x22,
   SET_X_SPEED_UM_S = 0x23,
   SET_Y_SPEED_UM_S = 0x24,
   SET_Z_SPEED_UM_S = 0x25,
   SET_X_ACCELERATION_UM_S2 = 0x26,
   SET_Y_ACCELERATION_UM_S2 = 0x27,
   SET_Z_ACCELERATION_UM_S2 = 0x28,
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
