#ifndef _HELPERS_
#define _HELPERS_

#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>		/* strstr,strchr */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>

#define BUFLEN 4096
#define LINELEN 1000

#define HOST_MAIN_SERVER "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com"
#define PORT_MAIN        8080
#define IP_HOST_MAIN     "3.8.116.10"

#define URL_REGISTER    "/api/v1/tema/auth/register"
#define URL_LOGIN   "/api/v1/tema/auth/login"
#define URL_ACCESS  "/api/v1/tema/library/access"
// "/api/v1/tema/library/books/1" to get a specific book
#define URL_BOOKS   "/api/v1/tema/library/books"
#define URL_LOGOUT  "/api/v1/tema/auth/logout"
#define DATA_JSON   "application/json"

#define MY_DEBUG_ENABLE 0

// shows the current error
void error(const char *msg);

// adds a line to a string message
void compute_message(char *message, const char *line);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, char *message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
char *basic_extract_json_response(char *str);

#endif
