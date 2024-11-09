# Lock Free Queue

A simple thread safe non-blocking queue.

## Why is this necessary
When multiple threads are writing and reading to a data structure thread safety is important to ensure our program works as expected.

A simple example can illustrate this.
```c++
void qHelper(Queue *q)
{
  q->enqueue(21);
}

for(int i = 0; i < 1000000; i++){
  thread t1(qHelper);
  t1.detach();
}
printf("QUEUE SIZE: %d\n", q->size);
```

Running the program three times the following is outputted.

```
QUEUE SIZE: 999876
QUEUE SIZE: 999913
QUEUE SIZE: 999897
```

The expectation is that our program ouputs `QUEUE SIZE: 1000000`, but thats not happening.

Our `enqueue` operation looks like the following
```c++
void enqueue(int val){
  //Add item to queue
  size++;
}
```

If `size=21` and we have two threads, `x` and `y` running. `x` can read `21` and increment it to `22`, but before it's written to memory `y` will have read and recieved the value `21` it will also increment to `21`. This is called a `data race`.

We need make sure our value is synchronized to prevent this. 

# Mutex lock 
A simple way to implement synchronization is via a `mutex lock`. We simply wrap the `critical section` with a `lock` statement prior to execution and an `unlock` after it completes execution.
```c++
mutex m;
void enqueue(int val){
  m.lock()
  //Operation to add new element to our queue
  m.unlock()
}
```
If our `mutex` is locked our thread will become blocked, switch over to another task and check back later to see if the `mutex` is unlocked. The `thread` switching can increase latency in our program.
## Blocking vs Non-Blocking Queue
Typically a lock is used for synchronization in data structures, this can be a bottleneck for parallel programs.

Synchorinzation is necessary for thread safe queues, a simple example of a non thread safe queue can illustrate this.

```c++
void qHelper(Queue *q)
{
  q->enqueue(21);
}

for(int i = 0; i < 1000000; i++){
  thread t1(qHelper);
  t1.detach();
}
printf("QUEUE SIZE: %d\n", q->size);
```

Running the program three times the following is outputted.

```
QUEUE SIZE: 999876
QUEUE SIZE: 999913
QUEUE SIZE: 999897
```

The expectation is that our program ouputs 1000000, but thats not happening.

Whats happening is our threads are sometimes writing at the same time overriding one another. For example `thread x` creates a `new node a` and `thread y` creates a `new node b`. Before `thread x` can complete its operation of pointing the `tail's next value` to `node a` and setting `node a` as the new tail, `thread y` comes in and sets the current `tail's next value` to `node b`. The correct behaviour is waiting for `node a` to become the tail value before setting the `tail's next value to node b`.

A simple change to our queue can ensure thread safety in our `enqueue` operation.

```c++
mutex m;
void enqueue(int val){
  m.lock()
  //Operation to add new element to our queue
  m.unlock()
}
```
This pretty much allows us to ensure only 1 thread can execute the critical section at a time. The updated code outputs the following.
```
QUEUE SIZE: 1000000
QUEUE SIZE: 1000000
QUEUE SIZE: 1000000
```


When we want to write an element we use a lock preventing other threads from gaining access, this blocks threads and causes thread switching which results in increased latency.
Some very rough numbers of a simple lock based queue look something like the following.
```
TIME TAKEN: 5143809
TIME TAKEN: 5214764
TIME TAKEN: 5248978
```

The foundation of lock free data structures are `atomic` operations. Essentially these are special hardware instructions that execute without interruption. `Compare and Swap` can be used to check if our value is correct based on when we fetched it, if it is only then we update the value. Otherwise we will fetch and repeat until the value is what is expected.

```c++
Node* newNode = Node(value, nullptr);
Node* old = tail.load();
while(!tail.compare_exchange_weak(old, newNode)){
  old = tail.load();
}
```

LOCK FREE QUEUE
```
TIME TAKEN: 5096014
TIME TAKEN: 5045408
TIME TAKEN: 5091301
```

`g++ main.cpp -o temp --std=c++11`
