#ifndef NETWORK_H
#define NETWORK_H

#define ISVALID(s) (s > 0)
#define CLOSESOCKET(s) (close(s))

#define  ERROR_CODE (-1)


#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/select.h>


#endif // NETWORK_h
