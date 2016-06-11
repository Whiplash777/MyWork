#include "MyQueueVector.h"
#include "rcpthread.h"

void FunctionTest1()
{
    CirQueue CQ;
     CQ.InitQueue(QueueSize);
     printf("QueueEmpty is:%d\n", CQ.QueueEmpty());
     printf("QueueFull  is:%d\n", CQ.QueueFull());
     printf("QueueFront :%c\n", CQ.QueueFront());
     CQ.TransQueue();
     printf("CanBorrow :%d\n", CQ.CanBorrow());
     printf("CanReturn :%d\n", CQ.CanReturn());

     Data d1 = '1';
     Data d2 = '2';
     Data d3 = '3';
     CQ.Borrow(d1);
     CQ.Borrow(d2);
     CQ.Borrow(d3);
     CQ.TransQueue();
     printf("after borrow CanBorrow :%d\n", CQ.CanBorrow());
     printf("after borrow CanReturn :%d\n", CQ.CanReturn());
     CQ.Return();
     CQ.TransQueue();
     printf("after return CanBorrow :%d\n", CQ.CanBorrow());
     printf("after return CanReturn :%d\n", CQ.CanReturn());
     CQ.QueueDoubleSize();
     CQ.InfoQueue();
     CQ.InfoUsage();
     CQ.TransQueue();

     Data d4 = '4';
     Data d5 = '5';
     Data d6 = '6';
     CQ.Borrow(d4);
     CQ.Borrow(d5);
     CQ.Borrow(d6);
     CQ.TransQueue();
     printf("after borrow CanBorrow :%d\n", CQ.CanBorrow());
     printf("after borrow CanReturn :%d\n", CQ.CanReturn());
     CQ.Return();
     CQ.TransQueue();
     printf("after return CanBorrow :%d\n", CQ.CanBorrow());
     printf("after return CanReturn :%d\n", CQ.CanReturn());
     int t = 100;
     while(t--)
     {
         CQ.Borrow(d4);
         CQ.Return();
         CQ.Return();
     }
     CQ.Borrow(d4);
     CQ.TransQueue();
     printf("after return CanBorrow :%d\n", CQ.CanBorrow());
     printf("after return CanReturn :%d\n", CQ.CanReturn());
}

class Worker : public RCPthread{
    public:
    Worker(void* data):RCPthread(data) {}
    virtual ~Worker(){}

    virtual void Run(void* data);
};


class TestThread{
    public:
    TestThread(int type, CirQueue* queue)
        : m_tType(type)
        , m_pQueue(queue)
        , m_pWorker(nullptr)
        {
            m_pWorker = new Worker(this);
            m_pWorker->Init();
            m_BorrowCounter = 0;
            m_ReturnCounter = 0;
        }
    virtual ~TestThread()
    {
        if (m_pWorker)
        {
            delete m_pWorker;
            m_pWorker = nullptr;;
        }
    }


    void BorrowQueue(Data d) { m_pQueue->Borrow(d); }
    void ReturnQueue() { m_pQueue->Return(); }

    void TestBorrow()
    {
         while(1)
         {
             Data d = 't';
             BorrowQueue(d);
             printf("TestBorrow counter:%lld\n", ++m_BorrowCounter);
         }
    }

    void TestReturn()
    {
        while(1)
        {
             ReturnQueue();
             printf("TestReturn counter:%lld\n", ++m_ReturnCounter);
        }
    }

    int  GetTestType() { return m_tType; }

    private:
    int             m_tType;
    CirQueue*       m_pQueue;
    Worker*         m_pWorker;
    long long m_BorrowCounter;
    long long m_ReturnCounter;
};

void Worker::Run(void* data)
{
    TestThread* p_thread = (TestThread*)data;
    if (p_thread->GetTestType()==1)
    {
        p_thread->TestBorrow();
    } else {
        p_thread->TestReturn();
    }
}

void MutilPthreadTest()
{
    CirQueue queue;
    queue.InitQueue(20000);
    queue.QueueDoubleSize();
    queue.QueueDoubleSize();
    queue.QueueDoubleSize();
    queue.QueueDoubleSize();
    TestThread t1(1, &queue);
    usleep(4);
    TestThread t2(2, &queue);
    while(1);// usleep(1);
}

int main(int argc, const char* argv[])
{
    printf("argc:%d, argv[0]:%s\n", argc, argv[0]);
    //FunctionTest1();
    MutilPthreadTest();
    return 0;
}
