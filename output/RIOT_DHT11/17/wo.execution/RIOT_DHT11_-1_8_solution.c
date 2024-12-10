#include <ztimer.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <msg.h>
#include <netinet/in.h>
#include <kernel_defines.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5U * US_PER_SEC)

void read_dht(void) {
    int16_t temperature, humidity;
    if (dht_read(NULL, &temperature, &humidity) == 0) {
        printf("Temperature: %d.%d C\n", temperature / 10, abs(temperature % 10));
        printf("Humidity: %d.%d %%\n", humidity / 10, abs(humidity % 10));
    } else {
        printf("Failed to read data from DHT sensor\n");
    }
}

int main(void) {
    gpio_init(DHT_PIN, GPIO_IN);

    while (1) {
        read_dht();
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    return 0;
}