#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

using namespace std;

#define OK 1
#define ERROR 0
#define OVERFLOW 0

typedef struct QNode  //队列中 结点 的数据结构类型
{
    char data[10];
    struct QNode *next;
    int flag;// whether this Node is empty, it can use or not
} QNode, *QueuePtr;

typedef struct  //队列的数据结构类型
{
    QueuePtr front;
    QueuePtr rear;
    int count;// the count of QueueItem
} LinkQueue;

//判断队列是否为空
bool Empty(LinkQueue &Q)
{
    if( !Q.front && !Q.rear )
        return true;
    else
        return false;
}

//初始化队列
int InitQueue(LinkQueue &Q)
{
    Q.front = Q.rear = 0;
    Q.count = 0;
    return OK;
}

//置队列为空
int DestoryQueue(LinkQueue &Q)
{
    while(Q.front)
    {
        Q.rear = Q.front ->next;
        free(Q.front);
        Q.front = Q.rear;
    }
    Q.count = 0;
    return OK;
}

//在队列的尾端插入元素，分为两种情况。1队列为空。2队列非空
int EnQueue(LinkQueue &Q, const char* e)
{
    if(Empty(Q) == false)  //非空
    {
        QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
        if(!p)
        {
            printf("新结点内存分配失败!");
            return ERROR;
        }
        memcpy(p->data, e, (int)strlen(e));
        p->next = NULL;
        Q.rear->next = p;
        Q.rear = p;
    }
    else
    {
        QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
        if(!p)
        {
            printf("新结点内存分配失败!");
            return ERROR;
        }
        memcpy(p->data, e, (int)strlen(e));
        p->next = NULL;
        Q.rear = p;
        Q.front = Q.rear;
    }
    return OK;
}

//删除队列队头的元素，并用e返回
int DeQueue(LinkQueue &Q, char* e)
{
    //如果队列为空
    if(Empty(Q))
    {
        printf("队列为空");
        return ERROR;
    }
    QueuePtr p;
    p = Q.front;
    memcpy(e, p->data, (int)strlen(p->data));
    Q.front = p->next;
    if(Q.rear == p)
        Q.rear = Q.front=0; //当队列最后一个元素被删除的时候，尾指针也丢失，所以要重新给尾指针赋值
    free(p);
    return OK;
}

char* Front(LinkQueue &Q)
{
    return Q.front->data;
}

int main()
{
    const char *h = "hello";
    char hh[10] = { 0 };
    int i;
    LinkQueue Q;
    InitQueue(Q);
    if(Empty(Q) == true) //判断队列是否为空
        printf("队列为空\n");
    else
        printf("队列不为空\n");
    //向队列中插入0 ~ 9 10个数字
    for(i = 0 ; i < 10; i++)
    {
        EnQueue(Q, h);
    }
    for(int j = 0 ; j < 10; j++)
    {
        printf("%s\n", Front(Q));
        if(Empty(Q))
            printf("队列为空\n");
        else
            printf("队列不为空\n");
        DeQueue(Q, hh);
    }
    DeQueue(Q, hh);
    for(i = 0 ; i < 10 ; i++)
    {
        EnQueue(Q, h);
    }
    DestoryQueue(Q);
    if(Empty(Q) == true)
        printf("队列为空\n");
    else
        printf("队列不为空\n");
    return 0;
}
