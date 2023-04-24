#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint-gcc.h>

// Enum definition for commands
enum CommandType
{
   GET_X, // Command to get X
   SET_X, // Command to set X
   INVALID_COMMAND
};

// Struct definition
struct Command
{
   enum CommandType type; // Enum representing the command type
   int32_t num;           // Signed 32-bit integer
};

// Function to parse input string and convert to Command struct
struct Command parseCommand (const char * input);

const char * commandTypeToString (enum CommandType cmd);

#endif // COMMANDS_H
