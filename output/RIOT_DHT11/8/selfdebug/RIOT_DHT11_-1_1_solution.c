#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <dht.h>

#define DHT11_GPIO 2

static dht_t dht11;
static kernel_pid_t timer_pid;

void *dht11_reader(void *arg) {
    (void) arg;
    int16_t temp, hum;

    while (1) {
        int res = dht_read(&dht11, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        thread_sleep(5 * US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    dht_init(&dht11, &(dht_params_t){.pin = DHT11_GPIO, .type = DHT11});

    char stack[THREAD_STACKSIZE_MAIN];

    timer_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                              THREAD_CREATE_STACKTEST, dht11_reader, NULL, "dht11_reader");

    return 0;
}