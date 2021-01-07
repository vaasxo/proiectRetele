#include "commandEval.h"
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#define maxchr 1024

sqlite3 *db;

//Database functions
int callback_select(void *str, int argc, char **argv, char **azColName)
{
    char* data=(char*)str;
    for(int i=0;i<argc;i++)
    {
        printf("%s \n", argv[i]);
        fflush(stdout);
        strcat(data, argv[i]);
    }
    return 0;
}

int login(char* username, char* password, char* recv)
{
    int rc;
    char queryRes[maxchr];

    int i=1, j=0;						    //i - index pentru recv, j - index pentru parametri
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

    char sql[maxchr];

    sprintf(sql,"SELECT isAdmin FROM users WHERE uid ='%s' AND password ='%s';",username, password);

    printf("%s", sql);
    fflush(stdout);

    rc = sqlite3_exec(db, sql, callback_select, queryRes, NULL);
    if(rc!=SQLITE_OK)
        return 0;

    if(strcmp(queryRes, "1")==0)
        return 1;
    else if(strcmp(queryRes, "2")==0)
        return 2;
    else
        return -1;
}

int reg(char* user_admin, char* username, char* password, char* recv)
{
    int rc;

    int i=1, j=0;
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

    int isAdmin;

    if(strstr(user_admin,"user")==0)
        isAdmin=1;
    else if (strstr(user_admin,"admin")==0)
        isAdmin=2;
    else
        return -1;

    char sql[maxchr];
    sqlite3_stmt *stmt;

    sprintf(sql, "INSERT INTO users VALUES('%s', '%s', 1, %d);", username,password,isAdmin);

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE)
    {
        return 1;
    }
    else
    {
        return -2;
    }
}

int deleteSong(char* song_name, char* recv)
{
    int i=1, j=0;
    while(recv[i]!='\'')
    {
        song_name[j++]=recv[i++];
    }
}

int restrictVote(char* deleteUser, char* recv)
{
    int i=1, j=0;
    while(recv[i]!='\'')
    {
        deleteUser[j++]=recv[i++];
    }
}

