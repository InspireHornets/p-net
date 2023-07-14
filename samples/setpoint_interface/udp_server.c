#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "udp_server.h"
#include "app_log.h"

int open_socket (const char * host_address, int port)
{
   struct sockaddr_in server_addr;
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons (port);
   server_addr.sin_addr.s_addr = inet_addr (host_address);

   // Create UDP socket:
   int socket_desc = socket (server_addr.sin_family, SOCK_DGRAM, IPPROTO_UDP);

   if (socket_desc < 0)
   {
      APP_LOG_ERROR ("UDP server:Error while creating socket\n");
      close (socket_desc);

      return -1;
   }
   APP_LOG_INFO ("UDP server: Socket created successfully\n");

   // Bind to the set port and IP:
   if (bind (socket_desc, (struct sockaddr *)&server_addr, sizeof (server_addr)) < 0)
   {
      APP_LOG_ERROR ("UDP server: Couldn't bind to port %i\n", port);
      close (socket_desc);

      return -1;
   }

   APP_LOG_INFO (
      "UDP server: Listening for incoming messages on %s:%i\n\n",
      host_address,
      port);

   return socket_desc;
}
