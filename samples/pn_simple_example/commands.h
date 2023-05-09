#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint-gcc.h>

// Enum definition for commands
// TODO change to GET_X, and numbering
enum CommandType
{
   NO_COMMAND = 0x00,
   GET_X = 0x10,
   GET_Y = 0x11,
   GET_Z = 0x12,
   SET_X = 0x20,
   SET_Y = 0x21,
   SET_Z = 0x22,
   INVALID_COMMAND = 0xff,
};

// Struct definition
struct Command
{
   enum CommandType type; // Enum representing the command type
   uint32_t num;          // Signed 32-bit integer
};

// Function to parse input string and convert to Command struct
struct Command handle_command (const uint8_t * input);

// const char * commandTypeToString (enum CommandType cmd);

#endif // COMMANDS_H
