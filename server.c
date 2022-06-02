#include "network.h"
#include "student.h"
#include "rio.h"
#include <time.h>
#include <assert.h>
#include <stdint.h>

#define ARB_BUF (1000)
#define SUCCESS (0)
#define NEW_USER (1)
#define EXISTING (2)
#define ERROR (-2)
#define NON_EXIST (3)
#define WRONG_PASS (4)


student_t * return_stu (char * username);

student_t * head = NULL;

time_t t;
/*
 * this returns a socket fd
 */

int initiating(char * port) {

 int host;
 struct addrinfo  list;
 struct addrinfo * p;
 int sockopt = 1;

 memset(&list, 0, sizeof(struct addrinfo));

 list.ai_family = AF_INET;
 list.ai_socktype = SOCK_STREAM;
 list.ai_flags = AI_PASSIVE;
 list.ai_flags |= AI_NUMERICSERV;

 if (getaddrinfo(NULL, port, &list, &p) != 0) {

   fprintf(stderr, "getaddrinfo error occurred\n");
   return ERROR;
   }

  struct addrinfo * temp;
  for( temp = p; temp; temp = temp->ai_next) {

    host = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
    if (host < 0) {
    continue;
    }
    if (setsockopt(host, SOL_SOCKET ,SO_REUSEADDR, (const void *)&sockopt,
                   sizeof(host)) != 0) {

   fprintf(stderr, "There has been an error in sockopt\n");
   return ERROR;

}
   if ( bind(host, temp->ai_addr, temp->ai_addrlen) !=  0) {

     CLOSESOCKET(host);
     fprintf(stderr, "error in binding\n");
     return ERROR;
     }
    break;
    }
    if (listen(host, 1024) < 0) {
    fprintf(stderr, "An error in listening\n");
    CLOSESOCKET(host);
    return ERROR;
    }
    return host;
    }




  /*
   * This function sends the message to specified user
   */

    void send_msg (int fd, char * reciever, char *sender, char * msg) {

    student_t * recv = NULL;
    char  response [ARB_BUF] = {0};

   int value = 0;
    while (recv != NULL) {

   if (strcmp(recv->username, reciever) == 0) {
    value = 1;
    break;
    } else {
    value = 0;
    }
    recv = recv->next;
    }

    if ( value == 1) {
     written(recv->fd, msg, strlen(msg));
     sprintf(response,"msg sent to %s at %s\n" , reciever, ctime(&t));
     written(fd, response, strlen(response));
     return;
     }
     else {
     strcpy(response, "message failed\n\r\n");
     return;
     }
     } /* send_msg() */

   /*
    * This function would take care of the
    * user requested functions
    */

     int  evaluate (void * file, rio_t  rp, student_t * node, char * keyword) {

       int fd = *((int *) file);

       /* stores the username */

       char username [ARB_BUF];

       /* stores the password */

       char password [9];

       /* stores the message that needs to be sent */

       char msg [ARB_BUF];

      /* stores the keyword to do appropriate actions */

       char major [ARB_BUF];

     major[0] = '\0';
     username[0] = '\0';
     password[0] = '\0';
     msg[0] = '\0';

         int bytes = 0;

       if (strcmp(keyword, "l") == 0) {

        bytes = 0;
        if ( (bytes = readlineb(&rp, username, sizeof(username))) == -1 ) {

            return ERROR;
        }
     username[strlen(username) - 1] = '\0';

     bytes = 0;
     if ((bytes = readlineb(&rp, password, sizeof(password))) == -1) {
     return ERROR;
     }

    password[strlen(password) - 1] = '\0';

    bytes = 0;

     if ( (bytes = login_menu(username, password)) != SUCCESS) {

        int * tell = NULL;
           tell = &bytes;
           * tell = bytes;
         int write_int = send(fd, tell, sizeof(int), 0);
         return ERROR;
    }

        node = return_stu(username);
        node->fd = fd;
        node->online = true;


           return SUCCESS;
      }
      else if (strcmp(keyword, "s") == 0) {

        bytes = 0;
      if ( (bytes = readlineb(&rp, username, sizeof(username) )) == -1) {

         return ERROR;
      }
      bytes = 0;
      username[strlen(username) - 1] = '\0';

    if ((bytes =  readlineb(&rp, password, sizeof(password))) == -1) {

       return ERROR;
      }
       password[strlen(password) -1 ] = '\0';
       bytes = 0;
      if ((bytes = sign_up(username, password)) != SUCCESS) {

      int write_int = send(fd, &bytes, sizeof(bytes), 0);
      return ERROR;
      }
      bytes = 0;

      if ( (bytes = readlineb(&rp, major, sizeof(major)) ) == -1) {

      return ERROR;
      }

      node = malloc(sizeof(student_t *));
      assert(node != NULL);
      node->username = malloc(strlen(username));
      assert(node->username != NULL);
      strcpy(node->username, username);
      strcpy(node->password, password);
      node->major = malloc(strlen(major));
      assert(node->major != NULL);
      strcpy(node->major, major);
      node->fd = fd;
      node->online = true;
      return SUCCESS;
      }
      else if (strcmp(keyword, "m") == 0) {

     bytes = 0;

   if ((bytes = readlineb(&rp, username, sizeof(username))) == -1 ) {

      return ERROR;
      }
     username[strlen(username) - 1] = '\0';
       bytes = 0;
  if ((bytes = readlineb(&rp, msg, sizeof(msg))) == -1) {
   return ERROR;
   }
   msg[strlen(msg) -1] = '\0';
   bytes = 0;

     send_msg(node->fd, username, node->username, msg);
     return SUCCESS;
     }
     else if (strcmp(keyword, "f") == 0) {
     bytes = 0;

     if ( (bytes = readlineb(&rp, major, sizeof(major))) == -1) {

      return ERROR;
      }
   major[strlen(major) -1 ] = '\0';
   bytes = 0;

   student_t * temp = head;
   while(temp != NULL) {

   if (strcmp(temp->major, major) == 0) {

    written(fd,temp->username, strlen(temp->username));
     bytes = 1;
    } else {
     bytes = 0;
     }
    }
    if (bytes == 1) {
    return SUCCESS;
    }

    return NON_EXIST;
    }
    else if (strcmp(keyword,"u") == 0) {
      keyword[0] = '\0';
     bytes = 0;
     if ((bytes = readlineb(&rp , keyword, sizeof(keyword))) == -1) {

       return ERROR;
       }
       bytes = 0;
      if ( strcmp(keyword, "p") == 0 ) {

      password[0] = '\0';
      keyword[0] = '\0';
      student_t * temp = head;

      while(temp != NULL) {

  if ( strcmp(temp->username, node->username) == 0) {

        if ((bytes = readlineb(&rp, password, sizeof(password))) == -1) {

           return ERROR;
          }
       strcpy(node->password, password);
       node->password[strlen(node->password) -1 ] = '\0';
       strcpy(temp->password, password);
       temp->password[strlen(temp->password) -1 ] = '\0';
       return SUCCESS;
       }
       }
       }
       else if (strcmp(keyword, "m") == 0) {

        major[0] = '\0';
        keyword[0] = '\0';

        student_t * temp = head;
        while( temp != NULL) {

        if (strcmp(temp->username, node->username) == 0 ) {

          if ((bytes = readlineb(&rp, major, sizeof(major))) == -1) {

          return ERROR;
          }
          major[strlen(major) - 1] = '\0';
          strcpy(temp->major, major);
           temp->major[strlen(temp->major) - 1] = '\0';
          strcpy(node->major, major);
          node->major[strlen(node->major) - 1] = '\0';
          return SUCCESS;
          }
          }
          }
          }
          return 0;
          }


    /*
     * This function returns the student of the
     * same username
     */

      student_t * return_stu(char * username) {

      student_t * node = head;
      while(node != NULL) {

      if (strcmp(node->username, username) == 0 ) {

        return node;
        }
        node = node->next;
        }
        return NULL;
        } /* return_stu() */




    int main() {


    int sock = initiating("80");

    if (sock == ERROR) {

      fprintf(stderr, "There has been an error in the initiating part\n");
      return ERROR;
      }


   fd_set set;

   FD_ZERO(&set);
   FD_SET(sock, &set);
   int max = sock;

   while(1) {

   read_all_students(&head);

   fd_set duplicate = set;
    if (select(max + 1, &duplicate, 0, 0, 0) < 0) {

      fprintf(stderr,"An error in select\n");
      return ERROR;
      }
      for (int i = 0 ; i <=  max; i++) {

      if(FD_ISSET(i, &duplicate)) {

    int client = 0;
      if (i == sock) {

        struct sockaddr_storage my_client;

        socklen_t client_len = sizeof(my_client);

        client = accept(i, (struct sockaddr * )&my_client, &client_len) ;

        if (!ISVALID(client)) {

          fprintf(stderr,"There has been an error\n");
          return ERROR;
          }

             FD_SET(client, &set);
           if (client > sock) {

           max = client;
           }
         char address[1024];

     if (getnameinfo((struct sockaddr*)&client, client_len, address, sizeof(address) , 0 , 0 , NI_NUMERICHOST) < 0) {

        fprintf(stderr, "There has been an error in getting the address of the client\n");
        return -1;
        }
       printf("Now connecting to the client address: %s\n", address);
       } else {

        printf("Connected to a client\n");

    char read [ARB_BUF];

    int bytes = 0;
    rio_t rp;
    init(&rp, i);
    if ( (bytes = readlineb(&rp, read, sizeof(read))) == -1) {

     fprintf(stderr,"An error in getting initial information\n");
     return ERROR;
     }
     bytes = 0;

     student_t * node = NULL;



   bytes =   evaluate(&client, rp, node, read);

   int32_t  conv = htonl(bytes);

    written(i, &conv, sizeof(conv));
     }
     } /* for_loop */
     } /* while(1) */
    }
   CLOSESOCKET(sock);
      return 0;
    }
