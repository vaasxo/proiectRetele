#include "commandEval.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PORT 2728

int main ()
{
	//Server
	struct sockaddr_in server, from;		//structurile pentru server si clienti
	int sd, client;					        //descriptori de socket
	pid_t pid;
	unsigned int fromLen = sizeof(from);

	if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  	{
  		perror ("[server]Eroare la socket().\n");
    	return errno;
  	}

  	fromLen = sizeof(from);
  	bzero (&from, sizeof (from));
  	bzero (&server, sizeof (server));

  	server.sin_family = AF_INET;
  	server.sin_addr.s_addr = htonl (INADDR_ANY);
  	server.sin_port = htons (PORT);

  	//Socket bind
  	if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
  	{
    	perror ("[server]Eroare la bind().\n");
    	return errno;
  	}

  	//Start Listening
  	if (listen (sd, 5) == -1)
  	{
    	perror ("[server]Eroare la listen().\n");
    	return errno;
  	}

  	while(1)
  	{
        client = accept (sd, (struct sockaddr *) &from, &fromLen);		//accept

		if (client < 0)
	    {
	      	perror ("[server] Eroare la accept().\n");
	      	continue;
	    }

	    pid=fork();

		if (pid < 0)
	    {
	      	perror ("[server] Eroare la fork().\n");
	      	continue;
	    }

	    if(pid==0)
	    {
	    	close(sd);

	    	if (commandEval(client))
			{
	  			printf ("[server] S-a deconectat clientul cu descriptorul %d.\n",client);
	  			fflush (stdout);
	  		}

	  		close(client);
	  		exit(0);
	    }
	    close(client);
	}
}