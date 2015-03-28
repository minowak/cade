#ifndef __AGENT_TASK_H__
#define __AGENT_TASK_H__

/* Enum for holding task types. */
enum agent_task_type {
  ONE_TIME,
  CYCLIC,
  TIMED,
  REPEATED
};

/* Agent task structure. */
struct agent_task {
  void (*action)();               /* Action that agent neeed to do. */
  int done;                       /* Returns 1 if action is done, 0 otherwise. */
  enum agent_task_type task_type; /* Type of the task. */
};

/* Node for task list. */
struct agent_task_node {
  struct agent_task_node *next;
  struct agent_task task;
};

#endif
