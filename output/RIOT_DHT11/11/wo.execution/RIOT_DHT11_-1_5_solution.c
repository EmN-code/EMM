#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "event/thread.h"
#include "thread.h"

#define DHT_TYPE DHT11
#define DHT_PIN GPIO_PIN(GPIO_PORT(0), 2)

msg_t msg_q[8];

static dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT_TYPE
};

static void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        int res = dht_read(dht_params.type, dht_params.pin, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("DHT read failed, error: %d\n", res);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    puts("Starting RIOT DHT11 sensor example\n");

    /* initialize message queue */
    msg_init_queue(msg_q, 8);

    /* Initialize the DHT thread */
    thread_create(
        thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");

    return 0;
}