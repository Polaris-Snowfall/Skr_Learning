/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Polaris_Snowfall",
    /* First member's full name */
    "翰青HanQi",
    /* First member's email address */
    "811614792@qq.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};


/********************************************************
 * 运行相关宏(注释为原定数据,但由于没有服务器的实验环境,为求空间利用率最大进行了更改)
*********************************************************/
#define NDEBUG
#define TOPSIZE 0x10000 //0x10000
#define FASTBINS 7    //7
#define BINS 8       //8


/********************************************************
 * 机器相关,对齐相关宏
*********************************************************/

#define SIZE_SZ sizeof(size_t)

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 2*SIZE_SZ

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define ISALIGN(size) (!(size & (ALIGNMENT - 1-1))) //最低位不检测

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
#define MIN_CHUNKSIZE 4*SIZE_SZ

#define MAX_FASTBIN_SZ ((4+2*(FASTBINS-1))*SIZE_SZ)
/********************************************************
 * 堆块结构(参考glibc中ptmalloc实现)
 
 * 空闲堆块
  * +**************************+
 * |      prev_size           |   -->此区域仅当前一个块空闲时使用,
 * |    chunk_size|00         |      若前一个块已分配,则作为前一块的user_data
 * +**************************+
 * |        fd                |
 * |      undefined           |  -->预留空间,可以做成fd_nextsize之类的提高效率
 * |       (padding)          |
 * +**************************+
 
 * 已分配的堆块
 * +**************************+
 * |      prev_size           |
 * |    chunk_size|00         |    -->末尾为标记位,最后一个表示前一个块是否分配
 * +**************************+         即prev_inuse
 * |       user_data          |
 * |        .......           |
 * |        .......           |
 * +**************************+


*********************************************************/

struct malloc_chunk
{
    size_t prev_size;
    size_t size;
    struct malloc_chunk* fd;
    struct malloc_chunk* undefined;
};

typedef struct malloc_chunk* mchunkptr;




/********************************************************
 * 堆操作相关宏
*********************************************************/
#define chunk2mem(p) ((void*)((char*)(p)+2*SIZE_SZ))
#define mem2chunk(p) ((mchunkptr)((char*)(p)-2*SIZE_SZ))
// #define PACK(size,alloc) ((size)|(alloc))
#define PACK(size,alloc) ((alloc)?((size)|(alloc)):((size)&(~1)))
#define request2size(bytes) (ALIGN(bytes)+(2*SIZE_SZ))

#define chunk_at_offset(p,s) ((mchunkptr)((char*)(p)+(s)))
#define next_chunk(p) ((mchunkptr)((char*)(p)+((p)->size & ~3)))
#define prev_chunk(p) ((mchunkptr) (((char *) (p)) - ((p)->prev_size)))

#define IFinuse(p) ((next_chunk(p)->size)&1)
#define IFprev_inuse(p) ((p)->size&1)
/********************************************************
 * 用于Debug的assert宏
*********************************************************/

#ifdef NDEBUG
#define DBG(expr) ((void)0)
#define count() ((void)0)
#define dbg(x) ((void)0)
#else
#define DBG(expr) assert(expr)
mchunkptr dbg_var = NULL;
int count = 0;
#define dbg(x) (dbg_var=(x))
#define count() ++count;
#endif

//glibc的assert宏
// #define __STRING(x)	#x
// #ifndef NDEBUG
// #define assert(expr) ((void)0)
// #else
// #define assert(expr) \
//     (expr)?((void)0):mm_assert(__STRING(x))
// #endif


/********************************************************
 * 全局变量
*********************************************************/
mchunkptr top; 

/*
*/
mchunkptr fastbins[FASTBINS];
mchunkptr bins[BINS];
int fastbins_count[FASTBINS] = {0};
int bins_count[BINS] = {0};


/*
 打印错误信息
*/
void msg_error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(-1);
}


