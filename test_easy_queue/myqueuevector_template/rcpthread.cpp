#include <iostream>
#include <string>
extern "C"{
#include <assert.h>
}
#include "rcpthread.h"

RCPthread::RCPthread(void* data)
: p_data(data)
, p_st_thread(nullptr)
{

}

RCPthread::~RCPthread()
{

}

void RCPthread::Init()
{
    std::cout << "Start RCPthread..." << std::endl;
    p_st_thread = new STPthread();
    p_st_thread->_self = this;
    p_st_thread->_data = p_data;
    if(pthread_create(&m_pthread, 0, (void*(*)(void*))RCPthread::_Start, (void*)p_st_thread) != 0)
    {
        std::cout << "pthread_create failed" << std::endl;
        assert(false);
    }
}

void RCPthread::Unin()
{
    std::cout << "Join RCPthread..." << std::endl;
    pthread_join(m_pthread, NULL);
    std::cout << "Stop RCPthread..." << std::endl;
    if (p_st_thread)
    {
         delete p_st_thread;
         p_st_thread = nullptr;
    }
    std::cout << "Del STPthread..." << std::endl;
}

void RCPthread::_Start(void* p)
{
    std::cout << "Run RCPthread...:" << std::endl;
    STPthread* pst = (STPthread*)p;
    ((RCPthread*)(pst->_self))->Run(pst->_data);
}
