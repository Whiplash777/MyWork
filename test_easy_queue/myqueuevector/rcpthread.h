#ifndef _RC_PTHREAD_H_
#define _RC_PTHREAD_H_

extern "C"{
#include <pthread.h>
}

typedef struct STPthread{
    void* _self;
    void* _data;
}STPthread;

class RCPthread{
    public:
        RCPthread(void*);
        virtual ~RCPthread();
        void Init();
        void Unin();
        virtual void Run(void*) = 0;
    private:
        static void _Start(void*);
    private:
        pthread_t   m_pthread;
        void*       p_data;
        STPthread*  p_st_thread;
};

#endif // _RC_PHTREAD_H_
