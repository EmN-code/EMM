#include <ztimer.h>
#include <stdio.h>
#include <dht.h>
#include <xtimer.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static dht_t dev;
static ztimer_t timer;

static void timer_callback(void *arg) {
    (void)arg;  // unused parameter
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%dC, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        printf("Failed to read from sensor\n");
    }
    ztimer_set(ZTIMER_SEC, &timer, INTERVAL, timer_callback, NULL);
}

int main(void) {
    puts("Starting DHT sensor application...");

    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    ztimer_set(ZTIMER_SEC, &timer, INTERVAL, timer_callback, NULL);

    return 0;
}