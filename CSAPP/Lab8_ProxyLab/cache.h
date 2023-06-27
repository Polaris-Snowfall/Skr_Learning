#ifndef CACHE_H
#define CACHE_H
#include "csapp.h"
typedef struct 
{
    char uri[MAXLINE];
    char content_type[MAXLINE];
    char* data;
    char server[MAXLINE];
    size_t size;
    size_t lru;
}CacheLine;

typedef struct
{
    CacheLine* cachelines;
    size_t current_lru;
    size_t cache_num;
    size_t cache_size;
}Cache;


#endif