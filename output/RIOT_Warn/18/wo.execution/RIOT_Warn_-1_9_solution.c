#include <ztimer.h>  // For timer functionality
#include <xtimer.h> // For timer functionality
#include <stdlib.h>  // Standard library for general functions
#include <stddef.h>  // Definition of standard variable types
#include <stdbool.h> // For boolean types
#include <stdio.h>   // Standard I/O operations
#include <thread.h>  // Thread management
#include <dht.h>     // DHT sensor specific functionality
#include <msg.h>     // For messaging related functions
#include <periph/gpio.h> // To control GPIO

#define DHT_PIN GPIO_PIN(PORT_A, 10)  // Define the GPIO pin for DHT11
#define LED_PIN GPIO_PIN(PORT_A, 5)   // Define the GPIO pin for LED
#define TEMP_THRESHOLD 30             // Temperature threshold for LED trigger
#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)

static kernel_pid_t dht_thread_pid;
static char thread_stack[THREAD_STACKSIZE];

void *read_dht_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);
    
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("DHT init failed");
        return NULL;
    }
    
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C\n", temp / 10, temp % 10);
            printf("Humidity: %d.%d%%\n", hum / 10, hum % 10);
            
            if (temp > TEMP_THRESHOLD * 10) {  // Compare with threshold
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read from DHT sensor");
        }
        
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_dht_thread, NULL, "read_dht_thread");
    return 0;
}