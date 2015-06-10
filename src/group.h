#ifndef __GROUP_C_
#define __GROUP_C_

#include "agent.h"

#define MAX_AGENTS 20

struct group_t {
  char * id;                            /* ID of group. */
  struct agent_t * agents[MAX_AGENTS];    /* Agents array. */
  struct manager_agent * manager_agent; /* Manager agent. */
};

#endif