/*
获取bin的序号
*/
inline int size2index(size_t size)
{
    //使用fastbins
    if(size <= 4*SIZE_SZ)
        return 0;
    if(size <= MAX_FASTBIN_SZ)
        return (size/SIZE_SZ-4)/2;
    //使用bins
    size /= MAX_FASTBIN_SZ;
    int i = 0;
    while(1)
    {
        if(size>>=1)
            ++i;
        else
            break;
    }
    if(i>=BINS)
        i = BINS-1;
    return i;
}

void mm_unlink(mchunkptr mptr)
{
    if(mptr==NULL)
        msg_error("mm_unlink: invalid mptr");
    int idx = size2index(mptr->size);
    if(mptr->size<=MAX_FASTBIN_SZ)
    {
        if(fastbins[idx]!=NULL)
        {
            mchunkptr bck = fastbins[idx];
            mchunkptr fwd = fastbins[idx]->fd;
            if(bck==mptr)
            {
                fastbins[idx] = bck->fd;
                --fastbins_count[idx];
                return;
            }
            while(fwd!=NULL)
            {
                if(fwd==mptr)
                {
                    bck->fd = fwd->fd;
                    --fastbins_count[idx];
                    return;
                }
                else
                {
                    bck = fwd;
                    fwd = fwd->fd;
                }
            }
        }

        msg_error("mm_unlink: nonexistent mptr");
    }
    else
    {
        if(bins[idx]!=NULL)
        {
            mchunkptr bck = bins[idx];
            mchunkptr fwd = bins[idx]->fd;
            if(bck==mptr)
            {
                bins[idx] = bck->fd;
                --bins_count[idx];
                next_chunk(bck)->size = PACK(next_chunk(bck)->size,1);
                return;
            }
            while(fwd!=NULL)
            {
                if(fwd==mptr)
                {
                    next_chunk(fwd)->size = PACK(next_chunk(fwd)->size,1);
                    bck->fd = fwd->fd;
                    --bins_count[idx];
                    return;
                }
                else
                {
                    bck = fwd;
                    fwd = fwd->fd;
                }
            }
        }

        msg_error("mm_unlink: nonexistent mptr");
    }
}

void mm_expandtop()
{
    if((mem_sbrk(TOPSIZE)==(void*)-1))
            msg_error("mm_expandtop: sbrk error\n");
    top->size += TOPSIZE;
}


/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    top = NULL;
    for(int i = 0;i < FASTBINS;++i)
    {
        fastbins[i] = NULL;
        fastbins_count[i] = 0;
    }
    for(int i = 0;i< BINS;++i)
    {
        bins[i] = NULL;
        bins[i] = 0;
    }
    if((top = mem_sbrk(TOPSIZE))== (void*)-1)
        msg_error("mm_init: sbrk error\n");
    top->size = PACK(TOPSIZE,1);
    return 0;
}


