#include <cstdio>

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

int main()
{
  Queue *q = new Queue(nullptr, nullptr);
  q->enqueue(21);
  q->enqueue(42);
  q->enqueue(63);
  q->enqueue(70);
  q->enqueue(22);
  printf("%d\n", q->dequeue());
  printf("%d\n", q->dequeue());
  printf("%d\n", q->dequeue());
  printf("%d\n", q->dequeue());
  printf("%d\n", q->dequeue());
}