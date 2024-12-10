#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on the ESP32

static kernel_pid_t main_thread_pid;
static char main_thread_stack[THREAD_STACKSIZE_MAIN];

void *main_function(void *arg) {
    (void)arg;
    dht_t dev;
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };

    dht_init(&dev, &params);

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, abs(temp % 10), hum / 10, abs(hum % 10));
        } else {
            printf("Failed to read from sensor\n");
        }

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    main_thread_pid = thread_create(main_thread_stack, sizeof(main_thread_stack),
                                    THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                    main_function, NULL, "main");
    thread_yield_higher();
    return 0;
}