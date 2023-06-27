#ifndef SBUF_H
#define SBUF_H

#include <semaphore.h>
#include <stdlib.h>

typedef struct {
    int* buf;
    int count;
    int front;  //buf[(front+1)%count]为第一个item
    int rear;   //buf[rear%count]为最后一个元素
    sem_t mutex;
    sem_t slots;
    sem_t items;
}sbuf_t;

void sbuf_init(sbuf_t* sp,int n);
void sbuf_deinit(sbuf_t* sp);
void sbuf_insert(sbuf_t* sp,int item);
int sbuf_remove(sbuf_t* sp);

#endif