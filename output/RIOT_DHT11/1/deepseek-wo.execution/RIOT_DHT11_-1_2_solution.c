
#include "periph/gpio.h"
#include "thread.h"
#include "event/thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define DHT_TYPE            DHT11
#define DHT_PULL            GPIO_OD_PU

static dht_t dht_dev;

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    // Initialize the DHT sensor
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE,
        .in_mode = DHT_PULL
    };

    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        puts("DHT initialization failed");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
