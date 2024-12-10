
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "dht.h"
#include "event/thread.h"
#include "thread.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define DHT_PULL            GPIO_OD_PU

static dht_t dht_dev;

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == 0) {
        // Handle temperature and humidity readings
    } else {
        // Handle error
    }
}

int main(void) {
    // Initialize DHT11 sensor
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE,
        .in_mode = DHT_PULL
    };

    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        // Handle initialization error
        return 1;
    }

    while (1) {
        read_dht();
        // Sleep for 5 seconds
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    return 0;
}
