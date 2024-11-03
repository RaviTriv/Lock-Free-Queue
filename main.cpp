#include <cstdio>
#include <thread>
#include <mutex>
#include <condition_variable>

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
  mutex m;
  int size = 0;
  Queue(Node *head, Node *tail)
  {
    this->head = head;
    this->tail = tail;
  }
  void enqueue(int val)
  {
   // m.lock();
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
    size++;
    //m.unlock();
  }

  int dequeue()
  {
    m.lock();
    int val = head->data;
    Node *temp = head->next;
    head = temp;
    m.unlock();
    size--;
    return val;
  }
};

void doQueueStuff(Queue *q)
{
  q->enqueue(21);
}

int main()
{
  Queue *q = new Queue(nullptr, nullptr);
  for (int i = 0; i < 1000000; i++)
  {
    thread t1(doQueueStuff, q);
    t1.detach();
  }
  
  printf("QUEUE SIZE: %d\n", q->size);
}