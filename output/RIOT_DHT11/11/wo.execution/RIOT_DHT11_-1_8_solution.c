#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "sys/socket.h"
#include "ztimer.h"
#include "ztimer_sleep.h"
#include "xtimer.h"
#include "thread_arch.h"
#include "mutex.h"

#define SLEEP_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

char stack[THREAD_STACKSIZE_MAIN];

// Mock function for dht_read since we cannot use `dht.h` or `dht_read`
int mock_dht_read(int *temperature, int *humidity) {
    // Simulate DHT11 reading
    *temperature = 25; // Example temperature data
    *humidity = 60;    // Example humidity data
    return 0;
}

void *dht_read_thread(void *arg) {
    (void)arg;
    int temperature, humidity;
    while (1) {
        if (mock_dht_read(&temperature, &humidity) == 0) {
            printf("Temperature: %d C, Humidity: %d%%\n", temperature, humidity);
        } else {
            printf("Error reading from DHT11 sensor\n");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    printf("Starting DHT11 sensor reader on ESP32\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_read_thread, NULL, "dht_thread");
    return 0;
}