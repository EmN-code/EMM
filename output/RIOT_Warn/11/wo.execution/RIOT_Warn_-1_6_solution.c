#include "string.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "thread.h"

#define DHT11_PIN GPIO_PIN(0, 5)  // Example pin configuration
#define LED_PIN GPIO_PIN(0, 6)
#define TEMP_THRESHOLD 30  // Temperature threshold in degrees Celsius

void read_temp_control_led(void) {
    gpio_t dht_pin = DHT11_PIN;
    gpio_t led_pin = LED_PIN;
    int16_t temperature, humidity;
    
    // Initialize the DHT11 device (assuming a similar function or manual setup)
    gpio_init(dht_pin, GPIO_IN);
    gpio_init(led_pin, GPIO_OUT);
    gpio_clear(led_pin);

    while (1) {
        if (dht_read(&dht_pin, &temperature, &humidity) == 0) {
            printf("Temperature: %d
", temperature);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(led_pin);
            } else {
                gpio_clear(led_pin);
            }
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }
}

int main(void) {
    // Start the temperature monitoring function
    read_temp_control_led();
    return 0;
}