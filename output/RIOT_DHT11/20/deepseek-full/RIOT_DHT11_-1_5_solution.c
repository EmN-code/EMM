
#include "thread.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "event/thread.h"
#include "dht.h"
#include "mma8x5x_params.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define READ_INTERVAL       (5 * 1000 * 1000UL)  // 5 seconds in microseconds

static dht_t dht_dev;

void read_dht(void)
{
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
        } else {
            puts("Failed to read from DHT sensor");
        }
        xtimer_usleep(READ_INTERVAL);
    }
}

int main(void)
{
    if (gpio_init(DHT_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO");
        return 1;
    }

    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE
    };

    if (dht_init(&dht_dev, &dht_params) != 0) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    thread_create(thread_stack, sizeof(thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  read_dht, NULL, "dht_reader");

    return 0;
}
