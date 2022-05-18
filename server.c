#include<string.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>


#define ISVALID(s) (s > 0)
#define CLOSESOCKET(s) (close(s))
int main(){

typedef int SOCKET;

struct addrinfo host;

memset(&host, 0 , sizeof(host));

host.ai_family = AF_INET;

host.ai_flags = AI_PASSIVE;

host.ai_socktype = SOCK_STREAM;

struct addrinfo *binding;



printf("getaddrinfo\n");
if (getaddrinfo(0, "8080", &host, &binding) < 0){

   fprintf(stderr, "There has been an error in filling up the addrinfo struct\n");
   return -1;

}

  printf("socket\n");

int sock = socket(binding->ai_family, binding->ai_socktype, binding->ai_protocol);

 printf("after socket\n");
if (!ISVALID(sock)){
    fprintf(stderr, "There has been an error in trying to set up the socket\n");
    return -1;

}

  printf("binding to an address\n");
 if (bind(sock, binding->ai_addr, binding->ai_addrlen) < 0) {
     fprintf(stderr, "There has been an error in binding the socket to an address\n");
    return -1;

   }

   freeaddrinfo(binding);

  printf("Listening to a connection\n");

  if (listen(sock , 10) < 0 ){
      fprintf(stderr, "There has been an error in listening connections\n");
      return -1;
  }

  printf("Setting up a fd_set\n");
  fd_set sock_set;

  FD_ZERO(&sock_set);

   FD_SET(sock, &sock_set);

  int max = sock;

  printf("hello\n");

   while(1){

   fd_set duplicate = sock_set;

   printf("Selecting in a loop\n");
   if (select(max +1 , &duplicate, 0, 0, 0) < 0) {

   fprintf(stderr, "There has been an error in select() \n");
   return -1;
   }

   for(int i = 0; i <= max; i++){

   if(FD_ISSET(i, &duplicate)) {

    int client;

    if(i == sock) {

    struct sockaddr_storage my_client;

    socklen_t client_size = sizeof(my_client);

   printf("accepting a connection \n");
    client = accept(sock, (struct sockaddr *)&my_client, &client_size);

    if (!ISVALID(client) ){

     CLOSESOCKET(client);
     fprintf(stderr,"There has been an error in accepting a connection\n");
     return -1;
     }

     FD_SET(client, &sock_set);
     if (client > max){
       max = client;
       }

     char address[1024];

     if (getnameinfo((struct sockaddr*)&client, client_size, address, sizeof(address) , 0 , 0 , NI_NUMERICHOST) < 0) {

        fprintf(stderr, "There has been an error in getting the address of the client\n");
        return -1;
        }
       printf("Now connecting to the client address: %s\n", address);
     }

     else {

      printf("Connected to a client\n");

     char read[4096];

     int bytes_recv = recv(client, read, sizeof(read), 0);
     if (bytes_recv < 1 ){
      FD_CLR(i, &sock_set);
      CLOSESOCKET(i);

     fprintf(stderr, "There has been an error in recieving the information sent out by the server\n");
     return -1;
     }


    for(int j = 1; j <= max; j++){

     if(FD_ISSET(j, &sock_set) ){

     if (j == i || j == sock){
     continue;
     }

     int bytes_send = send(j, read, strlen(read), 0);

}

}
}
} //FD_ISSET()
} // while(1)
}
printf("Process completed\n");
printf("Closing socket\n");

CLOSESOCKET(sock);
return 0;
}


