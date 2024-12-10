#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Ensure correct port and pin number for the DHT11
#define LED_PIN GPIO_PIN(PORT_B, 1)  // Ensure correct port and pin number for the LED
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

void dht11_read_temperature(void) {
    dht_t dev;
    int16_t temp, hum;

    while (dht_init(&dev, &dht_params[0]) != 0) {
        printf("DHT initialization failed\n");
        xtimer_usleep(1000000); // retry initialization every 1 second
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_usleep(5000000); // wait 5 seconds before next read
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht11_read_temperature();
    return 0;
}