
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

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static dht_t dht_dev;

void read_dht_task(void) {
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    if (gpio_init(DHT_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize GPIO\n");
        return 1;
    }

    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11
    };

    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, read_dht_task, NULL, "dht_reader");

    return 0;
}
