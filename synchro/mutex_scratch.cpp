#include <iostream>
#include <thread>
#include <atomic>
#include <queue>
using namespace std;

class Mutex {
    atomic<bool> locked{false};
public:
    void lock() { while(locked.exchange(true)); }
    void unlock() { locked = false; }
};

Mutex mtx;
queue<int> buffer;
const int CAP = 2;

void producer() {
    for(int i = 0; i < 5; i++) {
        while(true) {
            mtx.lock();
            if(buffer.size() < CAP) {
                buffer.push(i);
                cout << "Produced: " << i << endl;
                mtx.unlock();
                break;
            }
            mtx.unlock();
        }
    }
}

void consumer() {
    for(int i = 0; i < 5; i++) {
        while(true) {
            mtx.lock();
            if(!buffer.empty()) {
                cout << "Consumed: " << buffer.front() << endl;
                buffer.pop();
                mtx.unlock();
                break;
            }
            mtx.unlock();
        }
    }
}

int main() {
    thread p(producer), c(consumer);
    p.join(); c.join();
    cout << "Done!" << endl;
}
