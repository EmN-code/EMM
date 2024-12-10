#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "thread.h"
#include "stdio.h"
#include "stdlib.h"

#define DHT11_PIN GPIO_PIN(0, 2) // GPIO 2

char stack[THREAD_STACKSIZE_MAIN];

void *read_dht11(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&temperature, &humidity, DHT11_PIN) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
        }
        else {
            printf("Failed to read from sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht11, NULL, "dht11_reader");
    return 0;
}