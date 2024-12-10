#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "event/timeout.h"
#include "event/periodic.h"
#include "dht.h"
#include "xtimer.h"
#include "phydat.h"
#include "msg.h"

#define DHT_PIN GPIO2
#define DHT_TYPE DHT11
#define PERIOD_MS 5000

static dht_t dht_dev;

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("DHT Temperature: %d.%d C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Error reading DHT sensor\n");
        }
        xtimer_sleep(PERIOD_MS / 1000);
    }
    return NULL;
}

int main(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE;
    dht_init(&dht_dev, &params);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  dht_thread, NULL, "dht_thread");

    msg_t msg;
    msg_receive(&msg);
    return 0;
}
