#include "xtimer.h"
#include "thread.h"
#include "dht.h"
#include "msg.h"

#define DHT_GPIO    2
#define SLEEP_INTERVAL (5U * US_PER_SEC)

static dht_t dev;
static dht_params_t params = {
    .pin = DHT_GPIO,
};

void *read_dht(void *arg) {
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read DHT sensor\n");
        } else {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }
    
    thread_t thread;
    thread_create(
        thread.stack, sizeof(thread.stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, read_dht, NULL, "dht_reader"
    );
    
    msg_t msg;
    msg_receive(&msg);
    return 0;
}