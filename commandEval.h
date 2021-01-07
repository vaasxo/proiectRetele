//
// Created by xovaa on 1/6/2021.
//

#ifndef SERVER_C_COMMANDEVAL_H
#define SERVER_C_COMMANDEVAL_H

int commandEval(int fd);

int login(char* username, char* password, char* recv, int rc);

int reg(char* user_admin, char* username, char* password, char* recv, int rc);

int deleteSong(char* song_name, char* recv, int rc);

int restrictVote(char* deleteUser, char* recv, int rc);

int addSong(char* song_name, char* description, char* genre, char* link, char* recv, int rc);

int voteSong(char* song_name, char* recv, int rc);

int commentSong(char* song_name, char* comment, char* recv, int rc);

int sortGeneral(char* sort, int rc);

int sortGenre(char* genre, char* recv, char* sort, int rc);


#endif //SERVER_C_COMMANDEVAL_H

