#ifndef __RIO_H__
#define __RIO_H__

#define MAX_SIZE (8192)
#include "student.h"

typedef struct rio{

 int rio_fd; // the file descriptor
 int rio_cnt; // no.of unread bytes in the internal buffer

  char * bufr_ptr; // next unread byte in internal buffer

  char rio_buf[MAX_SIZE]; // the internal  buffer


 } rio_t;


size_t readn(int , void * , size_t);
size_t written(int , void * , size_t);
void init(rio_t * , int );
ssize_t readnb(rio_t *, void *, size_t );
ssize_t readlineb(rio_t *, void *, size_t );
int login_menu(char * , char * );
int  sign_up(char * , char * );
void add_student(student_t ** , student_t * );
int read_all_students(student_t **);
int delete_student(student_t ** , student_t *);

#endif

