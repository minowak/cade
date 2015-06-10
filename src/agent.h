#ifndef __AGENT_H_
#define __AGENT_H_

#include "agent_task.h"
#include <stdio.h>

/* Enum holding agent state. */
enum agent_state {
	INITIALIZING,
	LISTENING,
	COMPUTING,
	PAUSED
};

/* Description of service provided by agent. */
typedef struct agent_service {
	char * agent_id; /* Agent ID. */
	char * type;     /* Service type. */
	char * name;     /* Service name. */
} agent_service_t;

/* Agent struct. */
typedef struct agent {
	char * id;																  /* ID of agent. */
	char * gid;																	/* Group ID. */
	enum agent_state state;                     /* Agent state. */
	agent_service_t service;									/* Agent service. */
} agent_t;

int send_status(agent_t agent, char * sender, char * reply)
{
	switch(agent.state)
	{
		case INITIALIZING:
			sprintf(reply, "%s;%s;%s", agent.id, sender, "status-INITIALIZING");
			break;
		case LISTENING:
			sprintf(reply, "%s;%s;%s", agent.id, sender, "status-LISTENING");
			break;
		case COMPUTING:
			sprintf(reply, "%s;%s;%s", agent.id, sender, "status-COMPUTING");
			break;
		case PAUSED:
			sprintf(reply, "%s;%s;%s", agent.id, sender, "status-PAUSED");
			break;
		default:
			sprintf(reply, "%s;%s;%s", agent.id, sender, "status-error");
			break;
	}
}

#endif