/*
    mmm_malloc:mm_malloc的实际实现
    之所以封装一层是因为实现中使用了递归
    但不清楚递归调用mm_malloc接口是否会对评分造成影响
*/
void* mmm_malloc(size_t size)
{
    size_t nsize = request2size(size);
    mchunkptr mresult = 0;
    

    int idx = size2index(nsize);
    if(nsize<=MAX_FASTBIN_SZ)
    {
        if(fastbins[idx]!=NULL)
        {
            mresult = fastbins[idx];
            fastbins[idx] = fastbins[idx]->fd;
            --fastbins_count[idx];
            // next_chunk(mresult)->size = PACK(next_chunk(mresult)->size,1);

            return chunk2mem(mresult);
        }
    }
    else
    {
        if(bins[idx]!=NULL)
        {
            if(nsize<=bins[idx]->size)
            {
                mresult = bins[idx];
                bins[idx] = bins[idx]->fd;
                next_chunk(mresult)->size = PACK(next_chunk(mresult)->size,1);
                --bins_count[idx];

                if(nsize<=mresult->size-MIN_CHUNKSIZE)
                {
                    size_t nnextsize = mresult->size-nsize;
                    mresult->size = PACK(nsize,IFprev_inuse(mresult));
                    mchunkptr nextchunk = next_chunk(mresult);
                    nextchunk->size = PACK(nnextsize,1);
                    mm_free(chunk2mem(nextchunk));

                }
                return chunk2mem(mresult);
            }
            mchunkptr bck,fwd;
            bck = bins[idx];
            fwd = bins[idx]->fd;
            while(fwd!=NULL)
            {
                if(nsize<=fwd->size)
                {
                    mresult = fwd;
                    bck->fd = fwd->fd;
                    next_chunk(mresult)->size = PACK(next_chunk(mresult)->size,1);

                    --bins_count[idx];
                    if(nsize<=fwd->size-MIN_CHUNKSIZE)
                    {
                        size_t nnextsize = mresult->size-nsize;
                        mresult->size = PACK(nsize,IFprev_inuse(mresult));
                        mchunkptr nextchunk = next_chunk(mresult);
                        nextchunk->size = PACK(nnextsize,1);
                        mm_free(chunk2mem(nextchunk));

                    }
                    return chunk2mem(mresult);
                }
                else
                {
                    bck = fwd;
                    fwd = fwd->fd;
                }
            }
        }

    }

    if(nsize<=(top->size-MIN_CHUNKSIZE))
    {
        size_t old_topsize = top->size;
        mresult = top;
        top = (mchunkptr)((char*)top+nsize);
        top->size = old_topsize-nsize;
        mresult->size = PACK(nsize,1);

        return chunk2mem(mresult);
    }
    else
    {
        mm_expandtop();
        mmm_malloc(size);
    }
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
// void *mm_malloc(size_t size)
// {
//     int newsize = ALIGN(size + SIZE_T_SIZE);
//     void *p = mem_sbrk(newsize);
//     if (p == (void *)-1)
// 	return NULL;
//     else {
//         *(size_t *)p = size;
//         return (void *)((char *)p + SIZE_T_SIZE);
//     }
// }


void *mm_malloc(size_t size)
{
    return mmm_malloc(size);
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    mchunkptr mptr = mem2chunk(ptr);
    if(!ISALIGN(mptr->size)||mptr->size<MIN_CHUNKSIZE)
        msg_error("mm_free: invalid size");
    int idx = size2index(mptr->size);
    
    //fastbin不参与合并也不排序,直接从链表头部插入
    if(mptr->size<=MAX_FASTBIN_SZ)
    {
        mptr->fd = fastbins[idx];
        fastbins[idx] = mptr;
        ++fastbins_count[idx];
    }
    else
    {
        mchunkptr bck,fwd;
        mchunkptr nextchunk;
        mchunkptr prevchunk;

        //向上合并
        if(!IFprev_inuse(mptr))
        {
            prevchunk = prev_chunk(mptr);
            mm_unlink(prevchunk);
            prevchunk->size += PACK(mptr->size,0);
            mptr = prevchunk;
        }

        nextchunk = next_chunk(mptr);

        // 向下合并
        if(nextchunk==top)
        {
               mptr->size += PACK(nextchunk->size,0);
                top = mptr;
                return;
                //因为topchunk合并后不会被链入bin中,直接在此处返回.
        }
        if(!IFinuse(nextchunk))
        {
            mm_unlink(nextchunk);
            mptr->size += PACK(nextchunk->size,0);
            //nextchunk的prev_inuse/size位在链入bin中时再设置

        }

        //重新计算合并后的idx
        idx = size2index(mptr->size);

        //对应bin为空或插入位置在bin的头部
        if(bins[idx]==NULL||mptr->size<=bins[idx]->size)
        {
            mptr->fd = bins[idx];
            bins[idx] = mptr;
            ++bins_count[idx];
            nextchunk = next_chunk(mptr);
            nextchunk->size = PACK(nextchunk->size,0);
            nextchunk->prev_size = PACK(mptr->size,0);
        }
        else
        {
            bck = bins[idx];
            fwd = bins[idx]->fd;
            while(fwd!=NULL)
            {
                if(mptr->size<=fwd->size)
                {
                    break;
                }
                else
                {
                    bck = fwd;
                    fwd = fwd->fd;
                }
            }
            mptr->fd = fwd;
            bck->fd = mptr;
            ++bins_count[idx];
            nextchunk = next_chunk(mptr);
            nextchunk->size = PACK(nextchunk->size,0);
            nextchunk->prev_size = PACK(mptr->size,0);
        }
    }
    return;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    size_t nsize;
    mchunkptr mptr;
    mchunkptr nextchunk;

    if(size==0)
    {
        mm_free(ptr);
        return NULL;
    }
    if(size<MIN_CHUNKSIZE)
    {
        msg_error("mm_realloc: invalid size");
    }
    if(ptr==NULL)
    {
        newptr = mm_malloc(size);
        return newptr;
    }
    nsize = request2size(size);
    mptr = mem2chunk(ptr);
    nextchunk = next_chunk(mptr);

    //若nsize比当前块小且切割后至少能剩下一个最小堆块,释放
    if(nsize<=mptr->size-MIN_CHUNKSIZE)
    {
        size_t nnextsize = mptr->size-nsize;
        mptr->size = PACK(nsize,IFprev_inuse(mptr));
        nextchunk = next_chunk(mptr);
        nextchunk->size = PACK(nnextsize,1);
        mm_free(chunk2mem(nextchunk));
        return chunk2mem(mptr);
    }
    //nsize比当前块小但无法切割出一个最小块,直接返回
    else if(nsize <= mptr->size)
    {

        return chunk2mem(mptr);
    }
    else
    {
            //fastbin中空闲块不参与拓展
        if(PACK(nextchunk->size,0)>MAX_FASTBIN_SZ)
        {
            if(!IFinuse(nextchunk))
            {
                if(nsize==PACK(nextchunk->size,0)+PACK(mptr->size,0))
                {
                    if(nextchunk!=top)
                    {
                        mm_unlink(nextchunk);
                        // next_chunk(nextchunk)->prev_size = nsize;
                        next_chunk(nextchunk)->size = PACK(next_chunk(nextchunk)->size,1);
                        mptr->size = PACK(nsize,IFprev_inuse(mptr));
                    }
                    else
                    {
                        mm_expandtop();
                        return mm_realloc(ptr,size);
                    }
                    newptr = chunk2mem(mptr);
    
                }
                else if(nsize<=PACK(nextchunk->size,0)+PACK(mptr->size,0)-MIN_CHUNKSIZE)
                {
                    size_t nnextsize = PACK(nextchunk->size,0)+PACK(mptr->size,0)-nsize;
                    mptr->size = PACK(nsize,IFprev_inuse(mptr));
                    if(nextchunk!=top)
                    {
                        mm_unlink(nextchunk);
                        nextchunk = next_chunk(mptr);
                        nextchunk->size = PACK(nnextsize,1);
                        mm_free(chunk2mem(nextchunk));
                    }
                    else
                    {
                        nextchunk = next_chunk(mptr);
                        nextchunk->size = PACK(nnextsize,1);
                        top = nextchunk;
                    }

                    newptr = chunk2mem(mptr);
                }
                else
                {
                    newptr = mm_malloc(size);
                }
            }
            else
            {
                newptr = mm_malloc(size);
            }
        }
        else
        {
            newptr = mm_malloc(size);
        }

        if (newptr == NULL)
            return NULL;
        copySize = mem2chunk(ptr)->size;
        if (size < copySize)
        copySize = size;
        memcpy(newptr, oldptr, copySize);
        if(oldptr!=newptr)
            mm_free(oldptr);

        return newptr;
    }


}














