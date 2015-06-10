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
agent_service_t services[MAXAGENTS];
int services_number = 0;

/* Checks if agent is already registered. */
int has_agent(char * a_name)
{
  int i;
  for(i = 0 ; i < MAXAGENTS ; i = i + 1)
  {
    if(agents[i] != NULL && strcmp(agents[i], a_name) == 0)
    {
      return 1;
    }
  }
  return 0;
}

/* Creates response for message. */
void handle_message(char * sender, char * message, char * reply)
{
  memset(reply, 0, strlen(reply));
  if(strcmp(message, "register") == 0 && !has_agent(sender))
  {
    agents[agents_number] = (char *) malloc(sizeof(sender));
    strcpy(agents[agents_number], sender);
    agents_number = agents_number + 1;
    printf("Registered new agent: %s\n", sender);
    sprintf(reply, "%s;%s;register-ok", g_name, sender);
  } else
  if(starts_with("service", message))
  {
    printf("Registering service %s for agent %s\n", message, sender);

    char type[50];
    char name[50];

    sscanf(message, "service-%[^-]-%[^-]", type, name);
    services[services_number].agent_id = sender;
    services[services_number].type = type;
    services[services_number].name = name;
    services_number++;
  }
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
      } else
      if(strcmp(buff, "s") == 0)
      {
        int i;
        printf("Asking for agents status[%d]...\n", agents_number);
        for(i = 0 ; i < agents_number ; i++)
        {
          char msg[150];
          sprintf(msg, "%s;%s;status", g_name, agents[i]);
          printf("Sending status request to %s\n", agents[i]);
          send_multicast_message(msg);
        }
      } else
      if(strcmp(buff, "a") == 0)
      {
        printf("Agent list:\n");
        int i;
        for(i = 0 ; i < agents_number ; i++)
        {
          printf("%d) %s\n", i+1, agents[i]);
        }
      }
    }
  }
}
