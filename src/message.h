#ifndef __MESSAGE_H__
#define __MESSEGE_H__

struct message {
  char * sender;    /* Agent ID to whom this message is sent. */
  char * receiver;  /* Agent ID who sent this message. */
  char * content;   /* Message content. */
};

int send_message(struct message *);

#endif
