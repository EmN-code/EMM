
#include "periph/gpio.h"
#include "mma8x5x.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static dht_t dht_dev;

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == 0) {
        printf("Temperature: %d, Humidity: %d\n", temp, hum);
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void) {
    if (dht_init(&dht_dev, DHT_TYPE_DHT11, DHT_PIN) < 0) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
