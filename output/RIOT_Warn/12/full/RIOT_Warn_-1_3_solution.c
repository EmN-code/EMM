#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "dht.h"
#include "dht_params.h"
#include "thread.h"

#define DHT_PIN      GPIO_PIN(PORT_B, 2)
#define LED_PIN      GPIO_PIN(PORT_B, 3)
#define TEMP_THRESHOLD 25
#define I2C_DEV       I2C_DEV(0)

void setup_sensors(void) {
    i2c_init(I2C_DEV);
}

void control_led(int temperature) {
    if (temperature > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    puts("Starting the DHT11 Temperature Monitoring...");

    gpio_init(LED_PIN, GPIO_OUT);
    setup_sensors();

    int16_t temp, hum;
    dht_params_t params = DHT_PARAMS_DEFAULT;

    while (1) {
        int rc = dht_read(&params, DHT_PIN, &temp, &hum);
        if (rc != 0) {
            printf("Failed to read from DHT11 sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            control_led(temp);
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}