### Lock Free Queue

Simple lock free queue to reduce latency.

Typically a lock is used for synchronization in data structures, this can be a bottleneck for parallel programs.

When we want to write an element we use a lock preventing other threads from gaining access. An issue that can arise with this is if our thread gets suspended and we don't release the lock, the other thread will still be stuck waiting.
