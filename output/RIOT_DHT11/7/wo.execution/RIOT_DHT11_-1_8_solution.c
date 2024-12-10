#include "xtimer.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"

#define DHT11_PIN GPIO_PIN(0, 2) // Pin configuration for GPIO2

// Placeholder for DHT11 data reading function
void read_dht11(int16_t *temperature, int16_t *humidity) {
    printf("Reading sensor data...\n");
    // Simulate reading
    *temperature = 24; // Example static value
    *humidity = 60;    // Example static value
}

void *dht11_thread(void *arg) {
    (void)arg;

    int16_t temperature = 0;
    int16_t humidity = 0;
  
    while (1) {
        read_dht11(&temperature, &humidity);
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    kernel_pid_t tid;
    char stack[THREAD_STACKSIZE_MAIN];

    // Create a thread to handle DHT11 readings
    tid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                        0, dht11_thread, NULL, "dht11_reader");

    (void)tid;

    while (1) {
        // Main thread can perform other tasks
    }

    return 0;
}
