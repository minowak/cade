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

agent_t agent;  /* This agent. */

void handle_message(char * sender, char * message, char * reply)
{
  memset(reply, 0, strlen(reply));
  if(agent.state == INITIALIZING && strcmp(message, "register-ok") == 0)
  {
    printf("Registered successfully in group %s\n", sender);
    agent.state = LISTENING;
  } else
  if(agent.state == LISTENING)
  {
    if(strcmp(message, "status") == 0)
    {
      send_status(agent, sender, reply);
    }
  }
}

/* Main. */
int main(int argc, char ** argv)
{
  if(argc < 3)
  {
    printf("Usage: %s <agent_name> <group_name>\n", argv[0]);
    return 0;
  }

  agent.id = argv[1];
  agent.gid = argv[2];
  agent.state = INITIALIZING;

  printf("Creating listening thread\n");

  pthread_t listen_t;
  pthread_create(&listen_t, NULL, &listen_loop, (void *) agent.id);

  printf("Agent '%s' started\n", agent.id);
  printf("Connecting to group '%s'\n", agent.gid);

  char msg[256];
  sprintf(msg, "%s;%s;register", agent.id, agent.gid);

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
