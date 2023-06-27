#include <stdio.h>
#include "csapp.h"
#include  "sbuf.h"
#include "cache.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define MAX_CACHE_LINE 100

#define MAX_PORTLEN 10
#define MAX_HEADERS 30

#define NTHREADS 4
#define SBUFSIZE 16

#define PREFIX_PRINT "HanQi_Proxy > "
/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3";


typedef struct 
{
    char method[MAXLINE];
    char host[MAXLINE];
    char port[MAX_PORTLEN];
    char path[MAXLINE];
    char cgiargs[MAXLINE];
    char version[MAXLINE];
}RequestLine;

typedef struct 
{
    char name[MAXLINE];
    char value[MAXLINE];
}RequestHeader;


sbuf_t sbuf;
Cache cache;

//更新read_cnt的互斥锁
sem_t cache_mutex;

//阻塞写的互斥锁
sem_t cache_w_mutex;

//读者更新lru的互斥锁
sem_t cache_lru_mutex;

size_t cache_read_cnt = 0;