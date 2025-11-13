#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <unistd.h>  // for sleep()
using namespace std;

mutex mtx;
condition_variable cv;
int capacity = 2;
queue<int> buffer;

void producer() {
    int item = 0;
    for(int i = 0 ; i < 5 ; i++) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return buffer.size() < capacity; });

        buffer.push(item);
        cout << "Produced: " << item << endl;
        item++;

        cv.notify_all();
        lock.unlock();
        sleep(1);
    }
}

void consumer() {
    int item = 0;
    for(int i = 0 ; i < 5 ; i++) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, []{ return !buffer.empty(); });

        int val = buffer.front();
        buffer.pop();
        cout << "Consumed: " << val << endl;
        item++;

        cv.notify_all();
        lock.unlock();
        sleep(1);
    }
}

int main() {
    thread prod(producer);
    thread cons(consumer);

    prod.join();
    cons.join();

    cout << "Finished using Mutexes!" << endl;
    return 0;
}