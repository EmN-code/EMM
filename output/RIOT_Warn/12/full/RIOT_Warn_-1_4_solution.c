#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"
#include "dht_params.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "periph/rtc.h"

#define DHT_PIN GPIO_PIN(PORT_A, 5)  // Assume the DHT is connected to pin PA5
#define LED_PIN GPIO_PIN(PORT_B, 0)  // Assume the LED is connected to pin PB0
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning (in ºC)

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    dht_t dev;
    dht_params_t params = DHT_PARAMS_DEFAULT;
    params.pin = DHT_PIN;
    dht_init(&dev, &params);

    int16_t temp, hum;

    while (1) {
        int res = dht_read(&dev, &temp, &hum);

        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
                printf("Temperature threshold exceeded! LED is ON.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is below threshold. LED is OFF.\n");
            }
        } else {
            printf("Error reading from DHT sensor\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}