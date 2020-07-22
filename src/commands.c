#include  "commands.h"

char* book(char* title,char* author,char* genre,char* publisher,int page_count) {
    
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *serialized_string = NULL;
    
    json_object_set_string(root_object, "title", title);
    json_object_set_string(root_object, "author", author);
    json_object_set_string(root_object, "genre", genre);
    json_object_set_number(root_object, "page_count", page_count);
    json_object_set_string(root_object, "publisher", publisher);
    serialized_string = json_serialize_to_string(root_value);

    json_value_free(root_value);
    return serialized_string;
}

void line(char* line)
{
	int i=0;
	char x;
	while( (x = getchar()) )
	{
	    if(x == '\n'||x == '\0')
	       return;
	    else{
	        line[i++]=x;
	    }
	}
}

void register_to_server(int sockfd)
{
	char* username=calloc(25,sizeof(char));
	char* password=calloc(25,sizeof(char));
	printf("username:");
	scanf("%s",username);
	printf("password:");
	scanf("%s",password);
	
	char* to_send=calloc(60,sizeof(char));
	sprintf(to_send,"{\n\t\"username\":\"%s\",\n\t\"password\":\"%s\"\n}",username,password);
	
	char* data;
	char* response;
	
	data  = compute_post_request(HOST_MAIN_SERVER, URL_REGISTER, DATA_JSON,&to_send,1, NULL,NULL, 0);
    send_to_server(sockfd, data);
    if(MY_DEBUG_ENABLE){
    	printf("<Request sent>, metadata: %s\n", data);
    }
    
    response = receive_from_server(sockfd);
	if(MY_DEBUG_ENABLE){
    	printf("<Response received>, metadata: %s\n", response);
	}

    if(strstr(response,"error")){
        printf("Error during login\n");
        if(strstr(response,"taken"))
            printf("Username already in use\n");
        return ;
    }
    else
    {
        printf("Successfull login\n");
    }

	printf("\n");
}

void login(int sockfd,char** cookies,int* cookies_count)
{
	char* username=calloc(25,sizeof(char));
	char* password=calloc(25,sizeof(char));
	printf("username:");
	scanf("%s",username);
	printf("password:");
	scanf("%s",password);
	
	char* to_send=calloc(60,sizeof(char));
	sprintf(to_send,"{\"username\":\"%s\",\"password\":\"%s\"}",username,password);
	
	char* data;
	char* response;
	
	data  = compute_post_request(HOST_MAIN_SERVER, URL_LOGIN, DATA_JSON,&to_send,1, NULL,NULL, 0);
    send_to_server(sockfd, data);
    if(MY_DEBUG_ENABLE){
    	printf("<Request sent>, metadata: %s\n", data);
    }
    
    response = receive_from_server(sockfd);
    if(MY_DEBUG_ENABLE){
    	printf("<Response received>, metadata: %s\n", response);
    }

    if(strstr(response,"error")){
    	printf("Error during login\n");
        if(strstr(response,"Credentials are not good"))
            printf("Username or password incorect\n");
        if(strstr(response,"No account with this username!"))
            printf("Username or password incorect\n");
    	return ;
    }
    else
    {
    	printf("Successfull login\n");
    }
    // locate cookie
    char* cookie=calloc(150,sizeof(char));
    int i=0;
    char* point=strstr(response,"Set-Cookie");
    point = strchr(point,':');
    char* end=strchr(point,'\n');
    // copy cookie
    for(point+=2 ; point<end ;point++)
    	cookie[i++]=point[0];

    cookies[*cookies_count]=cookie;
    (*cookies_count)++;

   	printf("\n");
}

