#include "group.h"
#include "utils.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

void handle_message(char * sender, char * message, char * reply)
{
  // TODO
}

/* Main. */
int main(int argc, char ** argv)
{
  if(argc < 3)
  {
    printf("Usage: %s <agent_name> <group_name>\n", argv[0]);
    return 0;
  }

  char * a_name = argv[1];
  char * g_name = argv[2];

  printf("Creating listening thread\n");

  pthread_t listen_t;
  pthread_create(&listen_t, NULL, &listen_loop, (void *) a_name);

  printf("Agent '%s' started\n", a_name);
  printf("Connecting to group '%s'\n", g_name);

  char msg[150];
  sprintf(msg, "%s;%s;register", a_name, g_name);

  printf("Registering agent [%s]...\n", msg);
  send_multicast_message(msg);

  while(1)
  {
    char buff[100];

    if(get_line(PROMPT, buff, sizeof(buff)) == OK)
    {
      if(strcmp(buff, "q") == 0)
      {
        printf("Exitting...\n");
        return 0;
      }
      // TODO
    }
  }
}
