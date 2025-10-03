#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Buffer configuration
const int BUFFER_SIZE = 10;
const int MAX_ITEMS = 20;

// Shared buffer and counters
int buffer[BUFFER_SIZE];
int count = 0;           // Current items in buffer
int in = 0;              // Producer index
int out = 0;             // Consumer index
int produced_total = 0;  // Total items produced
int consumed_total = 0;  // Total items consumed

// Synchronization primitives
pthread_mutex_t mutex;
pthread_cond_t not_full;
pthread_cond_t not_empty;

// Thread control
bool production_done = false;

// Structure to pass thread arguments
struct ThreadArgs {
    int id;
    int items_to_process;
};

// Function to display buffer state (for debugging/visualization)
void displayBuffer() {
    cout << "Buffer [";
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i < count) {
            cout << buffer[(out + i) % BUFFER_SIZE];
        } else {
            cout << "_";
        }
        if (i < BUFFER_SIZE - 1) cout << " ";
    }
    cout << "] Count: " << count << "/" << BUFFER_SIZE << endl;
}

// Producer thread function
void* producer(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int producer_id = args->id;
    int items = args->items_to_process;

    for (int i = 0; i < items; i++) {
        // Produce an item (random number between 1-100)
        int item = rand() % 100 + 1;
        
        // Simulate production time
        usleep((rand() % 500 + 100) * 1000); // 100-600ms

        // Lock the mutex before accessing shared resources
        pthread_mutex_lock(&mutex);

        // Wait while buffer is full
        while (count == BUFFER_SIZE) {
            cout << "Producer " << producer_id << " waiting (buffer full)..." << endl;
            pthread_cond_wait(&not_full, &mutex);
        }

        // Add item to buffer
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        produced_total++;

        cout << ">>> Producer " << producer_id << " produced item: " << item 
             << " (Total produced: " << produced_total << ")" << endl;
        displayBuffer();

        // Signal that buffer is not empty
        pthread_cond_signal(&not_empty);

        // Unlock the mutex
        pthread_mutex_unlock(&mutex);
    }

    cout << "*** Producer " << producer_id << " finished production ***" << endl;
    delete args;
    return nullptr;
}

// Consumer thread function
void* consumer(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int consumer_id = args->id;
    int items = args->items_to_process;

    for (int i = 0; i < items; i++) {
        // Lock the mutex before accessing shared resources
        pthread_mutex_lock(&mutex);

        // Wait while buffer is empty
        while (count == 0) {
            // Check if production is done and buffer is empty
            if (production_done && count == 0) {
                pthread_mutex_unlock(&mutex);
                cout << "*** Consumer " << consumer_id << " detected production done and buffer empty ***" << endl;
                delete args;
                return nullptr;
            }
            cout << "Consumer " << consumer_id << " waiting (buffer empty)..." << endl;
            pthread_cond_wait(&not_empty, &mutex);
        }

        // Remove item from buffer
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        consumed_total++;

        cout << "<<< Consumer " << consumer_id << " consumed item: " << item 
             << " (Total consumed: " << consumed_total << ")" << endl;
        displayBuffer();

        // Signal that buffer is not full
        pthread_cond_signal(&not_full);

        // Unlock the mutex
        pthread_mutex_unlock(&mutex);

        // Simulate consumption time
        usleep((rand() % 500 + 100) * 1000); // 100-600ms
    }

    cout << "*** Consumer " << consumer_id << " finished consumption ***" << endl;
    delete args;
    return nullptr;
}

int main() {
    // Seed random number generator
    srand(time(nullptr));

    cout << "========================================" << endl;
    cout << "  PRODUCER-CONSUMER SIMULATION         " << endl;
    cout << "========================================" << endl;
    cout << "Buffer Size: " << BUFFER_SIZE << endl;
    cout << "Max Items: " << MAX_ITEMS << endl;
    cout << "========================================\n" << endl;

    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&not_full, nullptr);
    pthread_cond_init(&not_empty, nullptr);

    // Create thread arrays
    const int NUM_PRODUCERS = 2;
    const int NUM_CONSUMERS = 2;
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    // Calculate items per thread
    int items_per_producer = MAX_ITEMS / NUM_PRODUCERS;
    int items_per_consumer = MAX_ITEMS / NUM_CONSUMERS;

    // Create producer threads
    cout << "Creating " << NUM_PRODUCERS << " producer threads..." << endl;
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        ThreadArgs* args = new ThreadArgs;
        args->id = i + 1;
        args->items_to_process = items_per_producer;
        
        if (pthread_create(&producers[i], nullptr, producer, args) != 0) {
            cerr << "Error creating producer thread " << i + 1 << endl;
            return 1;
        }
        cout << "Producer " << (i + 1) << " created (will produce " 
             << items_per_producer << " items)" << endl;
    }

    // Create consumer threads
    cout << "Creating " << NUM_CONSUMERS << " consumer threads..." << endl;
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        ThreadArgs* args = new ThreadArgs;
        args->id = i + 1;
        args->items_to_process = items_per_consumer;
        
        if (pthread_create(&consumers[i], nullptr, consumer, args) != 0) {
            cerr << "Error creating consumer thread " << i + 1 << endl;
            return 1;
        }
        cout << "Consumer " << (i + 1) << " created (will consume " 
             << items_per_consumer << " items)" << endl;
    }

    cout << "\n--- Starting Production and Consumption ---\n" << endl;

    // Wait for all producer threads to finish
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], nullptr);
    }

    // Mark production as done
    pthread_mutex_lock(&mutex);
    production_done = true;
    pthread_cond_broadcast(&not_empty); // Wake up all waiting consumers
    pthread_mutex_unlock(&mutex);

    cout << "\n*** All producers finished ***\n" << endl;

    // Wait for all consumer threads to finish
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], nullptr);
    }

    cout << "\n*** All consumers finished ***\n" << endl;

    // Display final statistics
    cout << "\n========================================" << endl;
    cout << "         SIMULATION COMPLETE            " << endl;
    cout << "========================================" << endl;
    cout << "Total items produced: " << produced_total << endl;
    cout << "Total items consumed: " << consumed_total << endl;
    cout << "Final buffer count: " << count << endl;
    cout << "========================================" << endl;

    // Cleanup
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}