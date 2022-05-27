#ifndef STUDENT_H
#define STUDENT_H


#include <sys/malloc.h>

#define MAX_PASS_LEN (9)
#define ERROR_CODE_STU (-1)
#define STUDENT_EXISTS (1);
#define NO_STUDENT (0)
typedef struct student{

int id;
char * username;
char  password [MAX_PASS_LEN];
char * major;
struct student * next;
struct student * prev;
} student_t;
#endif // STUDENT_H


