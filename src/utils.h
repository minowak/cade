#ifndef __utils_h__
#define __utils_h_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

#define PROMPT " >> "

#define PORT 5001
#define HOST "225.0.0.37"

#define MSGBUFSIZE 256
#define MAXAGENTS 100

static int send_multicast_message(char * msg)
{
  struct sockaddr_in g_addr;
  int fd;

  if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    printf("ERROR while creating a socket\n");
  }

  memset(&g_addr, 0, sizeof(g_addr));
  g_addr.sin_family = AF_INET;
  g_addr.sin_addr.s_addr = inet_addr(HOST);
  g_addr.sin_port = htons(PORT);

  if(sendto(fd, msg, strlen(msg), 0, (struct sockaddr *) &g_addr, sizeof(g_addr)) < 0)
  {
    printf("ERROR sending message\n");
    return 0;
  }
}

void handle_message(char * sender, char * message, char * reply);

/* Thread for istening for messages. */
void * listen_loop(void * unused)
{
  char * name = (char *) unused;
  struct sockaddr_in addr;
  int fd, n_bytes, addrlen;
  struct ip_mreq mreq;
  char msgbuf[MSGBUFSIZE];

  u_int yes = 1;

  if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    printf("ERROR setting socket\n");
    pthread_exit(NULL);
  }

  if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
  {
    printf("ERROR setting socket options\n");
    pthread_exit(NULL);
  }
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(HOST);
  addr.sin_port = htons(PORT);

  if(bind(fd, (struct sockaddr *) &addr,sizeof(addr)) < 0)
  {
    printf("ERROR while binding\n");
    pthread_exit(NULL);
  }

  mreq.imr_multiaddr.s_addr = inet_addr(HOST);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
  {
    printf("ERROR while setting socket options\n");
    pthread_exit(NULL);
  }

  printf("Listening...\n");
  while(1)
  {
    addrlen = sizeof(addr);

    if((n_bytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *) &addr, &addrlen)) < 0)
    {
      printf("ERROR while receiving message\n");
      pthread_exit(NULL);
    }
    char sender[50];
    char receiver[50];
    char message[50];

    sscanf(msgbuf, "%s;%s;%s", sender, receiver, message);
    printf("Received multicast message for %s [%s]\n", receiver, msgbuf);
    if(strcmp(receiver, name) == 0)
    {
      printf("Received message '%s' from agent %s\n", message, sender);
      char reply[150];
      handle_message(sender, message, reply);
      send_multicast_message(reply);
    }
  }
}

/* Gets line fron stdin. */
static int get_line (char * prmpt, char * buff, size_t sz)
{
  int ch, extra;

  if (prmpt != NULL)
  {
    printf ("%s", prmpt);
    fflush (stdout);
  }
  if (fgets (buff, sz, stdin) == NULL)
  {
    return NO_INPUT;
  }

  if (buff[strlen(buff)-1] != '\n')
  {
    extra = 0;
    while (((ch = getchar()) != '\n') && (ch != EOF))
    {
      extra = 1;
    }
    return (extra == 1) ? TOO_LONG : OK;
  }

  buff[strlen(buff)-1] = '\0';
  return OK;
}

int starts_with(const char *pre, const char *str)
{
  size_t lenpre = strlen(pre),
  lenstr = strlen(str);
  if(lenstr < lenpre) return 0;
  return !strncmp(pre, str, lenpre);
}

#endif
