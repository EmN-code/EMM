#include "thread.h"
#include "xtimer.h"
#include "stdio.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 2)
#define SLEEP_DURATION (5U * US_PER_SEC) // 5 seconds

void *dht11_thread(void *arg) {
    (void)arg;
    while (1) {
        // Placeholder for DHT11 reading logic
        printf("Reading DHT11 sensor...\n");
        
        // Sleep for specified duration
        xtimer_sleep(SLEEP_DURATION);
    }
    return NULL;
}

int main(void) {
    char thread_stack[THREAD_STACKSIZE_MAIN];

    // Create a thread for reading DHT11 sensor
    thread_create(thread_stack, sizeof(thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  dht11_thread, NULL, "dht11_reader");

    // Main thread can continue doing other tasks
    while(1) {
        // Placeholder for main thread logic
        printf("Main thread is running...\n");
        
        // Yield to other threads
        thread_yield();
        
        // Sleep for a short while
        xtimer_usleep(1000000); // 1 second
    }
    return 0;
}