#include <cstdio>
#include <thread>
using namespace std;
class Node
{
public:
  int data;
  Node *next;
  Node(int data, Node *next)
  {
    this->data = data;
    this->next = next;
  }
};

class Queue
{
public:
  Node *head;
  Node *tail;
  Queue(Node *head, Node *tail)
  {
    this->head = head;
    this->tail = tail;
  }
  void enqueue(int val)
  {
    // check if head is nullptr
    if (head == nullptr)
    {
      head = new Node(val, nullptr);
    }
    else if (head->next == nullptr)
    {
      // check if tail is nullptr
      tail = new Node(val, nullptr);
      head->next = tail;
    }
    else
    {
      Node *temp = new Node(val, nullptr);
      tail->next = temp;
      tail = temp;
    }
  }

  int dequeue()
  {
    int val = head->data;
    Node *temp = head->next;
    head = temp;
    return val;
  }
};

void doQueueStuff(Queue *q, int threadNumber)
{
  for (int i = 0; i < 13; i++)
  {
    printf("ADDING FROM THREAD: %d\n", threadNumber);
    q->enqueue(21);
    printf("ADDING FROM THREAD: %d\n", threadNumber);
    q->enqueue(42);
    printf("ADDING FROM THREAD: %d\n", threadNumber);
    q->enqueue(63);
  }
}

int main()
{
  Queue *q = new Queue(nullptr, nullptr);

  thread t1(doQueueStuff, q, 1);
  thread t2(doQueueStuff, q, 2);
  thread t3(doQueueStuff, q, 3);
  thread t4(doQueueStuff, q, 4);
  thread t5(doQueueStuff, q, 5);
  thread t6(doQueueStuff, q, 6);
  thread t7(doQueueStuff, q, 7);
  thread t8(doQueueStuff, q, 8);
  thread t9(doQueueStuff, q, 9);
  thread t10(doQueueStuff, q, 10);
  thread t11(doQueueStuff, q, 11);
  thread t12(doQueueStuff, q, 12);
  thread t13(doQueueStuff, q, 13);
  thread t14(doQueueStuff, q, 14);
  thread t15(doQueueStuff, q, 15);
  thread t16(doQueueStuff, q, 16);
  // printf("ADDING FROM MAIN\n");
  // q->enqueue(21);
  // printf("ADDING FROM MAIN\n");
  // q->enqueue(42);
  // printf("ADDING FROM MAIN\n");
  // q->enqueue(63);
  // printf("ADDING FROM MAIN\n");
  // q->enqueue(70);
  // printf("ADDING FROM MAIN\n");
  // q->enqueue(22);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  t7.join();
  t8.join();
  t9.join();
  t10.join();
  t11.join();
  t12.join();
  t13.join();
  t14.join();
  t15.join();
  t16.join();
  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());
}