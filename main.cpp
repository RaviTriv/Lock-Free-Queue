#include <cstdio>
#include <thread>
#include <chrono>
#include <atomic>
using namespace std;
using namespace std::chrono;

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
  atomic<Node *> head;
  atomic<Node *> tail;
  atomic<int> size;
  mutex m;
  Queue() : size(0)
  {
    head.store(nullptr);
    tail.store(nullptr);
  }

  void enqueue(int val)
  {
    Node *tmp = new Node(val, nullptr);
    Node *old = tail.load();
    while (!tail.compare_exchange_weak(old, tmp))
    {
      old = tail.load();
    }
    if (old != nullptr)
    {
      old->next = tmp;
    }
    else
    {
      head.store(tmp);
    }
    size.fetch_add(1);
  }

  int dequeue()
  {
    Node *old = head.load();
    while (old != nullptr && !head.compare_exchange_weak(old, old->next))
    {
      old = head.load();
    }
    size.fetch_sub(1);
    return old->data;
  }
};

void doQueueStuff(Queue *q)
{
  q->enqueue(21);
}

int main()
{
  auto start = high_resolution_clock::now();
  Queue *q = new Queue();

  for (int i = 0; i < 1000000; i++)
  {
    thread t1(doQueueStuff, q);
    t1.detach();
  }
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  printf("QUEUE SIZE: %d\n", q->size.load());
  printf("TIME TAKEN: %lld\n", duration.count());
}