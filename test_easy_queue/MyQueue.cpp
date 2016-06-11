#include <iostream>
#include <string>
#include <stdio.h>

#define QueueSize (10)

typedef char Data;

class CirQueue{
    public:
    CirQueue();
    virtual ~CirQueue();

    void InfoQueue();
    void InitQueue();
    int QueueEmpty();
    int QueueFull();
    void EnQueue(Data data);
    void DeQueue();
    Data QueueFront();
    void TransQueue();

    private:
        int _front;
        int _rear;
        int _count;
        Data _data[QueueSize];
};

CirQueue::CirQueue()
{}

CirQueue::~CirQueue()
{}

void CirQueue::InfoQueue()
{
     printf("Q front:%d, rear:%d, count:%d\n", _front, _rear, _count);
}


void CirQueue::InitQueue()
{
    _front = _rear = 0;
    _count = 0;
}

int CirQueue::QueueEmpty()
{
    return _count==0;
}

int CirQueue::QueueFull()
{
    return _count==QueueSize;
}

void CirQueue::EnQueue(Data data)
{
     if (QueueFull())
     {
         printf("Queue is over flow\n");
         return;
     }

     _count++;
     _data[_rear]=data;
     _rear = (_rear+1)%QueueSize;
     InfoQueue();
}

void CirQueue::DeQueue()
{
     if (QueueEmpty())
     {
         printf("Queue is underflow\n");
         return;
     }
     Data t;
     t = _data[_front];
     _count--;
     _front = (_front+1)%QueueSize;
     InfoQueue();
}

Data CirQueue::QueueFront()
{
     if (QueueEmpty())
     {
         printf("Queue is empty\n");
         return 'e';
     }

     return _data[_front];
}

void CirQueue::TransQueue()
{
     int t = _front;
     while(t != _rear)
     {
         printf("the data is:%c\n", _data[t]);
         t++;
     }
}

int main(int argc, const char* argv[])
{
    printf("argc:%d, argv[0]:%s\n", argc, argv[0]);
     CirQueue CQ;
     CQ.InitQueue();
     printf("QueueEmpty is:%d\n", CQ.QueueEmpty());
     printf("QueueFull  is:%d\n", CQ.QueueFull());
     Data d1 = '1';
     Data d2 = '2';
     Data d3 = '3';
     CQ.EnQueue(d1);
     CQ.EnQueue(d2);
     CQ.EnQueue(d3);
     CQ.TransQueue();
     printf("QueueFront :%c\n", CQ.QueueFront());
     CQ.DeQueue();
     CQ.TransQueue();
     printf("QueueFront :%c\n", CQ.QueueFront());
     return 0;
}
