#include "thread_config.h"
#include "timex.h"
#include "thread.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_DURATION (5U * US_PER_SEC)

// Stack size for the thread
char stack[THREAD_STACKSIZE_MAIN];

void *dht_read_thread(void *arg){
    dht_params_t params;
    dht_t dev;
    int16_t temp, hum;

    params.pin = DHT_PIN;
    params.type = DHT11;
    dht_init(&dev, &params);

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from sensor\n");
        }
        xtimer_sleep(SLEEP_DURATION);
    }

    return NULL;
}

int main(void){
    puts("Starting DHT11 reader thread...");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_read_thread, NULL, "dht_reader");
    return 0;
}