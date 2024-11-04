#include <cstdio>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
using namespace std;
using namespace std::chrono;

class Node
{
public:
  int data;
  // int data;
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
  // atomic<Node *> head;
  // atomic<Node *> tail;
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
    m.lock();
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
    m.unlock();
  }

  int dequeue()
  {
    m.lock();
    int val = head->data;
    Node *temp = head->next;
    head = temp;
    size--;
    m.unlock();
    return val;
  }
};

void doQueueStuff(Queue *q)
{
  q->enqueue(21);
}

int main()
{
  auto start = high_resolution_clock::now();
  Queue *q = new Queue(nullptr, nullptr);
  for (int i = 0; i < 1000000; i++)
  {
    thread t1(doQueueStuff, q);
    t1.detach();
  }
  // q->enqueue(21);
  // q->enqueue(42);
  // q->enqueue(63);
  // q->enqueue(84);

  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  printf("QUEUE SIZE: %d\n", q->size);
  printf("TIME TAKEN: %lld\n", duration.count());
}