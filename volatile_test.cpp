// Using two competing threads to increase a volatile counter x.
//
// Restarting the worker threads and signalling that they're done uses an atomic_int
// which they each use an hardcoded prime (29 for one thread, 31 for the other). When the
// sum is 60, we know that both threads are done.
//
// I noticed that contention gets way worse for global variables than members on x64.


#include <atomic>
#include <iostream>
#include <thread>


volatile int x = 0;
std::atomic_int prime_counter(0);


void thread1() {
    prime_counter += 29;
    while (prime_counter >= 0) {
        while (prime_counter == 29); // wait until other thread catches up
        while (prime_counter == 60); // wait until released
        for (int i = 0; i < 10; i++) {
            x++;
        }
        prime_counter += 29;
    }
}

void thread2() {
    prime_counter += 31;
    while (prime_counter >= 0) {
        while (prime_counter == 31); // wait until other thread catches up
        while (prime_counter == 60); // wait until released
        for (int i = 0; i < 10; i++) {
            x++;
        }
        prime_counter += 31;
    }
}


int main() {
    int results[30] = {};
    std::thread t1(&thread1);
    std::thread t2(&thread2);

    while (prime_counter != 60); // wait until both threads are ready

    for (int i = 0; i < 10000000; ++i) {
        x = 0; // reset volatile counter
        prime_counter = 0; // release both threads
        while (prime_counter != 60); // wait until both ready
        int y = x;
        if (y >= 0 && y < 30) {
            results[y]++; // register volatile counter
        }
    }
    std::cout << "result" << '\t' << "hits" << '\n';
    for (int i = 0; i < 30; ++i) {
        std::cout << i << '\t' << results[i] << '\n';
    }
    prime_counter = -100; // quits while loop for both threads
    t1.join();
    t2.join();
    return 0;
}
