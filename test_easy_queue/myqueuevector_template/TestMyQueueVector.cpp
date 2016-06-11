#include "MyQueueVector.h"
#include "rcpthread.h"

#if 1
void FunctionTest1()
{
    CirQueue<char> CQ;
     CQ.InitQueue(QueueSize);
     printf("QueueEmpty is:%d\n", CQ.QueueEmpty());
     printf("QueueFull  is:%d\n", CQ.QueueFull());
     printf("QueueFront :%c\n", CQ.QueueFront());
     CQ.TransQueue();
     printf("CanBorrow :%d\n", CQ.CanBorrow());
     printf("CanReturn :%d\n", CQ.CanReturn());

     char d1 = '1';
     char d2 = '2';
     char d3 = '3';
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

     char d4 = '4';
     char d5 = '5';
     char d6 = '6';
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
#endif

class Worker : public RCPthread{
    public:
    Worker(void* data):RCPthread(data) {}
    virtual ~Worker(){}

    virtual void Run(void* data);
};


template <class T>
class TestThread{
    public:
    TestThread(int type, CirQueue<T>* queue)
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


    void BorrowQueue(T t) { m_pQueue->Borrow(t); }
    void ReturnQueue() { m_pQueue->Return(); }

    void TestBorrow()
    {
         while(1)
         {
             T t;
             BorrowQueue(t);
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
    int                 m_tType;
    CirQueue<T>*     m_pQueue;
    Worker*             m_pWorker;
    long long           m_BorrowCounter;
    long long           m_ReturnCounter;
};

void Worker::Run(void* data)
{
    TestThread<std::string>* p_thread = (TestThread<std::string>*)data;
    if (p_thread->GetTestType()==1)
    {
        p_thread->TestBorrow();
    } else {
        p_thread->TestReturn();
    }
}

void MutilPthreadTest()
{
    //CirQueue<char> queue;
    CirQueue<std::string> queue;
    queue.InitQueue(20000);
    queue.QueueDoubleSize();
    queue.QueueDoubleSize();
    queue.QueueDoubleSize();
    queue.QueueDoubleSize();
    TestThread<std::string> t1(1, &queue);
    usleep(4);
    TestThread<std::string> t2(2, &queue);
    while(1);// usleep(1);
}

int main(int argc, const char* argv[])
{
    printf("argc:%d, argv[0]:%s\n", argc, argv[0]);
    //FunctionTest1();
    MutilPthreadTest();
    return 0;
}
