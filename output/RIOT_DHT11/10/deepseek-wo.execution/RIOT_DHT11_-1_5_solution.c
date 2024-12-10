
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "dht_params.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void _dht_cb(void *arg) {
    (void)arg;
    // Placeholder for any callback actions
}

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN, _dht_cb, NULL) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }

    return 0;
}
