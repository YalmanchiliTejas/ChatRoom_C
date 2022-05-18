
#include "student.h"
#include "postgres.h"

#define QUIT (0)
#define NEW_USER (1)
#define EXISTING (2)
#define ERROR (-2)
#define 
/*
 * Prints out the initial welocme Menu
 */

 int first_menu(){

 printf("Welcome to the CHAT_APP!!!\n _______---___----___---_____\n");
 printf("Created By Tejas Yalamanchili(v 0.0.1)____---___---___---__\n");
 printf("Please choose one of the values from 0-2\n");

  printf("0) Quit\n");
  printf("1) Login\n");
  printf("2) Sign Up?\n");
   int value = 0;

   scanf("%d", &value);
   getchar();

  if (value != QUIT && value != EXISTING && value != NEW_USER) {

    return ERROR;
    }
    return value;
    } /* first_menu() */


 /*
  * This function is when the user selects the login menu
  */

  student_t*  login_menu() {

   char read[1024];
   printf("Enter Username:\n");

   char *username = NULL;
    scanf("%1023[^\n]\n", read);
    getchar();
    read[1023] = '\0';

     printf("Enter password:\n");

     char pass[9] = {0};

     scanf("%8[^\n]\n", pass);
     getchar();
     pass[8] = '\0';

     student_t *  new_student = malloc(sizeof(student_t));
     assert(new_student != NULL);
     new_student->username = malloc(strlen(read));
     assert(new_student->username != NULL);
     strcpy(new_student->username, read);

     new_student->username[sizeof(username) - 1] = '\0';
     strcpy(new_student->password, pass);

      new_student->password[sizeof(new_student->password)] = '\0';

      PGconn * conn = PQconnectdb("user = tejasyalamanchili dbname = new_student");

      if (PQstatus(conn) == CONNECTION_BAD) {

      fprintf(stderr, "There has been an error in connecting to a database\n");

      FINISH(conn);
      return CONNECTION_ERROR;
      }
    PGresult * res = PQexec(conn, "BEGIN");

    if ( PQresultStatus(res) != PGRES_ COMMAND_OK) {


      fprintf(stderr, "BEGIN command failed \n");
      CLEAR(res);
      FINSIH(conn);
      return CONNECTION_ERROR;
      }

  int rows = PQntuples(res);
 




      return new_student;
      } /* login_menu */





