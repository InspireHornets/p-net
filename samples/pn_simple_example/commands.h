#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint-gcc.h>

// Enum definition for commands
enum CommandType
{
   GET, // Command to get motor value
   SET, // Command to set motor value
   INVALID_COMMAND
};

// Enum definition for motor coordinates
enum Coordinate
{
   X,
   Y,
   Z,
   INVALID_COORDINATE
};

// Struct definition
struct Command
{
   enum CommandType type;      // Enum representing the command type
   enum Coordinate coordinate; // Enum representing the motor coordinate
   int32_t num;                // Signed 32-bit integer
};

// Function to parse input string and convert to Command struct
struct Command parseCommand (const char * input);

const char * commandTypeToString (enum CommandType cmd);

const char * coordinateToString (enum Coordinate coord);

#endif // COMMANDS_H
