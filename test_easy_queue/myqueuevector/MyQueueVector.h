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

typedef char Data;

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

class CirQueue{
    public:
        CirQueue();
        virtual ~CirQueue();

        void InitQueue(int maxSize);
        void UninQueue();

        // memcopy data to the back one
        void Borrow(Data data);
        // memset the front one
        void Return();
        bool CanBorrow();
        bool CanReturn();
        void InfoUsage();

        int QueueEmpty();
        int QueueFull();
        int QueueDoubleSize();

        Data QueueFront();
        void TransQueue();
        void InfoQueue();

    private:
        void EnQueue(Data data)
        {
            if (QueueFull())
            {
                printf("Queue is over flow\n");
                return;
            }

            {
                _count++;
                _data.push_back(data);
                _rear = (_rear+1)%_max;
            }
            InfoQueue();
        }

        void DoubleEn(Data data)
        {
            for(int i=_max;i<_max*2;++i)
            {
                printf("DoubleEn assign index:%d data:%c\n", _count, data);
                _data.at(_count++) = data;
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
        std::vector<Data>   _data;
        CirMutex            _mutexQueue;
        CirMutex            _mutexUsage;
};

#endif // _MY_QUEUE_VECTOR_H_
