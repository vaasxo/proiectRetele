#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

#define port 2728
#define max 1024
extern int errno;

void beginText()
{
	printf("Starting...\n");
    sleep(1);
    printf("Fixing bugs, sweeping errors under the rug...\n");
    sleep(1);
    printf("Almost ready...\n\n");
    sleep(2);
    printf("                         Welcome to TopMusic!\n\n");
}

void endText()
{
	printf("Quitting...\n");
	printf("See you next time!\n");
}


int main() 
{
    int sd, msgLength;
    struct sockaddr_in server;
    char send[max];
    char recv[max];

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("Error creating socket :/.\n");
        return errno;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr ("127.0.0.1");
    server.sin_port = htons (port);

    if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("Error connecting to server.\n");
      return errno;
    }

    beginText();

    while(1)
    {

    	printf(">");
    	fflush(stdout);
    	
     	memset (send, 0, max);
     	read (0, send, max);

     	write (sd, send, sizeof (send));
     	memset (recv, 0, sizeof (recv));
     	read (sd, recv, sizeof (recv));

     	if(strncmp(recv, "quit", 4)==0)
     		break;
     	else
     		printf ("%s\n", recv);
 	}

 	endText();

 	close(sd);
    return 1;
}
