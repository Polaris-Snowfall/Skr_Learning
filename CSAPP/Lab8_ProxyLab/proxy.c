#include "proxy.h"


void parse_uri(char uri[],RequestLine* requestline)
{
    char* address_ptr = NULL;
    char* port_ptr = NULL;
    char* path_ptr = NULL;

    if(address_ptr = strstr(uri,"//"))
        address_ptr += 2;
    else
        address_ptr = uri;
    if(port_ptr = strstr(address_ptr,":"))
        port_ptr += 1;
    path_ptr = strstr(address_ptr,"/");

    if(path_ptr!=NULL)
        strncpy(requestline->path,path_ptr,MAXLINE);
    else
    {
        strncpy(requestline->path,"/",2);
        path_ptr = address_ptr+strlen(address_ptr);
    }
    if(port_ptr!=NULL)
        strncpy(requestline->port,port_ptr,path_ptr-port_ptr);
    else
    {
        strncpy(requestline->port,"80",3);
        port_ptr = path_ptr;
    }
    strncpy(requestline->host,address_ptr,port_ptr-address_ptr-1);
    printf(PREFIX_PRINT"RequestTarget: %s:%s%s\n",requestline->host,requestline->port,requestline->path);
}



int read_requesthdrs(rio_t* rio,RequestHeader requestheaders[MAX_HEADERS])
{
    int i = 0;
    char buf[MAXLINE];
    Rio_readlineb(rio,buf,MAXLINE);
    while(strcmp("\r\n",buf))
    {
        sscanf(buf,"%[^:]: %s\r\n",requestheaders[i].name,requestheaders[i].value);
        // printf("%s: %s\r\n",requestheaders[i].name,requestheaders[i].value);
        ++i;
        if(i==MAX_HEADERS)
            break;
        Rio_readlineb(rio,buf,MAXLINE);
    }
    return i;
}

void add_headers(RequestHeader requestheaders[MAX_HEADERS],int* headers_num,char* headername,char* headervalue)
{
    for(int i = 0;i<MAX_HEADERS;++i)
    {
        if(!strcmp(headername,requestheaders[i].name))
        {
            strncpy(requestheaders[i].value,headervalue,MAXLINE);
            return;
        }
    }
    strncpy(requestheaders[*headers_num].name,headername,MAXLINE);
    strncpy(requestheaders[*headers_num].value,headervalue,MAXLINE);
    ++(*headers_num);
    return;
}

int add_cache(int server_fd,int connfd,RequestHeader requestheaders[],int headers_num,char* uri)
{
    //保存一份临时拷贝,全部查找完毕后再复制到Cache中,减少上锁的时间.
    char content_type[MAXLINE];
    char server[MAXLINE];
    char buf[3*MAXLINE+5];
    size_t content_lenth = 0;
    int flag = 2;
    size_t size = 0;
    int i = 0;
    rio_t server_rio;

    printf(PREFIX_PRINT"Adding to cache\n");

    Rio_readinitb(&server_rio,server_fd);

    for(int j = 0;j<headers_num;++j)
    {
        // if(!flag)
        //     break;
        if(!strcmp(requestheaders[j].name,"Content-type"))
        {
            memcpy(content_type,requestheaders[j].value,MAXLINE);
            --flag;
            continue;
        }
        if(!strcmp(requestheaders[j].name,"Content-length"))
        {
            content_lenth = strtol(requestheaders[j].value,NULL,10);
            --flag;
            continue;
        }
        // if(!strcmp(requestheaders[j].name,"Server"))
        // {
        //     memcpy(server,requestheaders[j].value,MAXLINE);
        //     continue;
        // }
    }

    if(content_lenth>=MAX_OBJECT_SIZE||content_lenth==0||flag)
    {
        printf(PREFIX_PRINT"failed to cache\n");
        return 1;
    }
    else
    {
        //转发请求报头
        for(int j = 0;j<headers_num;++j)
        {
            snprintf(buf,3*MAXLINE+5,"%s: %s\r\n",requestheaders[j].name,requestheaders[j].value);
            printf("%s",buf);
            Rio_writen(connfd,buf,strlen(buf));
        }
        Rio_writen(connfd,"\r\n",2);
        printf("\r\n");

        sem_wait(&cache_w_mutex);
        //如果本次加入的缓存内容过大,可能需要牺牲多个缓存.
        while(cache.cache_num>=MAX_CACHE_LINE||cache.cache_size+content_lenth+1>MAX_CACHE_SIZE)
        {
            int i,index;
            size_t min_lru = (size_t)-1;
            //因为一次可能牺牲多个缓存,这里不能用cache_num来遍历
            for( i = 0;i<MAX_CACHE_LINE;++i)
            {
                if(cache.cachelines[i].size>0&&cache.cachelines[i].lru<min_lru)//也许应该加个等号
                {
                    min_lru = cache.cachelines[i].lru;
                    index = i;
                }
            }
            free(cache.cachelines[index].data);
            cache.cache_size -= cache.cachelines[index].size;
            cache.cachelines[index].size = 0;
            cache.cachelines[index].uri[0] = '\0';
            cache.cache_num--;
        }
        sem_post(&cache_w_mutex);

        char* data = malloc(content_lenth+1);
        while(size = Rio_readnb(&server_rio,data,content_lenth+1))
        {
            printf("%s",data);
            Rio_writen(connfd,data,size);
        }
        
        sem_wait(&cache_w_mutex);
                
        for( i = 0;i<MAX_CACHE_LINE;++i)
        {
            if(cache.cachelines[i].size==0)
                break;
        }

        //填入cacheline
        cache.cachelines[i].data = malloc(content_lenth+1);
        memcpy(cache.cachelines[i].data,data,content_lenth+1);
        memcpy(cache.cachelines[i].content_type,content_type,MAXLINE);
        memcpy(cache.cachelines[i].uri,uri,MAXLINE);
        memcpy(cache.cachelines[i].server,server,MAXLINE);
        cache.cachelines[i].size = content_lenth+1;
        cache.cachelines[i].lru = cache.current_lru++;

        //修改cache数据

        ++(cache.current_lru);
        ++(cache.cache_num);
        cache.cache_size += content_lenth+1;

        sem_post(&cache_w_mutex);
        return 0;
    }
}

