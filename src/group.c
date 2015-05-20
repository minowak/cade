#include "group.h"
#include "utils.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

/* Globals. */
char ** agents; /* Agents holder. */
int agents_number = 0; /* Number of agents. */
char * g_name; /* Group name. */

/* Checks if agent is already registered. */
int has_agent(char * a_name)
{
  int i;
  for(i = 0 ; i < MAXAGENTS ; i = i + 1)
  {
    if(strcmp(agents[i], a_name) == 0)
    {
      return 1;
    }
  }
  return 0;
}

/* Creates response for message. */
void handle_message(char * sender, char * message, char * reply)
{
  sprintf(reply, "%s;%s;error", g_name, sender);
  if(strcmp(message, "register") == 0 && !has_agent(sender))
  {
    agents[agents_number] = (char *) malloc(sizeof(sender));
    strcpy(agents[agents_number], sender);
    agents_number = agents_number + 1;
    printf("Registered new agent: %s", sender);
  }
  sprintf(reply, "%s;%s;ok", g_name, sender);
}

/* Main. */
int main(int argc, char ** argv)
{
  if(argc < 2)
  {
    printf("Usage: %s <group_name>\n", argv[0]);
    return 0;
  }

  g_name = argv[1];
  agents = (char **) malloc(MAXAGENTS * sizeof(char *));

  printf("Group '%s' started\n", g_name);
  printf("Creating listening thread\n");

  pthread_t listen_t;
  pthread_create(&listen_t, NULL, &listen_loop, (void *)g_name);

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
