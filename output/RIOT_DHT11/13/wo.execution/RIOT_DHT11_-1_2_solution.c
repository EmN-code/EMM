#include "dht.h"
#include "thread.h"
#include "xtimer.h"

#define DHT_PIN 2
#define DHT_TYPE DHT11
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

char dht_thread_stack[THREAD_STACKSIZE];

void *dht_read_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&temperature, &humidity, DHT_TYPE, DHT_PIN) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    thread_create(dht_thread_stack, THREAD_STACKSIZE, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, dht_read_thread, NULL, "dht_reader");
    return 0;
}