int Cache2Client(int connfd,char* uri,RequestHeader requestheaders[],int headers_num)
{
    int i;
    printf(PREFIX_PRINT"Trying forward to client from cache,ing...\n");
    char buf[3*MAXLINE+5];

    sem_wait(&cache_mutex);
    ++cache_read_cnt;
    if(cache_read_cnt==1)
        sem_wait(&cache_w_mutex);
    sem_post(&cache_mutex);

    for( i = 0;i<MAX_CACHE_LINE;++i)
    {
        if(cache.cachelines[i].size!=0&&!strcmp(cache.cachelines[i].uri,uri))
        {
            sem_wait(&cache_lru_mutex);
            cache.cachelines[i].lru = cache.current_lru++;
            sem_post(&cache_lru_mutex);
            break;
        }
    }
    //缓存不命中,返回1
    if(i==MAX_CACHE_LINE)
    {
        printf(PREFIX_PRINT"Cache miss!\n");
        sem_wait(&cache_mutex);
        --cache_read_cnt;
        if(cache_read_cnt==0)
            sem_post(&cache_w_mutex);
        sem_post(&cache_mutex);
        return 1;
    }


    printf(PREFIX_PRINT"Cache hit!\n");
    
    //开始转发请求报头
    sem_wait(&cache_mutex);
    printf("%s", "HTTP/1.0 200 OK\r\n");
    Rio_writen(connfd, "HTTP/1.0 200 OK\r\n",17);
    
    // if(cache.cachelines[i].server!=NULL)
    // {
    //     strncpy(buf,"Server: ",MAXLINE);
    //     strncat(buf,cache.cachelines[i].server,MAXLINE);
    //     strcat(buf,"\r\n");
    //     Rio_writen(connfd,buf,strlen(buf));
    //     write(1,buf,strlen(buf));
    // }

    snprintf(buf,3*MAXLINE+5,"%s: %lu\r\n","Content-lenth",cache.cachelines[i].size-1);
    Rio_writen(connfd,buf,strlen(buf));
    printf("%s: %lu\r\n","Content-lenth",cache.cachelines[i].size-1);//转发的时候不转发末位的阶段符

    snprintf(buf,3*MAXLINE+5,"%s: %s\r\n","Content-type",cache.cachelines[i].content_type);
    Rio_writen(connfd,buf,strlen(buf));
    printf("%s: %s\r\n","Content-type",cache.cachelines[i].content_type);

    Rio_writen(connfd,"\r\n",2);
    printf("\r\n");

    //开始转发数据内容
    Rio_writen(connfd,cache.cachelines[i].data,cache.cachelines[i].size-1);
    write(1,cache.cachelines[i].data,cache.cachelines[i].size);

    --cache_read_cnt;
    if(cache_read_cnt==0)
        sem_post(&cache_w_mutex);
    sem_post(&cache_mutex);

    return 0;
}

