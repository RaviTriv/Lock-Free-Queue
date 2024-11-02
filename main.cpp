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

int main()
{
  Node *f = new Node(21, nullptr);
  Node *s = new Node(22, nullptr);
  f->next = s;
  printf("NEXT NODE VALUE: %d\n", f->next->data);
}