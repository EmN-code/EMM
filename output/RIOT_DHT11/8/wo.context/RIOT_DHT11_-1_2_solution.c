#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "periph/gpio.h"

#define LED_PIN     GPIO_PIN(PORT_B,0)
#define STACKSIZE   (THREAD_STACKSIZE_MAIN)
#define DHT_GPIO    GPIO_PIN(PORT_A,2)  // Assuming GPIO 2 is PORT_A_PIN_2

char stack[STACKSIZE];

void *thread_handler(void *arg) {
    (void)arg;
    while(1) {
        // Simulating DHT11 read with a placeholder
        printf("Reading DHT11 sensor values...\n");

        // Sleep for 5 seconds
        thread_sleep(5);
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_set(LED_PIN);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread_handler");

    // Yield to thread
    thread_yield();

    return 0;
}