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
  atomic<int> y;
  // Node *head;
  // Node *tail;
  mutex m;
  int size = 0;
  // Queue(Node *head, Node *tail)
  // {
  //   this->head.store(head);
  //   this->tail.store(tail);
  // }
  Queue() : y(0)
  {
    head.store(nullptr);
    tail.store(nullptr);
  }

  void push(int val)
  {
    // make new last node and assign it appropriate value
    Node *tmp = new Node(val, nullptr);
    Node *old = tail.load();
    // check if value of old tail is expected, replace if it is expected value
    while (!tail.compare_exchange_weak(old, tmp))
    {
      // keep fetching latest old value
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
    y.fetch_add(1);
    // size++;
  }

  int pop()
  {
    Node *old = head.load();
    while (old != nullptr && !head.compare_exchange_weak(old, old->next))
    {
      old = head.load();
    }
    size--;
    return old->data;
  }

  void enqueue(int val)
  {
    // m.lock();
    //  check if head is nullptr
    if (head.load() == nullptr)
    {
      // head = new Node(val, nullptr);
      Node *t = head.load();
      while (!head.compare_exchange_weak(t, new Node(val, nullptr)))
      {
        t = head.load();
      }
      // head.store(new Node(val, nullptr));
    }
    else if (head.load()->next == nullptr)
    {
      // check if tail is nullptr
      // tail = new Node(val, nullptr);
      Node *t = tail.load();
      while (!tail.compare_exchange_weak(t, new Node(val, nullptr)))
      {
        t = tail.load();
      }
      // FIX THIS
      //  tail.store(new Node(val, nullptr));
      head.load()->next = tail;
    }
    else
    {
      Node *temp = new Node(val, nullptr);
      Node *t = tail.load();
      while (!tail.compare_exchange_weak(t, temp))
      {
        t = tail.load();
      }
      // tail.load()->next = temp;
      // tail = temp;
    }
    size++;
    // m.unlock();
  }

  int dequeue()
  {
    m.lock();
    int val = head.load()->data;
    Node *temp = head.load()->next;
    // head = temp;
    head.store(temp);
    size--;
    m.unlock();
    return val;
  }
};

void doQueueStuff(Queue *q)
{
  // q->enqueue(21);
  q->push(21);
}

int main()
{
  auto start = high_resolution_clock::now();
  Queue *q = new Queue();
  // q->push(21);
  // q->push(22);
  // q->push(23);

  // printf("%d\n", q->pop());
  // printf("%d\n", q->pop());
  // printf("%d\n", q->pop());

  // Queue *q = new Queue(nullptr, nullptr);
  for (int i = 0; i < 1000000; i++)
  {
    thread t1(doQueueStuff, q);
    t1.detach();
  }
  //  q->enqueue(21);
  //  q->enqueue(42);
  //  q->enqueue(63);
  //  q->enqueue(84);

  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());
  // printf("%d\n", q->dequeue());

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  // printf("QUEUE SIZE: %d\n", q->size);
  printf("QUEUE SIZE: %d\n", q->y.load());

  printf("TIME TAKEN: %lld\n", duration.count());
}