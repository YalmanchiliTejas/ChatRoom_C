#ifndef __RIO_H__
#define __RIO_H__

#define MAX_SIZE (8192)
typedef struct rio{

 int rio_fd; // the file descriptor
 int rio_cnt; // no.of unread bytes in the internal buffer

  char * bufr_ptr; // next unread byte in internal buffer

  char rio_buf[MAX_SIZE]; // the internal  buffer


 } rio_t;


size_t rio_readn(int , void * , size_t);
size_t rio_written(int , void * , size_t);
void init(rio_t * , int );
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

#endif

