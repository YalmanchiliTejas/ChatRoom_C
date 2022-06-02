#include "postgres.h"

#include <stdlib.h>
#include <string.h>

int main() {


 char * people = malloc(sizeof(char) * 18);

 char read [3] = {'h', 'i', '\0'};

  strcpy(people, read);


PGconn * conn = PQconnectdb("user=tejasyalamanchili dbname=new_student");

if (PQstatus(conn) == CONNECTION_BAD) {

  printf("Unable to connect to a database\n");

  return -1;
  }


PGresult * res = PQexec(conn, "DROP TABLE IF EXISTS student");

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {

  printf("There has been an issue dropping the table\n");

  CLEAR(res);

  return -1;
  }

  CLEAR(res);

   res = PQexec(conn, "CREATE TABLE IF NOT EXISTS student (Id INTEGER "\
                       "PRIMARY KEY, Name VARCHAR(100))");

  if (PQresultStatus(res) != PGRES_COMMAND_OK){

    fprintf(stderr, "%s\n", PQerrorMessage(conn));
    CLEAR(res);

    return -1;

    }
    CLEAR(res);

    printf("Created a table");


  res = PQexec(conn, "INSERT INTO student VALUES(1,'Tejas')");

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {

      fprintf(stderr,"%s\n", PQerrorMessage(conn));
      CLEAR(res);

      return -1;
      }

      CLEAR(res);

      res = PQexec(conn, "INSERT INTO student VALUES(2, 'Vignesh')");

      if (PQresultStatus(res) != PGRES_COMMAND_OK) {

      fprintf(stderr, "%s\n", PQerrorMessage(conn));

      CLEAR(res);
      return -1;
      }

      CLEAR(res);

      printf("created and entered two new values\n");

#if 0
      res = PQexec(conn , "BEGIN");

      if (PQresultStatus(res) != PGRES_COMMAND_OK) {

       printf("error occurred\n");
      fprintf(stderr, "%s\n", PQerrorMessage(conn));

      CLEAR(res);
      return -1;
      }
      CLEAR(res);
#endif
      res = PQexec(conn , "SELECT * FROM student");

      if (PQresultStatus(res) != PGRES_TUPLES_OK) {

         printf("reached the second error\n");

         fprintf(stderr, "%s\n", PQerrorMessage(conn));
         return -1;
         }

       printf("reached here\n");
      int rows = PQntuples(res);

      printf("%d\n", rows);

      CLEAR(res);

      printf("Are the number of the rows that are present\n");
 

    return 0;
    }

