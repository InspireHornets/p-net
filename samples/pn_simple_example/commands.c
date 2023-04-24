#include "commands.h" // Include the header file with struct and enum definitions
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to parse input string and convert to Command struct
struct Command parseCommand (const char * input)
{
   struct Command cmd;
   // Copy input to a buffer for parsing
   char buffer[100];
   strncpy (buffer, input, sizeof (buffer));
   buffer[sizeof (buffer) - 1] = '\0'; // Null-terminate the buffer

   // Use strtok to tokenize the buffer by whitespace
   char * token = strtok (buffer, " ");

   // Check if the first token is a valid command
   if (strcmp (token, "get_x") == 0)
   {
      cmd.type = GET_X;
   }
   else if (strcmp (token, "set_x") == 0)
   {
      cmd.type = SET_X;
   }
   else
   {
      printf ("Error: Invalid command type '%s'\n", token);
      cmd.type = INVALID_COMMAND; // Set an invalid value to indicate error
      return cmd;
   }

   // Get the next token as the integer value
   token = strtok (NULL, " ");

   // Check if the second token is a valid integer
   char * endptr;
   long num = strtol (token, &endptr, 10);
   if (*endptr != '\0')
   {
      printf ("Error: Invalid number '%s'\n", token);
      cmd.type = INVALID_COMMAND; // Set an invalid value to indicate error
      return cmd;
   }

   // Cast the long value to int32_t
   cmd.num = (int32_t)num;

   return cmd;
}
