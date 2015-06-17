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
char ** agent_args; /* Arguments passed to agent on startup. May be used to configure execution of a task. */

void handle_message(char * sender, char * message, char * reply)
{
  memset(reply, 0, strlen(reply));
  if(agent.state == INITIALIZING && strcmp(message, "register-ok") == 0)
  {
    printf("Registered successfully in group %s\n", sender);
    printf("Registering service\n");

    char msg[256];
    sprintf(msg, "%s;%s;service-%s-%s", agent.id, sender, agent.service.type, agent.service.name);
    send_multicast_message(msg);

    agent.state = LISTENING;
  } else
  if(agent.state == LISTENING)
  {
    if(strcmp(message, "status") == 0)
    {
      send_status(agent, sender, reply);
    } else
    if(starts_with("start", message))
    {
      char args[50];
      sscanf(message, "start-%s", args);
      char msg[256];
      void * result = malloc(256);
      if(do_task(args, result))
      {
        sprintf(msg, "%s;%s;result(%s)", agent.id, sender, (char *)result);
      } else
      {
        sprintf(msg, "%s;%s;failed", agent.id, sender);
      }
      send_multicast_message(msg);
    }
  }
}

/* TASK BEGIN */

int task(void * args, void * result)
{
  FILE *fp;
  int status;
  char path[1024];

  fp = popen("python gen.py", "r");
  if(fp == NULL)
  {
    return -1;
  }
  while(fgets(path, 1024, fp) != NULL)
  {
    sprintf(result, "%s", path);
  }
  pclose(fp);

  return 1;
}

/* TASK END */

int do_task(void * args, void * result)
{
  if(strcmp(agent.service.type, "once") == 0)
  {
    return task(args, result);
  }
  return 0;
  // TODO rest
}

/* Main. */
int main(int argc, char ** argv)
{
  if(argc < 3)
  {
    printf("Usage: %s <agent_name> <group_name>\n", argv[0]);
    return 0;
  }

  agent_args = argv;

  agent.id = argv[1];
  agent.gid = argv[2];
  agent.state = INITIALIZING;

  /* SERVICE BEGIN */
  agent.service.agent_id = agent.id;
  agent.service.type = "once";
  agent.service.name = "s1";
  /* SERVICE END */

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
    }
  }
}
