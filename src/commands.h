#ifndef _COMMANDS_
#define _COMMANDS_

#include "helpers.h"
#include "requests.h"
#include "parson.h"

char* book(char* title,char* author,char* genre,char* publisher,int page_count);

void register_to_server(int sockfd);
void login(int sockfd,char** cookies,int* cookies_count);
void enter_library(int sockfd,char** cookies,int* cookies_count,char* token);
void get_books(int sockfd,char** cookies,int* cookies_count,char* token);
void get_book(int sockfd,char* token);
void add_book(int sockfd,char* token);
void delete_book(int sockfd,char* token);
void logout(int sockfd,char** cookies,int* cookies_count,char* token);

#endif