#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include <semaphore.h>
#include <unistd.h>  // for sleep()
using namespace std;

sem_t empty;
sem_t full;
mutex mtx;
int capacity = 2;
queue<int> buffer;

void producer() {
    int item = 0;
    for(int i = 0 ; i < 5 ; i++) {
        sem_wait(&empty);
        mtx.lock();

        buffer.push(item);
        cout << "Produced: " << item << endl;
        item++;

        mtx.unlock();
        sem_post(&full);
        sleep(1);
    }
}

void consumer() {
    int items = 0 ;
    for(int i = 0 ; i < 5 ; i ++) {
        sem_wait(&full);
        mtx.lock();
        
        int val = buffer.front();
        buffer.pop();
        cout << "Consumed: " << val << endl;
        items++;

        mtx.unlock();
        sem_post(&empty);
        sleep(1);
    }
}
int main() {
    sem_init(&empty, 0, capacity);
    sem_init(&full, 0, 0);

    thread prod(producer);
    thread cons(consumer);

    prod.join();
    cons.join();

    sem_destroy(&empty);
    sem_destroy(&full);

    cout << "Finished using Semaphores!" << endl;
    return 0;
}