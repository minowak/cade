#ifndef __AGENT_H_
#define __AGENT_H_

#include "agent_task.h"

/* Enum holding agent state. */
enum agent_state {
	STARTED,
	SUSPENDED,
	STOPPED
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
struct agent {
	char * id;																  /* ID of agent. */
	enum agent_state state;                     /* Agent state. */
	struct agent_task_node *task_list_parent;   /* Task list. */
	struct agent_service_node * agent_services; /* agent services. */
};

/* Agent description used by manager. */
struct agent_desc {
	char * agent_id; 		                         /* Agent ID providing this service. */
	struct agent_service_node * services_parent; /* List of services. */
};

/* Node for agent description list. */
struct agent_desc_node {
	struct agent_desc_node * next;
	struct * agent_desc;
};

/* Manager agent. */
struct manager_agent {
  struct agent_desc_node * agent_descs; /* Agent descriptions. */
};

int add_task_to_agent(struct agent *, struct agent_task *);
int do_task(struct agent *);

int register_agent_service(struct agent *, struct agent_service *);
int register_agent_description(struct manager_agent *, struct agent_desc *);

#endif
