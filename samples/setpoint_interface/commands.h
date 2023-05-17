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
   GET_X_ACCELERATION_UM_S2 = 0x16,
   GET_Y_ACCELERATION_UM_S2 = 0x17,
   GET_Z_ACCELERATION_UM_S2 = 0x18,
   GET_X_POWER = 0x19,
   GET_Y_POWER = 0x1a,
   GET_Z_POWER = 0x1b,
   GET_X_TEMPERATURE = 0x1c,
   GET_Y_TEMPERATURE = 0x1d,
   GET_Z_TEMPERATURE = 0x1e,
   GET_X_TRAJECTORY_POINT = 0x20,
   GET_Y_TRAJECTORY_POINT = 0x21,
   GET_Z_TRAJECTORY_POINT = 0x22,

   SET_X_POSITION_UM = 0x40,
   SET_Y_POSITION_UM = 0x41,
   SET_Z_POSITION_UM = 0x42,
   SET_X_SPEED_UM_S = 0x43,
   SET_Y_SPEED_UM_S = 0x44,
   SET_Z_SPEED_UM_S = 0x45,
   SET_X_ACCELERATION_UM_S2 = 0x46,
   SET_Y_ACCELERATION_UM_S2 = 0x47,
   SET_Z_ACCELERATION_UM_S2 = 0x48,
   SET_X_STATE = 0x49,
   SET_Y_STATE = 0x4a,
   SET_Z_STATE = 0x4b,
   SET_X_TRAJECTORY_POINT = 0x50,
   SET_Y_TRAJECTORY_POINT = 0x51,
   SET_Z_TRAJECTORY_POINT = 0x52,

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