void Write2Server(int server_fd,RequestLine* requestline,RequestHeader requestheaders[],int headers_num)
{
    printf(PREFIX_PRINT"Forwarding to %s:%s,ing...\n",requestline->host,requestline->port);
    char buf[3*MAXLINE+5];

    //这里不是很懂,最后转发给服务器的uri只留下文件路径?
    snprintf(buf,3*MAXLINE+5,"%s %s %s\r\n",requestline->method,requestline->path,requestline->version);    
    printf("%s",buf);
    Rio_writen(server_fd,buf,strlen(buf));

    for(int i = 0;i<headers_num;++i)
    {
        snprintf(buf,3*MAXLINE+5,"%s: %s\r\n",requestheaders[i].name,requestheaders[i].value);
        printf("%s",buf);
        Rio_writen(server_fd,buf,strlen(buf));
    }
    Rio_writen(server_fd,"\r\n",2);
    printf("\r\n");
}

void Forward2Client(int client_fd,int server_fd,char* uri)
{
    int headers_num = 0;
    char buf[2*MAXLINE+5];
    rio_t server_rio;
    RequestHeader requestheaders[MAX_HEADERS];
    int size = 0;

    printf(PREFIX_PRINT"Forwarding to Client,ing...\n");

    Rio_readinitb(&server_rio,server_fd);
    Rio_readlineb(&server_rio,buf,2*MAXLINE+5);
    printf("%s",buf);
    Rio_writen(client_fd,buf,strlen(buf));

    headers_num = read_requesthdrs(&server_rio,requestheaders);

    if(add_cache(server_fd,client_fd,requestheaders,headers_num,uri))
    {
        while(size = Rio_readlineb(&server_rio,buf,MAXLINE))
        {
            printf("%s",buf);
            Rio_writen(client_fd,buf,size);
        }
    }
    
}

void* StartWork(void* vargp)
{
    int connfd;
    char buf[MAXLINE],uri[MAXLINE];
    RequestLine requestline;
    RequestHeader requestheaders[MAX_HEADERS];
    rio_t client_rio;
    int server_fd;

    pthread_detach(pthread_self());

    while(1)
    {
        connfd = sbuf_remove(&sbuf);

        Rio_readinitb(&client_rio,connfd);
        if(!Rio_readlineb(&client_rio,buf,MAXLINE))
            return NULL;
        printf("%s",buf);
        //writeup中提到这里有个多行请求行的问题,待处理
        /*
        Ideally  your  HTTP  request  parser  will  be  fullyrobust according to the relevant sections of RFC 1945, except for one detail: while the specification allowsfor multiline request fields
        */
        sscanf(buf,"%s %s %s",requestline.method,uri,requestline.version);
        int headers_num = read_requesthdrs(&client_rio,requestheaders);

        parse_uri(uri,&requestline);

        if(Cache2Client(connfd,uri,requestheaders,headers_num)) //缓存不命中时会返回1,命中返回0.
        {
            add_headers(requestheaders,&headers_num,"Host",requestline.host);
            add_headers(requestheaders,&headers_num,"User-Agent",user_agent_hdr);
            add_headers(requestheaders,&headers_num,"Connection","close");
            add_headers(requestheaders,&headers_num,"Proxy-Connection","close");

            server_fd = open_clientfd(requestline.host,requestline.port);
            printf(PREFIX_PRINT"Connect to %s:%s.(serverFd: %d)\n",requestline.host,requestline.port,server_fd);

            Write2Server(server_fd,&requestline,requestheaders,headers_num);
            Forward2Client(connfd,server_fd,uri);
        }

        close(connfd);
        close(server_fd);
    }

    return NULL;
}

void cache_init()
{
    sem_init(&cache_mutex,0,1);
    sem_init(&cache_w_mutex,0,1);
    sem_init(&cache_lru_mutex,0,1);
    cache.cachelines = calloc(MAX_CACHE_LINE,sizeof(CacheLine));
    cache.cache_num = 0;
    cache.cache_size = 0;
    cache.current_lru = 0;
}

int main(int argc,char* argv[])
{
    int listenfd,connfd;
    char client_hostname[MAXLINE];
    char client_port[MAX_PORTLEN];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    if(argc!=2)
    {
        fprintf(stderr,PREFIX_PRINT"usage: %s <port>\n",argv[0]);
        exit(1);
    }

    sbuf_init(&sbuf,SBUFSIZE);
    cache_init();

    for(int i = 0;i<NTHREADS;++i)
        pthread_create(&tid,NULL,StartWork,NULL);
    
    listenfd = open_listenfd(argv[1]);
    while(1)
    {
        clientlen = sizeof(clientaddr);
        connfd = accept(listenfd,(SA*)&clientaddr,&clientlen);        
        Getnameinfo((SA*)&clientaddr,clientlen,client_hostname,MAXLINE,client_port,MAX_PORTLEN,0);
        printf(PREFIX_PRINT"Accepted connection from (%s,%s),clientFd: %d\n",client_hostname,client_port,connfd);

        sbuf_insert(&sbuf,connfd);
    }

    return 0;
}
