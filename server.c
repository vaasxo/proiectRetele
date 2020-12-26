#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sqlite3.h>

#define port 8888
#define max 1024
extern int errno;

int main ()
{
  struct sockaddr_in server;
  struct sockaddr_in from;	
  char send[max]; 
  char recv[max]=" ";
  int sd;

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
  	perror ("[server]Eroare la socket().\n");
    return errno;
  }
  
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  server.sin_family = AF_INET;	
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  server.sin_port = htons (port);
  
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
  {
    perror ("[server]Eroare la bind().\n");
    return errno;
  }

  if (listen (sd, 5) == -1)
  {
    perror ("[server]Eroare la listen().\n");
    return errno;
  }
}