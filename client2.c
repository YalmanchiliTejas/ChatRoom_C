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

int main(int args , char ** argv) {


if (args != 3){
fprintf(stderr, "Please provide the localhost and the port in the specified order\n");
return -1;
}

struct addrinfo client;

memset(&client, 0, sizeof(client));
client.ai_socktype = SOCK_STREAM;

struct addrinfo *binder;

printf("The ooo\n");
if(getaddrinfo(argv[1], argv[2], &client, &binder) < 0){
  fprintf(stderr, "There has been an error in getaddrinfo\n");
  return -1;
  }

  char address_buffer[100];
  char service_buffer[100];

   printf("getaddrinfo\n");
  if (getnameinfo(binder->ai_addr, binder->ai_addrlen, address_buffer,
      sizeof(address_buffer), service_buffer,
      sizeof(service_buffer), NI_NUMERICHOST) < 0) {
  fprintf(stderr, "There has been an error in the getnameinfo section\n");
  return -1;
  }

int sock = socket(binder->ai_family, binder->ai_socktype, binder->ai_protocol);
printf("sock\n");
if ( sock < 0){
fprintf(stderr, "There has been an error in getting a socket\n");
return -1;
}

if (connect(sock, binder->ai_addr, binder->ai_addrlen) < 0) {

 fprintf(stderr, "There has been an error connecting to the server\n");

 return -1;
 }
 printf("connect\n");

 freeaddrinfo(binder);

printf("freeaddrinfo\n");

printf("Now, connecting to a server and sending out information\n");

char buffer [1024];

printf("Press q to stop sending information\n");
while(1){
 printf("Entered the loop\n");
fscanf(0, "%1023[^\n]\n", buffer);
printf("reading information\n");
if (fgetc(0) == '\n') {

   printf("Sending the information\n");
  int bytes_sent = send(sock, buffer, strlen(buffer), 0);
  printf("Sent these many %d bytes\n", bytes_sent);
  if (bytes_sent < 1){

  printf("connection closed\n");
  break;
  }

  }
  if (fgetc(0) == 'q' || fgetc(0) == 'Q') {
  printf("stopping to send the information\n");
   break;
   }
   }
   close(sock);
   printf("client connection closed\n");

   return 0;
   }

   

