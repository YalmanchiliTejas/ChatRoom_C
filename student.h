#ifndef STUDENT_H
#define STUDENT_H

#define MAX_PASS_LEN (9)
#define ERROR_CODE_STU (-1)

typedef struct student{

int id;
char * username;
char [MAX_PASS_LEN + 1] password;
} student_t;
#endif // STUDENT_H


