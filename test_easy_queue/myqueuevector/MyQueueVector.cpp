#include "MyQueueVector.h"

CirQueue::CirQueue()
{}

CirQueue::~CirQueue()
{}

void CirQueue::InitQueue(int maxSize)
{
    CirLock lock(&_mutexQueue);
    _max = maxSize;
    _front = _rear = 0;
    _data.clear();
    _count = 0;
    for(int i=0;i<_max;++i)
    {
        Data data = 'a';
        EnQueue(data);
    }
    _borrow = 0;
    _return = 0;
    _used = 0;
}

void CirQueue::UninQueue()
{
    CirLock lock(&_mutexQueue);
    while(!QueueEmpty())
        DeQueue();
    _front = _rear = 0;
    _data.clear();

}

int CirQueue::QueueDoubleSize()
{
    printf("DoubleSize _data.capacity:%lu, _data.size:%lu, _data.max_size:%lu\n", _data.capacity(), _data.size(), _data.max_size());
    {
        CirLock lock(&_mutexQueue);
        _data.resize(_max*2);
        printf("DoubleSize doubleen _data.capacity:%lu, _data.size:%lu, _data.max_size:%lu\n", _data.capacity(), _data.size(), _data.max_size());
        Data data = 'b';
        DoubleEn(data);
    }
    printf("DoubleSize resize _data.capacity:%lu, _data.size:%lu, _data.max_size:%lu\n", _data.capacity(), _data.size(), _data.max_size());
    return 0;
}

// memcopy data to the back one
void CirQueue::Borrow(Data data)
{
    if (!CanBorrow())
    {
        InfoUsage();
        printf("all the data used, no on to borrow\n");
        return;
    }
    {
        CirLock lock(&_mutexUsage);
        _used++;
        memcpy(&_data.at(_borrow), &data, sizeof(Data));
        _borrow = (_borrow+1)%_count;
    }
    InfoUsage();
}

// memset the front one
void CirQueue::Return()
{
    if (!CanReturn())
    {
         InfoUsage();
         printf("no data is used, no need to return\n");
         return;
    }
    {
        CirLock lock(&_mutexUsage);
        memset(&(_data.at(_return)), '\0', sizeof(Data));
        _used--;
        _return = (_return+1)%_count;
    }
    InfoUsage();
}

bool CirQueue::CanBorrow()
{
    return _used!=_count;
}

bool CirQueue::CanReturn()
{
    return _used!=0;
}

void CirQueue::InfoUsage()
{
    printf("Usage _borrow:%d, _return:%d, _used:%d\n", _borrow, _return, _used);
}

int CirQueue::QueueEmpty()
{
    return _count==0;
}

int CirQueue::QueueFull()
{
    return _count==_max;
}

Data CirQueue::QueueFront()
{
     if (QueueEmpty())
     {
         printf("Queue is empty\n");
         return 'e';
     }

     return _data.front();
}

void CirQueue::TransQueue()
{
    if (QueueFull())
    {
        int t = _front;
        while(t != _max)
        {
            printf("the data is:%c\n", _data[t]);
            t++;
        }
    } else {
        int t = _front;
        while(t != _rear)
        {
            printf("the data is:%c\n", _data[t]);
            t++;
        }
    }
}

void CirQueue::InfoQueue()
{
     printf("Q front:%d, rear:%d, count:%d\n", _front, _rear, _count);
}