void enter_library(int sockfd,char** cookies,int* cookies_count,char* token)
{
	char* data;
	char* response;
	
	data  = compute_get_request(HOST_MAIN_SERVER, URL_ACCESS, NULL,NULL,cookies,*cookies_count);
    send_to_server(sockfd, data);
    if(MY_DEBUG_ENABLE){
    	printf("<Request sent>, metadata: %s\n", data);
    }
    
    response = receive_from_server(sockfd);
    if(MY_DEBUG_ENABLE){
    	printf("<Response received>, metadata: %s\n", response);
	}
    
    if(strstr(response,"error")){
    	printf("Error during enter_library\n");
        if(strstr(response,"You are not logged in"))
            printf("You are not logged in\n");
    	return ;
    }
    else
    {
    	printf("Successfull enter_library\n");
    }

    char* mytoken=strstr(response,"token");
    mytoken=strchr(mytoken,':');
    mytoken+=2;

    char* end=strchr(mytoken,'}');
    int i=0;

	for(; mytoken<end ;mytoken++)
    	token[i++]=mytoken[0];

   	printf("\n");
}

void get_books(int sockfd,char** cookies,int* cookies_count,char* token)
{

	char* data;
	char* response;
	
	data  = compute_get_request(HOST_MAIN_SERVER, URL_BOOKS, NULL,token,NULL,0);
    send_to_server(sockfd, data);
    if(MY_DEBUG_ENABLE){
    	printf("<Request sent>, metadata: %s\n", data);
    }
    
    response = receive_from_server(sockfd);
    if(MY_DEBUG_ENABLE){
    	printf("<Response received>, metadata: %s\n", response);
    }

    if(strstr(response,"error")){
        
        printf("Error during get_books\n");
        
        if(strstr(response,"Error when decoding tokenn"))
            printf("You have no access!\n");

    	return ;
    }
    else
    {
    	printf("Successfull get_books\n");
    }

    char* myjson=calloc(250,sizeof(char));
    char* lo=strchr(response,'[');
    char* hi=strchr(response,']');
    strncpy(myjson,lo,hi-lo+1);

    JSON_Value *root_value;
    JSON_Array *books;
    JSON_Object *book;
    size_t i;
    
    root_value = json_parse_string(myjson);
    books = json_value_get_array(root_value);

    printf("%s %s\n", "id", "title");
    for (i = 0; i < json_array_get_count(books); i++) {
        book = json_array_get_object(books, i);
        printf("%d %s\n",
               (int)json_object_get_number(book, "id"),
               json_object_get_string(book, "title"));
    }
    json_value_free(root_value);

   	printf("\n");
}

void get_book(int sockfd,char* token)
{

	char* data;
	char* response;

	int id;
	printf("id:");
	scanf("%d",&id);

	char* the_url=calloc(75,sizeof(char));
	strcpy(the_url,URL_BOOKS);
	sprintf(the_url,"%s/%d",the_url,id);

	data  = compute_get_request(HOST_MAIN_SERVER, the_url, NULL,token,NULL,0);
    send_to_server(sockfd, data);
    if(MY_DEBUG_ENABLE){
    	printf("<Request sent>, metadata: %s\n", data);
    }
    
    response = receive_from_server(sockfd);
    if(MY_DEBUG_ENABLE){
    	printf("<Response received>, metadata: %s\n", response);
    }

    if(strstr(response,"error")){
    	printf("Error during get_book\n");

        if(strstr(response,"Error when decoding tokenn"))
            printf("You have no access!\n");

        if(strstr(response,"No book was found!"))
            printf("No book was found!\n");        
    	
        return ;
    }

    char* myjson=calloc(250,sizeof(char));
    char* lo=strchr(response,'[');
    char* hi=strchr(response,']');
    strncpy(myjson,lo,hi-lo+1);

    JSON_Value *root_value;
    JSON_Array *books;
    JSON_Object *book;
    size_t i;
    
    root_value = json_parse_string(myjson);
    books = json_value_get_array(root_value);

    for (i = 0; i < json_array_get_count(books); i++) {
        book = json_array_get_object(books, i);
        printf("%s%s\n%s%s\n%s%d\n%s%s\n%s%s\n",
               "Title\t\t",json_object_get_string(book, "title"),
               "Author\t\t",json_object_get_string(book, "author"),
               "Page count\t",(int)json_object_get_number(book, "page_count"),
               "Genre\t\t",json_object_get_string(book, "genre"),
               "Publisher\t",json_object_get_string(book, "publisher"));
    }
    json_value_free(root_value);
    
   	printf("\n");
}

