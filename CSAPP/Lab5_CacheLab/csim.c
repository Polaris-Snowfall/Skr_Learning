#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <getopt.h>

typedef struct CacheLine{
    int valid;
    unsigned long long time;
    long long tag;
}CacheLine;

CacheLine** Cache = NULL;
int groupcount = 0;
int blocksize = 0;
int s,E,b,t;
char verbose = 0;
int misses = 0;
int hits = 0;
int evictions = 0;

//因为不需要实际访问数据内容,所以不需要分配2^b字节的数据空间.
//只要地址位于某Set中且标志位相同,则hit



void Cache_Init(int s,int E,int b)
{
    groupcount = pow(2,s);
    blocksize = pow(2,b);
    Cache = (CacheLine**)malloc(sizeof(CacheLine*)*groupcount);
    for(int i = 0;i<groupcount;++i)
    {
        Cache[i] = (CacheLine*)malloc(sizeof(CacheLine)*E);
        for(int j = 0;j<E;++j)
        {
            Cache[i][j].valid = 0;
            Cache[i][j].time = 0;
        }
    }
}

void hit()
{
    if(verbose==1)
        printf(" hit");
    ++hits;
}

void miss()
{
    if(verbose==1)
        printf(" miss");
    ++misses;
}

void eviction()
{
    if(verbose==1)
        printf(" eviction");
    ++evictions;
}

void AccessMemory(long long addr,int size)
{
    //这个地方使用掩码提取而不是直接移位,是因为算术右移会使标记发生变化.
    long long mask;
    //注意制作掩码的时候的常数类型LL
    mask = (1LL << (s + b)) - 1;
    int Setindex = (mask & addr) >> b;
    // mask = (1LL << b) - 1;
    // int blockoffset = mask & addr;
    mask = (1LL << t) - 1;
    long long tag = (addr >> (s + b)) & mask;


    CacheLine* CacheSet = Cache[Setindex];
    int flag = 0;
    unsigned long long LRtime = CacheSet[0].time;
    int LRid = 0;
    for(int i = 0;i<E;++i)
    {
        if(CacheSet[i].valid==0)
        {
            miss();
            CacheSet[i].tag = tag;
            CacheSet[i].valid = 1;
            CacheSet[i].time = clock();
            flag = 1;
            break;
        }
        if(CacheSet[i].tag==tag)
        {
            hit();
            CacheSet[i].time = clock();
            flag = 1;
            break;
        }
        if(CacheSet[i].time<LRtime)
        {
            LRtime = CacheSet[i].time;
            LRid = i;
        }
    }
    if(flag==0)
    {
        miss();
        CacheSet[LRid].tag = tag;
        CacheSet[LRid].time = clock();
        eviction();
    }
}

int main(int argc,char* argv[])
{

    char* trace = NULL;
    int optc;
    while((optc = getopt(argc,(char* const *)argv,"vs:E:b:t:"))!=-1)
    {
        switch(optc)
        {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                trace = (char*)malloc(strlen(optarg)+1);
                strcpy(trace,optarg);
                break;
            case 'v':
                verbose = 1;
                break;                
        }
    }
    t = 64-s-b;

    Cache_Init(s,E,b);

    FILE* tracefile = fopen(trace,"r");
    char buf[50];

    char opt;
    long long addr;
    int size;
    while(1)
    {
        memset(buf,0,50);
        fgets(buf,50,tracefile);
        if(sscanf(buf," %c %llx,%d",&opt,&addr,&size)!=3)
            break;
        switch(opt)
        {
            case 'I':
                continue;
            case 'L':
            case 'S':
                if(verbose==1)
                {
                    printf("%c %llx,%d",opt,addr,size);
                    AccessMemory(addr,size);
                    printf(" \n");
                }
                else
                    AccessMemory(addr,size);
                break;
            case 'M':
                if(verbose==1)
                {
                    printf("%c %llx,%d",opt,addr,size);
                    AccessMemory(addr,size);
                    AccessMemory(addr,size);
                    printf(" \n");
                }
                else
                {
                    AccessMemory(addr,size);
                    AccessMemory(addr,size);
                }
                break;
        }
    }
    printSummary(hits,misses,evictions);

    free(trace);
    trace = 0;
    for(int i = 0;i<groupcount;++i)
    {
        free(Cache[i]);
        Cache[i] = NULL;
    }
    free(Cache);
    return 0;
}
