#include "commands.h"



int main(int argc, char *argv[])
{
    int sockfd = open_connection(IP_HOST_MAIN, PORT_MAIN,
                                 AF_INET, SOCK_STREAM, 0);

    char** cookies=calloc(100,sizeof(char*));
    int cookies_count=0;
    char* token=calloc(300,sizeof(char));

    char* read=calloc(50,sizeof(char));

    while(1)
    {

        sockfd = open_connection(IP_HOST_MAIN, PORT_MAIN,
                                 AF_INET, SOCK_STREAM, 0);

        scanf("%s",read);
        if(!strncmp(read,"exit",4))
            break;
        if(!strncmp(read,"register",8))
            register_to_server(sockfd);

        if(!strncmp(read,"login",5))
            login(sockfd,cookies,&cookies_count);

        if(!strncmp(read,"enter_library",13))
            enter_library(sockfd,cookies,&cookies_count,token);

        if(!strncmp(read,"get_books",9))
            get_books(sockfd,cookies,&cookies_count,token);
        else
        {
            if(!strncmp(read,"get_book",8))
            get_book(sockfd,token);
        }
        
        if(!strncmp(read,"add_book",8))
            add_book(sockfd,token);

        if(!strncmp(read,"delete_book",11))
            delete_book(sockfd,token);

        if(!strncmp(read,"logout",6))
            logout(sockfd,cookies,&cookies_count,token);
        
    }

    close_connection(sockfd);

//    free(data);
//    free(cookie);
//    free(key);
    
    return 0;
}

