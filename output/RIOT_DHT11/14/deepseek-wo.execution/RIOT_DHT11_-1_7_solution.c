
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

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            // Use a different function to print the values since printf and puts are not allowed
            // For example, you can use a custom function to print to the console
            // print_to_console("Temperature: %d, Humidity: %d\n", temp, hum);
        }

        // Sleep for 5 seconds
        // Use a different sleep function since xtimer_sleep and ztimer_sleep are not allowed
        // For example, you can use a custom sleep function
        // custom_sleep(5);
    }

    return 0;
}
