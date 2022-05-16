#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/select.h>

int main(){

int sock;
struct addrinfo hints;
hints.ai_family = AF_INET;
hints.ai_flags = AI_PASSIVE;
hints.ai_socktype = SOCK_STREAM;

struct addrinfo * binding;
if (getaddrinfo(0, "8080", &hints, &binding) < 0){
fprintf(stderr,"There has been an error trying to get the assoicated struct filled\n");
return -1;
}

sock = socket(binding->ai_family, binding->ai_socktype, binding->ai_protocol);

if (sock < 0){
fprintf(stderr, "There has been an error when giving out a file descriptor\n");
return -1;

}
int n = 0;
#if 0
if (setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&n, sizeof(n))) {
fprintf(stderr, "The setsockopt command failed\n");
 return -1;
 }
#endif
if (bind(sock, binding->ai_addr, binding->ai_addrlen) < 0) {
fprintf(stderr,"There has been an error when trying to bind a port with an address\n");
return -1;

}

freeaddrinfo(binding);

if (listen(sock, 10) < 0){
fprintf(stderr, "There has been an error listening to a socket\n");
return -1;
}

struct sockaddr_storage client;

socklen_t client_size = sizeof(client);

int client_fd = accept(sock, (struct sockaddr*) &client, &client_size);
if (client_fd < 0){
fprintf(stderr, "There has been an error when accepting a client connection\n");
return -1;
}


printf("Client is connected\n");

char address_buffer [1024];

getnameinfo((struct sockaddr *)&client, client_size, address_buffer, sizeof(address_buffer),0,0, NI_NUMERICHOST);
printf("%s\n", address_buffer);


printf("Recieving a request\n");

char request [1024];

int bytes_read = recv(client_fd, request, 1024, 0);

if (bytes_read < 0){
fprintf(stderr, "There has been an error recieving from the client\n");
return -1;
}

printf("Sending the pretext..\n");

char * response = "HTTP/1.1 200 OK\r\n"
                  "Connection: close \r\n"
                  "Content-Type: text/plain\r\n\r\n"
                  "Local time is: ";
     int bytes_sent = send(client_fd, response, strlen(response), 0);

     if (bytes_sent < 0 ){
     fprintf(stderr, "There has been an error with sending the information\n");
     return -1;
     }

  time_t timer;
  time(&timer);
  char *time_msg = ctime(&timer);
  bytes_sent = send(client_fd, time_msg, strlen(time_msg), 0);

  if (bytes_sent < 0){
  fprintf(stderr, "There has been an error in sending the timestamp\n");
  return -1;
  }

  close(client_fd);
  return 0;
  }


