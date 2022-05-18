#ifndef __POSTGRES_H__
#define __POSTGRES_H__

#define CONNECTION_ERROR (-3)
#define FINISH(C) (PQfinish(C))
#define CLEAR(S) (PQclear(S))


#endif // __POSTGRES_H__