void add_book(int sockfd,char* token)
{

	char* to_send=calloc(200,sizeof(char));

	char* title=calloc(50,sizeof(char));
	char* author=calloc(50,sizeof(char));
	char* genre=calloc(50,sizeof(char));
	char* publisher=calloc(50,sizeof(char));
	int page_count=0;

	getchar();

	printf("title:");
	line(title);

	printf("author:");
	line(author);

	printf("genre:");
	line(genre);
	
	printf("page_count:");
	scanf("%d",&page_count);

	getchar();

	printf("publisher:");
	line(publisher);
	
	sprintf(to_send,"%s",book(title,author,genre,publisher,page_count));
	char* data;
	char* response;
	
	data  = compute_post_request(HOST_MAIN_SERVER, URL_BOOKS, DATA_JSON,&to_send,1, token,NULL, 0);
    send_to_server(sockfd, data);
    if(MY_DEBUG_ENABLE){
    	printf("<Request sent>, metadata: %s\n", data);
    }
    
    response = receive_from_server(sockfd);
    if(MY_DEBUG_ENABLE){
    	printf("<Response received>, metadata: %s\n", response);
    }

    if(strstr(response,"error")){
        printf("Error during get_book\n");
        
        if(strstr(response,"Error when decoding tokenn"))
            printf("You have no access!\n");

        return ;
    }

   	printf("Successfull add_book\n");
}

void delete_book(int sockfd,char* token)
{
	char* data;
	char* response;

	int id;
	printf("id:");
	scanf("%d",&id);

	char* the_url=calloc(75,sizeof(char));
	strcpy(the_url,URL_BOOKS);
	sprintf(the_url,"%s/%d",the_url,id);

	data  = compute_delete_request(HOST_MAIN_SERVER, the_url, DATA_JSON,NULL,0, token,NULL, 0);
    send_to_server(sockfd, data);
    if(MY_DEBUG_ENABLE){
    	printf("<Request sent>, metadata: %s\n", data);
    }
    
    response = receive_from_server(sockfd);
    if(MY_DEBUG_ENABLE){
    	printf("<Response received>, metadata: %s\n", response);
    }
   	printf("\n");

    if(strstr(response,"error")){
        if(strstr(response,"No book was deleted!"))
            printf("No book was deleted!\n");

        if(strstr(response,"Error when decoding tokenn"))
            printf("You have no access!\n");

        return ;
    }
    printf("Successfull delete\n");

}

void logout(int sockfd,char** cookies,int* cookies_count,char* token)
{
	char* data;
	char* response;

    //no cookies no prev login
    if(*cookies_count==0){
        printf("You are not logged in\n");
        return;
    }
	
	data  = compute_get_request(HOST_MAIN_SERVER, URL_LOGOUT, DATA_JSON,NULL,cookies, 1);
    send_to_server(sockfd, data);
    if(MY_DEBUG_ENABLE){
    	printf("<Request sent>, metadata: %s\n", data);
    }
    
    response = receive_from_server(sockfd);
    if(MY_DEBUG_ENABLE){
    	printf("<Response received>, metadata: %s\n", response);
    }
    if(strstr(response,"error")){
    	printf("Error during logout\n");
        if(strstr(response,"You are not logged in"))
            printf("You are not logged in\n");
    }
    else
    {
    	printf("Successfull logout\n");
    	free(cookies[*cookies_count]);
    	(*cookies_count)--;
    	memset(token,0,300);
    }

   	printf("\n");
}