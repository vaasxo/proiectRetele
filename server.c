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

/*sqlite3 *db;

//Database functions
int callback(void *str, int argc, char **argv, char **azColName)
{
	str = (char*) str;
	for (int i=0; i<argc; i++)
		if(argv[i])
			strcat(str, argv[i]);
	return 1;
}

//Command evaluation and database access
int commandEval(int fd)
{
	//Server
	char send[maxchr]; 				//mesajul trimis inapoi de server catre client
	char recv[maxchr]=" ";				//mesajul primit de server de la client
	char msg[maxchr];					//buffer pentru construirea mesajului de return
	char username[maxchr], password[maxchr], user_admin[maxchr], song_name[maxchr], description[maxchr], genre[maxchr], link[maxchr], comment[maxchr];
	int isLoggedIn=0; //1 - logged in as normal user, 2 - logged in as admin
	int i, j;
	int quit=0;

	//Database
	char *sql[maxchr];
	char *str[maxchr];
	char *err_msg = 0;

	//Database open
	int rc = sqlite3_open("topmusic.db", &db);
	if(rc != SQLITE_OK) 
	{
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return 1;
	}

	while(quit!=1)
	{
		fflush(stdout);
		memset (recv, 0, sizeof (recv));
		memset (send, 0, sizeof (send));
		memset (msg, 0, sizeof (msg));

		if (read (fd, recv, sizeof (recv)) < 0)
		{
	        perror ("[server]Eroare la read () de la client.\n");
	        return 1;
	    }

	    recv[strcspn(recv, "\n")] = 0;

	    if (strstr (recv, "login ") != NULL)
	    {
	    	memset(username, 0, sizeof(username));
	    	memset(password, 0, sizeof(password));

	    	strcpy(recv, recv+6);			//scapam de comanda in sine din mesajul primit
	    									//Cum comanda e fixa, folosim +6 in loc de strlen
	    	i=1, j=0;						//i - index pentru recv, j - index pentru parametri
	    	while(recv[i]!='\'')
	    	{
	    		username[j++]=recv[i++];
	    	}

	    	i+=3;							// sarim peste spatiul dintre comenzi
	    	j=0;							// resetam index-ul pentru parametri
	    	while(recv[i]!='\'')
	    	{
	    		password[j++]=recv[i++];
	    	}


	    	isLoggedIn=1;
	    	//if admin
	    		//set isLoggedIn 2
	    }
	    else if (strstr (recv, "register ") != NULL)
	    {
	    	memset(user_admin, 0, sizeof(user_admin));
	    	memset(username, 0, sizeof(username));
	    	memset(password, 0, sizeof(password));

	    	strcpy(recv, recv+9);

	    	i=1, j=0;
	    	while(recv[i]!='\'')
	    	{
	    		user_admin[j++]=recv[i++];
	    	}

	    	i+=3;
	    	j=0;
	    	while(recv[i]!='\'')
	    	{
	    		username[j++]=recv[i++];
	    	}

	    	i+=3;
	    	j=0;
	    	while(recv[i]!='\'')
	    	{
	    		password[j++]=recv[i++];
	    	}
	    }
	    else if ((strstr (recv, "delete song ") != NULL) && isLoggedIn==2)
	    {
	    	memset(song_name, 0, sizeof(song_name));

	    	strcpy(recv, recv+12);

	    	i=1, j=0;
	    	while(recv[i]!='\'')
	    	{
	    		song_name[j++]=recv[i++];
	    	}
	    }
	    else if ((strstr (recv, "restrict vote ") != NULL) && isLoggedIn==2)
	    {
	    	memset(username, 0, sizeof(username));

	    	strcpy(recv, recv+14);

	    	i=1, j=0;
	    	while(recv[i]!='\'')
	    	{
	    		username[j++]=recv[i++];
	    	}
	    }
	    else if (strstr (recv, "add song ") != NULL && isLoggedIn==1)
	    {
	    	memset(song_name, 0, sizeof(song_name));
	    	memset(description, 0, sizeof(description));
	    	memset(genre, 0, sizeof(genre));
	    	memset(link, 0, sizeof(link));

	    	strcpy(recv, recv+9);

	    	i=1, j=0;
	    	while(recv[i]!='\'')
	    	{
	    		song_name[j++]=recv[i++];
	    	}

	    	i+=3;
	    	j=0;
	    	while(recv[i]!='\'')
	    	{
	    		description[j++]=recv[i++];
	    	}

	    	i+=3;
	    	j=0;
	    	while(recv[i]!='\'')
	    	{
	    		genre[j++]=recv[i++];
	    	}

	    	i+=3;
	    	j=0;
	    	while(recv[i]!='\'')
	    	{
	    		link[j++]=recv[i++];
	    	}
	    }
	    else if (strstr (recv, "vote song ") != NULL && isLoggedIn==1)
	    {
	    	memset(song_name, 0, sizeof(song_name));

	    	strcpy(recv, recv+10);

	    	i=1, j=0;
	    	while(recv[i]!='\'')
	    	{
	    		song_name[j++]=recv[i++];
	    	}
	    }
	    else if (strstr (recv, "comment song ") != NULL && isLoggedIn==1)
	    {
	    	memset(song_name, 0, sizeof(song_name));
	    	memset(comment, 0, sizeof(comment));

	    	strcpy(recv, recv+13);

	    	i=1, j=0;
	    	while(recv[i]!='\'')
	    	{
	    		song_name[j++]=recv[i++];
	    	}

	    	i+=3;
	    	j=0;
	    	while(recv[i]!='\'')
	    	{
	    		comment[j++]=recv[i++];
	    	}
	    }
	    else if (strstr (recv, "top song general ") != NULL && isLoggedIn==1)
	    {
	    	//no parameters, will just return the sorted top
	    }
	    else if (strstr (recv, "top song genre ") != NULL && isLoggedIn==1)
	    {
	    	memset(genre, 0, sizeof(genre));

	    	strcpy(recv, recv+15);

	    	i=1, j=0;
	    	while(recv[i]!='\'')
	    	{
	    		genre[j++]=recv[i++];
	    	}
	    }
	    else if (strstr (recv, "commands") != NULL)
	    {
	    	strcpy(msg,"The list of available commands, followed by the necessary parameters:\n1. login 'username' 'password'\n2. register 'user/admin' 'username' 'password'\n");

	    	if(isLoggedIn==1)
	    		strcpy(msg,"3. add song 'song_name' 'description' 'genre' 'link'\n4. vote song 'song_name'\n5. comment song 'song_name' 'comment'\n6. top song general\n7. top song 'genre'\n");

	    	if(isLoggedIn==2)
	    		strcpy(msg,"8. delete song 'song_name'\n9. restrict vote 'username'\n");
	    	strcpy(send,msg);

	    }
	    else if (strstr (recv, "quit") != NULL)
	    {
	    	strcpy(send, "quit");
	    	quit=1;
	    }
	    else
	    {
	    	strcpy(send, "command not recognized! For a list of available commands, try 'commands'");
	    }

	    int sendLength=strlen(send);

	    if (write (fd, send, sendLength) <= 0)
	    {
			perror ("[server]Eroare la write() catre client.\n");
	        return errno;
	 	}
	}
    //printf("sent %s to client", recv);
    //fflush(stdout);
	sqlite3_close(db);
	return 0;
}*/

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