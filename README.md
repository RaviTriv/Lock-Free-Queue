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

If `size=21` and we have two threads, `x` and `y` running. `x` can read `21` and increment it to `22`, but before it's written to memory `y` will have read and recieved the value `21` it will also increment to `22`. 

![Concurrency Issue](/images/problem.png)

## Synchronization
We need make sure our value is synchronized to prevent this. 

### Mutex lock 
A simple way to implement synchronization is via a `mutex lock`. We simply wrap the `critical section` with a `lock` statement prior to execution and an `unlock` after it completes execution.
```c++
mutex m;
void enqueue(int val){
  m.lock()
  //Operation to add new element to our queue
  m.unlock()
}
```
If we fail to aquire a lock our thread will become blocked, and switch over to another task. We we will later check back to see if the `mutex` is unlocked. 

Applying these changes we getting the following output when we run the program three times.

```
QUEUE SIZE: 1000000
QUEUE SIZE: 1000000
QUEUE SIZE: 1000000
```

The downside of this is the `thread switching` can increase latency in our program.

### Atomic operations
Another approach for thread safety is using `atomics`. Essentially these are special hardware instructions that execute without interruption. This is the foundation of lock free data structures.

`Compare and Swap` is an atomic operation that can be used to check if our value is correct based on when we fetched it. If it is only then we update the value. Otherwise we will fetch and repeat until the value is what is expected.

```c++
Node* newNode = Node(value, nullptr);
Node* old = tail.load();
while(!tail.compare_exchange_weak(old, newNode)){
  old = tail.load();
}
```
Running the program 3 times with these changes we get the following output.
```
QUEUE SIZE: 1000000
QUEUE SIZE: 1000000
QUEUE SIZE: 1000000
```

## Performance differences

Very rough benchmarks.

### Mutex Lock Queue
```
TIME TAKEN: 5143809
TIME TAKEN: 5214764
TIME TAKEN: 5248978
```

### Lock Free Queue

```
TIME TAKEN: 5096014
TIME TAKEN: 5045408
TIME TAKEN: 5091301
```

## How to run
`g++ main.cpp -o temp --std=c++11`
