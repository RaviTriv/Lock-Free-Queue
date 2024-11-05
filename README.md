### Lock Free Queue

Simple lock free queue to reduce latency.

Typically a lock is used for synchronization in data structures, this can be a bottleneck for parallel programs.

Synchorinzation is necessary for thread safe queues.
A simple example that uses a non thread safe queue can illustrate this.

```
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

The expectation is that our ouput is 1000000.

Our threads are sometimes writing at the same time overriding one another. For example `thread x` creates a `new node a` and `thread y` creates a `new node b`. Before `thread x` can complete its operation of updating the pointing the `tail's next value` to `node a` and setting `node a` as the new tail, `thread y` comes in and sets the current `tail's next value` to `node b`. We should be waiting for `node a` to become the tail value before setting the `tail's next value to node b`.

A simple change to our queue can ensure thread safety in our `enqueue` operation.

```
mutex m;
void enqueue(int val){
  m.lock()
  //Operation to add new element to our queue
  m.unlock()
}
```
This pretty much allows us to ensure only 1 thread can execute the critical section at a time.
The updated code outputs the following.
```
QUEUE SIZE: 1000000
QUEUE SIZE: 1000000
QUEUE SIZE: 1000000
```

The example above is a blocking data structure. This can result in increased latency as a result of the thread switching that occurs when a thread is blocked.

When we want to write an element we use a lock preventing other threads from gaining access. An issue that can arise with this is if our thread gets suspended and we don't release the lock, the other thread will still be stuck waiting.
TIME TAKEN: 5143809
TIME TAKEN: 5214764
TIME TAKEN: 5248978

LOCK FREE QUEUE
TIME TAKEN: 5096014
TIME TAKEN: 5045408
TIME TAKEN: 5091301