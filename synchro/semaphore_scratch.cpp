#include <iostream>
#include <thread>
#include <atomic>
#include <queue>
using namespace std;

class Semaphore {
    atomic<int> count;
public:
    Semaphore(int val) : count(val) {}
    void wait() { while(count <= 0); count--; }
    void signal() { count++; }
};

class Mutex {
    atomic<bool> locked{false};
public:
    void lock() { while(locked.exchange(true)); }
    void unlock() { locked = false; }
};

Semaphore empty(2), full(0);
Mutex mtx;
queue<int> buffer;

void producer() {
    for(int i = 0; i < 5; i++) {
        empty.wait();
        mtx.lock();
        buffer.push(i);
        cout << "Produced: " << i << endl;
        mtx.unlock();
        full.signal();
    }
}

void consumer() {
    for(int i = 0; i < 5; i++) {
        full.wait();
        mtx.lock();
        cout << "Consumed: " << buffer.front() << endl;
        buffer.pop();
        mtx.unlock();
        empty.signal();
    }
}

int main() {
    thread p(producer), c(consumer);
    p.join(); c.join();
    cout << "Done!" << endl;
}
