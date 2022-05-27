
#include "student.h"
#include "postgres.h"
#include "rio.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#define SUCCESS (0)
#define NEW_USER (1)
#define EXISTING (2)
#define ERROR (-2)
#define NON_EXIST (3)
#define WRONG_PASS (4)

/*
 * Prints out the initial welocme Menu
 */

int login_menu(char * username, char * password) {


  assert(username != NULL);
  assert(password != NULL);


  PGconn * conn = PQconnectdb("user=tejasyalamanchili dbname=new_student");

  if (PQstatus(conn) == CONNECTION_BAD) {

    FINISH(conn);

    fprintf(stderr,"%s\n", PQerrorMessage(conn));
    return -1;
  }

  PGresult * res = PQexec(conn, "SELECT * FROM chat");

  if(PQresultStatus(res) != PGRES_TUPLES_OK) {

    fprintf(stderr, "%s\n", PQerrorMessage(conn));
    CLEAR(res);
    FINISH(conn);
    return -1;
  }

  int rows = PQntuples(res);

  for(int i = 0; i < rows; i++) {

    if (strcmp(PQgetvalue(res, i , 1), username) == 0 && PQgetvalue(res, i , 2) == 0) {

      return EXISTING;
    }
  }
  CLEAR(res);
  return NON_EXIST;

} /* login_menu */


/*
 * This function enables a person to signup for the service
 */

int  sign_up(char * username, char * password) {


  assert(username != NULL);
  assert(password != NULL);

  PGconn * conn = PQconnectdb("user=tejasyalamanchili dbname=new_student");

  if (PQstatus(conn) == CONNECTION_BAD) {

    fprintf(stderr, "%s\n", PQerrorMessage(conn));
    FINISH(conn);
    return -1;
  }

  PGresult * res = PQexec(conn, "CREATE TABLE IF NOT EXISTS chat (Id PRIMARY KEY,"\
       "Username VARCHAR(100),"\
      "Password VARCHAR(9), Major VARCHAR(20))");

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {

    fprintf(stderr,"%s\n", PQerrorMessage(conn));

    CLEAR(res);
    FINISH(conn);
      return -1;

  }

  CLEAR(res);

  res = PQexec(conn, "SELECT * FROM chat");

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {

    fprintf(stderr, "%s\n", PQerrorMessage(conn));

    CLEAR(res);
    FINISH(conn);
    return -1;
  }

  int rows = PQntuples(res);

  for(int i = 0 ; i < rows; i++) {

    if ( strcmp(PQgetvalue(res, i , 1), username) == 0) {

      CLEAR(res);
      return EXISTING;
    }
    }
    
    CLEAR(res);
    return SUCCESS;
  }/* sign_up() */



  /*
   * Implementing init
   */

  void init(rio_t * rp, int fd) {

    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->bufr_ptr = rp->rio_buf;
  } /* init() */


  /*
   * Trying to implement robust io functions
   */

  size_t readn(int fd,  void * buffer, size_t len) {

    char *  buf = (char * ) buffer;

    size_t left = len;
    size_t nread ;

    /* This loop loop till we get to the end of the message */

    while(left > 0 ) {

      if ( (nread = read(fd, buf, left)) <  0) {

        if(nread == EINTR) { // if interrupted by any other signal call will call read() again
          nread = 0;
        }
        else {

          fprintf(stderr, "An error occurred while reading\n");
          return -1;
        }

      } else if (nread == 0) { // encountered an EOF
        break;
      }
      left -= nread;
      buf += nread; // moves the char pointer
    }

    return (size_t) (len - left);


  } /* readn () */


  /*
   * Trying to implement the written function
   */

  size_t written (int fd  , void * buffer, size_t len) {


    size_t left = len;

    size_t nwritten;

    char * buf = (char * )buffer;

    while(left > 0 ) {

      if ( (nwritten = write(fd, buf,left)) < 0 ) {

        if (nwritten == EINTR) {

          nwritten = 0;
        }
        else {
          fprintf(stderr, "There has been an error\n");
          return -1;
        }
      }
      else if (len == 0) {

        break; // came across an EOF
      }
      left -= nwritten;
      buf += nwritten;
    }
    return (size_t)(len - nwritten);

  } /* witten() */


  /*
   * trying to implement read
   * I will also try to understand it better :)
   * rio_read - This is a wrapper for the Unix read() function that
   *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
   *    buffer, where n is the number of bytes requested by the user and
   *    rio_cnt is the number of unread bytes in the internal buffer. On
   *    entry, rio_read() refills the internal buffer via a call to
   *    read() if the internal buffer is empty.
   */

  static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {
    size_t cnt;

    while (rp->rio_cnt <= 0) { /* Refill if buf is empty */
      rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
      if (rp->rio_cnt < 0) {
        if (errno != EINTR) {
          return -1; /* errno set by read() */
        }

        /* Interrupted by sig handler return, nothing to do */
      } else if (rp->rio_cnt == 0) {
        return 0; /* EOF */
      } else {
        rp->bufr_ptr = rp->rio_buf; /* Reset buffer ptr */
      }
    }

    /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
    cnt = n;
    if ((size_t)rp->rio_cnt < n) {
      cnt = (size_t)rp->rio_cnt;
    }
    memcpy(usrbuf, rp->bufr_ptr, cnt);
    rp->bufr_ptr += cnt;
    rp->rio_cnt -= cnt;
    return (ssize_t)cnt;
  }  /* rio_read() */

  /*
   * trying to implement readnb
   * I will also try to understand it better :)
   */

  size_t readnb(rio_t * rp , void * buffer, size_t len) {

    size_t nleft =len;
    ssize_t nread;
    char *bufp = buffer;

    while (nleft > 0) {
      if ((nread = rio_read(rp, bufp, nleft)) < 0) {
        return -1; /* errno set by read() */
      } else if (nread == 0) {
        break; /* EOF */
      }
      nleft -= (size_t)nread;
      bufp += nread;
    }
    return (ssize_t)(len - nleft); /* return >= 0 */
  } /* readnb() */


  /*
   * trying to implement readlinenb
   * I will also try to understand it better :)
   */

  ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
    size_t n;
    ssize_t rc;
    char c, *bufp = usrbuf;

    for (n = 1; n < maxlen; n++) {
      if ((rc = rio_read(rp, &c, 1)) == 1) {
        *bufp++ = c;
        if (c == '\n') {
          n++;
          break;
        }
      } else if (rc == 0) {
        if (n == 1) {
          return 0; /* EOF, no data read */
        } else {
          break; /* EOF, some data was read */
        }
      } else {
        return -1; /* Error */
      }
    }
    *bufp = 0;
    return (ssize_t)(n - 1);

  } /* rio_readlineb() */

