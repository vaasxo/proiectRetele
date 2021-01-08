//
// Created by xovaa on 1/6/2021.
//

#ifndef SERVER_C_COMMANDEVAL_H
#define SERVER_C_COMMANDEVAL_H

int commandEval(int fd);

int login(char* username, char* password, char* recv);

int reg(char* user_admin, char* username, char* password, char* recv);

int deleteSong(char* song_name, char* recv);

int restrictVote(char* deleteUser, char* recv);

int addSong(char* song_name, char* description, char* genre, char* link, char* recv);

int voteSong(char* song_name, char* recv);

int commentSong(char* username, char* song_name, char* comment, char* recv);

int showComments(char* song_name, char* recv, char* comment);

int sortGeneral(char* sort);

int sortGenre(char* genre, char* recv, char* sort);

int callback_select(void *str, int argc, char **argv, char **azColName);

int callback_sort(void *str, int argc, char **argv, char **azColName);

int callback_comm(void *str, int argc, char **argv, char **azColName);

#endif //SERVER_C_COMMANDEVAL_H

