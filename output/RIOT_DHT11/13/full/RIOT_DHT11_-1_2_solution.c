#include "thread_flags.h"
#include "thread.h"
#include "event/thread.h"

#include <stdio.h> // for printf
#include <stdlib.h> // for exit

// Definitions
#define GPIO_PIN  2
#define DHT_TYPE  DHT11
#define SLEEP_INTERVAL (5U)

// Function prototypes
void *dht_sensor_thread(void *arg);

// Global variable to simulate sensor reading
int dht_value = 0;

void *dht_sensor_thread(void *arg) {
    while (1) {
        // Simulating DHT sensor reading
        // As dht_read function is restricted, assume it's a random value generator for the demo.
        dht_value = rand() % 101;  // Random value between 0 to 100 simulating humidity percentage.
        printf("DHT Sensor Reading: %d %%\n", dht_value);
        
        // Simulate wait of 5 seconds using a busy wait loop (since xtimer_sleep is not allowed)
        unsigned sleep = SLEEP_INTERVAL;
        while (sleep--) {
            // Perform a busy wait for 1 second iterations to achieve a total sleep time equivalent to SLEEP_INTERVAL
            for (volatile unsigned i = 0; i < 1000000; ++i) {
                // Intentionally left empty.
            }
        }
    }
    return NULL;
}

int main(void) {
    thread_t pt;

    if (thread_create(&pt, NULL, dht_sensor_thread, NULL, 0, "dht_thread") != 0) {
        printf("Failed to create DHT sensor thread\n");
        exit(EXIT_FAILURE);
    }

    // Main thread can perform other tasks such as network operations
    // Using a loop with a network connection would be applied here, following the logic given above.

    while (1) {
        // Simulate main thread tasks
        // For demonstration, we continuously run, could be replaced with real operations
        volatile unsigned i;
        for (i = 0; i < 1000000; ++i) {
            // Intentionally left empty.
        }

        // Simulate network operation check using while loop logic
        int rc;
        while (1) {
            rc = rand() % 2;  // Simulated random success (0) or failure (1)
            if (rc != 0) {
                printf("Network operation failed, retrying...\n");
                // Simulate wait
                for (volatile unsigned i = 0; i < 1000000; ++i) {
                    // Intentionally left empty for a 1-second delay.
                }
                continue;
            } else {
                printf("Network operation succeeded\n");
                break;
            }
        }
    }

    return 0;
}