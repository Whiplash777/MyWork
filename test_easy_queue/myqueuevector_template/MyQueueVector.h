#ifndef _MY_QUEUE_VECTOR_H_
#define _MY_QUEUE_VECTOR_H_

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>

#define QueueSize (10)

class CirMutex{
    public:
    CirMutex()
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_PRIVATE);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);

        pthread_mutex_init(&__mutex, &attr);
        pthread_mutexattr_destroy(&attr);
    }
    ~CirMutex()
    {
        pthread_mutex_destroy(&__mutex);
    }

    void Lock()
    {
         pthread_mutex_lock(&__mutex);
    }

    void Unlock()
    {
        pthread_mutex_unlock(&__mutex);
    }
    private:
    pthread_mutex_t     __mutex;
};

class CirLock{
    public:
    CirLock(CirMutex *mutex):_mutex(mutex){ _mutex->Lock(); }
    ~CirLock(){ _mutex->Unlock(); }

    private:
    CirMutex    *_mutex;
};

template <class D>
class CirQueue{
    public:
        CirQueue();
        virtual ~CirQueue();

        void InitQueue(int maxSize);
        void UninQueue();

        // memcopy data to the back one
        void Borrow(D d);
        // memset the front one
        void Return();
        bool CanBorrow();
        bool CanReturn();
        void InfoUsage();

        int QueueEmpty();
        int QueueFull();
        int QueueDoubleSize();

        D    QueueFront();
        void TransQueue();
        void InfoQueue();

    private:
        void EnQueue(D d)
        {
            if (QueueFull())
            {
                printf("Queue is over flow\n");
                return;
            }

            {
                _count++;
                _data.push_back(d);
                _rear = (_rear+1)%_max;
            }
            InfoQueue();
        }

        void DoubleEn(D d)
        {
            for(int i=_max;i<_max*2;++i)
            {
                printf("DoubleEn assign index:%d\n", _count);
                _data.at(_count++) = d;
            }
            _max *= 2;
        }

        void DeQueue()
        {
            if (QueueEmpty())
            {
                printf("Queue is underflow\n");
                return;
            }
            {
                 _data.pop_back();
                 _count--;
                 _front = (_front+1)%_max;
            }
            InfoQueue();
        }

    private:
        int _max;
        int _front;
        int _rear;
        int _count;
        int _borrow;
        int _return;
        int _used;
        std::vector<D>      _data;
        CirMutex            _mutexQueue;
        CirMutex            _mutexUsage;
};

template <class D>
CirQueue<D>::CirQueue()
{}

template <class D>
CirQueue<D>::~CirQueue()
{}

template <class D>
void CirQueue<D>::InitQueue(int maxSize)
{
    CirLock lock(&_mutexQueue);
    _max = maxSize;
    _front = _rear = 0;
    _data.clear();
    _count = 0;
    for(int i=0;i<_max;++i)
    {
        D d;
        EnQueue(d);
    }
    _borrow = 0;
    _return = 0;
    _used = 0;
}

template <class D>
void CirQueue<D>::UninQueue()
{
    CirLock lock(&_mutexQueue);
    while(!QueueEmpty())
        DeQueue();
    _front = _rear = 0;
    _data.clear();

}

template <class D>
int CirQueue<D>::QueueDoubleSize()
{
    printf("DoubleSize _data.capacity:%lu, _data.size:%lu, _data.max_size:%lu\n", _data.capacity(), _data.size(), _data.max_size());
    {
        CirLock lock(&_mutexQueue);
        _data.resize(_max*2);
        printf("DoubleSize doubleen _data.capacity:%lu, _data.size:%lu, _data.max_size:%lu\n", _data.capacity(), _data.size(), _data.max_size());
        D d;
        DoubleEn(d);
    }
    printf("DoubleSize resize _data.capacity:%lu, _data.size:%lu, _data.max_size:%lu\n", _data.capacity(), _data.size(), _data.max_size());
    return 0;
}

// memcopy data to the back one
template <class D>
void CirQueue<D>::Borrow(D d)
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
        memcpy(&_data.at(_borrow), &d, sizeof(D));
        _borrow = (_borrow+1)%_count;
    }
    InfoUsage();
}

// memset the front one
template <class D>
void CirQueue<D>::Return()
{
    if (!CanReturn())
    {
         InfoUsage();
         printf("no data is used, no need to return\n");
         return;
    }
    {
        CirLock lock(&_mutexUsage);
        memset(&(_data.at(_return)), 0x00, sizeof(D));
        _used--;
        _return = (_return+1)%_count;
    }
    InfoUsage();
}

template <class D>
bool CirQueue<D>::CanBorrow()
{
    return _used!=_count;
}

template <class D>
bool CirQueue<D>::CanReturn()
{
    return _used!=0;
}

template <class D>
void CirQueue<D>::InfoUsage()
{
    printf("Usage _borrow:%d, _return:%d, _used:%d\n", _borrow, _return, _used);
}

template <class D>
int CirQueue<D>::QueueEmpty()
{
    return _count==0;
}

template <class D>
int CirQueue<D>::QueueFull()
{
    return _count==_max;
}

template <class D>
D   CirQueue<D>::QueueFront()
{
     if (QueueEmpty())
     {
         printf("Queue is empty\n");
         D *d = new D;
         return *d;
     }

     return _data.front();
}

template <class D>
void CirQueue<D>::TransQueue()
{
    if (QueueFull())
    {
        int t = _front;
        while(t != _max)
        {
            //printf("the data is:%c\n", _data[t]);
            t++;
        }
    } else {
        int t = _front;
        while(t != _rear)
        {
            //printf("the data is:%c\n", _data[t]);
            t++;
        }
    }
}

template <class D>
void CirQueue<D>::InfoQueue()
{
     printf("Q front:%d, rear:%d, count:%d\n", _front, _rear, _count);
}

#endif // _MY_QUEUE_VECTOR_H_
