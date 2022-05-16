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

host.ai_family = AF_INET;

host.ai_flags = AI_PASSIVE;

host.ai_socktype = SOCK_STREAM;

struct addrinfo *binding;

if (getaddrinfo(0, "8080", &host, &binding) < 0){
 
   fprintf(stderr, "There has been an error in filling up the addrinfo struct\n");
   return -1;

}

int sock = socket(binding->ai_family, binding->ai_socktype, binding->ai_protocol);

if (!ISVALID(sock)){
    fprintf(stderr, "There has been an error in trying to set up the socket\n");
    return -1;

}

 if (bind(sock, binding->ai_addr, binding->ai_addrlen) < 0) {
     fprintf(stderr, "There has been an error in binding the socket to an address\n");
    return -1;

   }

   freeaddrinfo(binding);


  if (listen(sock , 10) < 0 ){
      fprintf(stderr, "There has been an error in listening to future connections\n");
      return -1;

  }
  printf("It tries to set a sock\n");
   fd_set set_sock;
   FD_ZERO(&set_sock);
FD_SET(sock, &set_sock);
 printf("The socket has been set\n");
int max = sock;
 struct timeval time;
 time.tv_sec = 0;
 time.tv_usec = 100000;
   while(1){

       fd_set duplicate; 
       duplicate = set_sock;
      if(select(max +1 , &duplicate, 0, 0 , &time) < 0 ){
          fprintf(stderr, "There has been an error with the select function\n");
          return -1;

      }

      for(int i = 0; i <= max; i++){
          if (FD_ISSET(i, &duplicate)){
          int client;
          if (i == sock){
                printf("accepting the incoming client\n");
                int my_client;
              struct sockaddr_storage client;
                 socklen_t client_size = sizeof(client);
              my_client = accept(sock, (struct sockaddr *) &client,  &client_size);
              if(!ISVALID(my_client)){
                  fprintf(stderr, "There has been an error when it tried to accept a client\n");
                  return -1;
              }
              FD_SET(my_client, &set_sock);
              if (my_client > max){
                  max = my_client;
              } 
              char address [1024];
              if(getnameinfo((struct sockaddr *)&client, client_size, address, sizeof(address), 0 , 0, NI_NUMERICHOST) < 0) {
                  fprintf(stderr, "There has been an error when getting its information");
                  return -1;

              }
              printf("The new connection is from %s\n", address);

          } else{
             printf("Connected to a client\n");
              printf("Got the client\n");
              char read [1024];
              int bytes_recv = recv(i, read, 1024, 0);
              if (bytes_recv < 1){
               FD_CLR(i , &set_sock);
               CLOSESOCKET(i);
               continue;

              }
              printf("The no of bytes read = %d\n", bytes_recv);


          for(int j = 0; j < bytes_recv; j++)
            read[j] = toupper(read[j]);
            int bytes_sent = send(i, read, strlen(read) , 0);

            if (bytes_sent  < 1){
                fprintf(stderr, "There has been an error in sending the information\n");
                return -1;
            }

          }
      }
      }

   }

   CLOSESOCKET(sock);
      printf("The process is finished\n");
      return 0;

}


