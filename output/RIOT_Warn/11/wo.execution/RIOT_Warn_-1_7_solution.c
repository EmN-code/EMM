#include "string.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "thread.h"
#include "paho_mqtt.h"

#define TEMP_THRESHOLD 30  // Define the temperature threshold
#define GPIO_LED GPIO_PIN(0, 0) // Define the GPIO pin for the LED

void setup_gpio(void) {
    gpio_init(GPIO_LED, GPIO_OUT);
}

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    setup_gpio(); // Set up the GPIO for the LED

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == ESP_OK) { // Correct the pointer type for the DHT sensor
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(GPIO_LED);  // Turn on LED if temperature exceeds threshold
            }
            else {
                gpio_clear(GPIO_LED);  // Turn off LED otherwise
            }
        }
        xtimer_sleep(5);  // Wait for 5 seconds before reading again
    }
    return 0;
}
