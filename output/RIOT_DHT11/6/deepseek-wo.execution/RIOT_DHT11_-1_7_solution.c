
#include "kernel_defines.h"
#include "thread.h"
#include "msg.h"
#include "ztimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11

static dht_t dht_dev;
static msg_t msg_queue[8];

void read_dht11(void)
{
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

void dht_thread(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 5000);
        read_dht11();
    }
}

int main(void)
{
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE
    };

    if (dht_init(&dht_dev, &dht_params) != 0) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    thread_create(
        stack, sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        dht_thread,
        NULL, "dht_thread"
    );

    return 0;
}
