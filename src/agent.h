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
struct agent_service {
	char * type;     /* Service type. */
	char * name;     /* Service name. */
};

/* Node of agent services list. */
struct agent_service_node {
	struct agent_service_node * next;
	struct agent_service * desc;
};

/* Agent struct. */
typedef struct agent {
	char * id;																  /* ID of agent. */
	char * gid;																	/* Group ID. */
	enum agent_state state;                     /* Agent state. */
	struct agent_task_node *task_list_parent;   /* Task list. */
	struct agent_service_node * agent_services; /* agent services. */
} agent_t;

/* Agent description used by manager. */
struct agent_desc {
	char * agent_id; 		                         /* Agent ID providing this service. */
	struct agent_service_node * services_parent; /* List of services. */
};

/* Node for agent description list. */
struct agent_desc_node {
	struct agent_desc_node * next;
	struct agent_desc * desc;
};

/* Manager agent. */
struct manager_agent {
  struct agent_desc_node * agent_descs; /* Agent descriptions. */
};

int add_task_to_agent(agent_t *, struct agent_task *);
int do_task(agent_t *);

int register_agent_service(agent_t *, struct agent_service *);
int register_agent_description(struct manager_agent *, struct agent_desc *);

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