int addSong(char* song_name, char* description, char* genre, char* link, char* recv)
{

    int i=1, j=0;
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

int voteSong(char* song_name, char* recv)
{
    int i=1, j=0;
    while(recv[i]!='\'')
    {
        song_name[j++]=recv[i++];
    }
}

int commentSong(char* song_name, char* comment, char* recv)
{
    int i=1, j=0;
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

int sortGeneral(char* sort)
{
    //TODO sorting of songs by votes
}

int sortGenre(char* genre, char* recv, char* sort)
{
    int i=1, j=0;
    while(recv[i]!='\'')
    {
        genre[j++]=recv[i++];
    }
}

int commandEval(int fd)
{
    //Server
    char send[maxchr]; 				    //mesajul trimis inapoi de server catre client
    char recv[maxchr];				    //mesajul primit de server de la client
    char msg[maxchr];					//buffer pentru construirea mesajului de return
    char username[maxchr], deleteUser[maxchr], password[maxchr], user_admin[maxchr], song_name[maxchr], description[maxchr], genre[maxchr], link[maxchr], comment[maxchr], sort[maxchr];
    int isLoggedIn=0; //1 - logged in as normal user, 2 - logged in as admin
    int quit=0;

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

            strcpy(recv, recv+6);

            int loginResult=login(username,password,recv);
            switch(loginResult){
                case -1:
                    strcpy(send, "cannot log in - username or password incorrect\n");
                    break;
                case 1:
                    strcpy(send, "log in as user successful!\n");
                    isLoggedIn=1;
                    break;
                case 2:
                    strcpy(send, "log in as admin successful!\n");
                    isLoggedIn=2;
                    break;
                default:
                    strcpy(send, "error trying to log in - probably the programmer's fault!\n");
                    break;
            }
        }
        else if (strstr (recv, "register ") != NULL)
        {
            memset(user_admin, 0, sizeof(user_admin));
            memset(username, 0, sizeof(username));
            memset(password, 0, sizeof(password));

            strcpy(recv, recv+9);

            int regResult = reg(user_admin,username,password,recv);
            switch(regResult){
                case -2:
                    strcpy(send, "cannot register - user already in database!\n");
                    break;
                case -1:
                    strcpy(send, "cannot register - make sure to include user/admin!\n");
                    break;
                case 1:
                    strcpy(send, "register successful!\n");
                    break;
                default:
                    strcpy(send, "error trying to register - probably the programmer's fault!\n");
                    break;
            }
        }
        else if ((strstr (recv, "delete song ") != NULL) && isLoggedIn==2)
        {
            memset(song_name, 0, sizeof(song_name));

            strcpy(recv, recv+12);

            int deleteResult=deleteSong(song_name,recv);
            switch(deleteResult){
                case -1:
                    strcpy(send, "cannot delete song - not found in database\n");
                    break;
                case 1:
                    strcpy(send, "successfully deleted song!\n");
                    break;
                default:
                    strcpy(send, "error trying to delete song - probably the programmer's fault!\n");
                    break;
            }
        }
        else if ((strstr (recv, "restrict vote ") != NULL) && isLoggedIn==2)
        {
            memset(deleteUser, 0, sizeof(deleteUser));

            strcpy(recv, recv+14);

            int restrictResult=restrictVote(deleteUser,recv);
            switch(restrictResult){
                case -1:
                    strcpy(send, "cannot restrict user right to vote - not found in database\n");
                    break;
                case 1:
                    strcpy(send, "successfully restricted voting permissons!\n");
                    break;
                default:
                    strcpy(send, "error trying to restrict vote permission - probably the programmer's fault!\n");
                    break;
            }
        }
        else if (strstr (recv, "add song ") != NULL && isLoggedIn==1)
        {
            memset(song_name, 0, sizeof(song_name));
            memset(description, 0, sizeof(description));
            memset(genre, 0, sizeof(genre));
            memset(link, 0, sizeof(link));

            strcpy(recv, recv+9);

            int addSongResult = addSong(song_name, description, genre, link, recv);
            switch(addSongResult){
                case -1:
                    strcpy(send, "cannot add song - already in database\n");
                    break;
                case 1:
                    strcpy(send, "song added successfully!\n");
                    break;
                default:
                    strcpy(send, "error trying to add song - probably the programmer's fault!\n");
                    break;
            }
        }
        else if (strstr (recv, "vote song ") != NULL && isLoggedIn==1)
        {
            memset(song_name, 0, sizeof(song_name));

            strcpy(recv, recv+10);

            int voteSongResult=voteSong(song_name,recv);
            switch(voteSongResult){
                case -1:
                    strcpy(send, "cannot vote song - you do not have permission to vote!\n");
                    break;
                case 1:
                    strcpy(send, "successfully voted!\n");
                    break;
                default:
                    strcpy(send, "error trying to vote song - probably the programmer's fault!\n");
                    break;
            }
        }
        else if (strstr (recv, "comment song ") != NULL && isLoggedIn==1)
        {
            memset(song_name, 0, sizeof(song_name));
            memset(comment, 0, sizeof(comment));

            strcpy(recv, recv+13);

            int commSongResult=commentSong(song_name,comment,recv);
            switch(commSongResult){
                case -1:
                    strcpy(send, "error commenting on song - make sure you use the foramt properly!\n");
                    break;
                case 1:
                    strcpy(send, "successfully commented!\n");
                    break;
                default:
                    strcpy(send, "error trying to comment on song - probably the programmer's fault!\n");
                    break;
            }

        }
        else if (strstr (recv, "top song general ") != NULL && isLoggedIn==1)
        {
            memset(sort,0,sizeof (sort));

            int sortGeneralResult=sortGeneral(sort);
            if(sortGeneralResult)
                strcpy(send, sort);
            else
                strcpy(send, "error trying to sort by votes - probably the programmer's fault!\n");
        }
        else if (strstr (recv, "top song genre ") != NULL && isLoggedIn==1)
        {
            memset(genre, 0, sizeof(genre));
            memset(sort,0,sizeof (sort));

            strcpy(recv, recv+15);

            int sortGenreResult=sortGenre(genre, recv, sort);
            if(sortGenreResult)
                strcpy(send, sort);
            else
                strcpy(send, "error trying to sort by genre - probably the programmer's fault!\n");
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

        int sendLength=(int)strlen(send);

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
}